//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "CoarseMeshElementID.h"

registerMooseObject("MooseApp", CoarseMeshElementID);

#include "libmesh/enum_point_locator_type.h"
#include "libmesh/elem.h"
#include "libmesh/mesh_serializer.h"

InputParameters
CoarseMeshElementID::validParams()
{
  InputParameters params = MeshGenerator::validParams();
  params.addRequiredParam<MeshGeneratorName>(
      "input", "Name of an existing mesh generator to which we assign coarse element IDs");
  params.addRequiredParam<MeshGeneratorName>(
      "coarse_mesh", "Name of an existing mesh generator as the coarse mesh");
  params.addRequiredParam<std::string>("coarse_element_id_name", "Name for the coarse element ID");
  params.addParam<bool>("enforce_mesh_embedding",
                        false,
                        "True to error out when the input mesh is not embedded in the coarse mesh");

  params.addClassDescription("Assign coarse element IDs for elements on a "
                             "mesh based on a coarse mesh.");
  return params;
}

CoarseMeshElementID::CoarseMeshElementID(const InputParameters & params)
  : MeshGenerator(params),
    _input(getMesh("input")),
    _coarse_mesh(getMesh("coarse_mesh")),
    _coarse_id_name(getParam<std::string>("coarse_element_id_name")),
    _must_embedding(getParam<bool>("enforce_mesh_embedding"))
{
}

std::unique_ptr<MeshBase>
CoarseMeshElementID::generate()
{
  std::unique_ptr<MeshBase> mesh = std::move(_input);

  std::unique_ptr<MeshBase> coarse_mesh = std::move(_coarse_mesh);

  unsigned int coarse_id;
  if (!mesh->has_elem_integer(_coarse_id_name))
    coarse_id = mesh->add_elem_integer(_coarse_id_name);
  else
    coarse_id = mesh->get_elem_integer_index(_coarse_id_name);

  // a tolerance on checking if fine mesh is embedded in the coarse mesh
  Real aeps = 0.01;

  // we need the following line to make this generator work with distributed mesh.
  // Note: this will be a null operation for a replicated mesh.
  MeshSerializer tm(*coarse_mesh);

  // build a point_locator on coarse mesh
  UniquePtr<PointLocatorBase> point_locator = PointLocatorBase::build(TREE_ELEMENTS, *coarse_mesh);

  // loop through fine mesh elements and get element's centroid
  for (auto & elem : mesh->active_element_ptr_range())
  {
    // Get the centroid of the fine elem
    Point centroid = elem->true_centroid();

    // Find coarse elem
    const Elem * coarse_elem = (*point_locator)(centroid);

    // Check if the fine elem is nested in the coarse element
    for (unsigned int n = 0; n < elem->n_nodes(); n++)
    {
      // Get the node: we need to manually move it towards the centroid to
      // ensure that nothing weird happes due to round-off
      Node current_node = elem->node_ref(n);
      current_node.add_scaled(current_node, -aeps);
      current_node.add_scaled(centroid, aeps);

      // Get the element this node is in and check if it is the same
      // as the coarse elem; however check if node_elem is valid as it
      // might not be in case the sub element it outside the coarse domain
      const Elem * node_elem = (*point_locator)(current_node);
      if (!node_elem)
        mooseError("Could not detect node_elem. Mesh boundaries of the input mesh and the coarse "
                   "mesh might be "
                   "different.");
      if (node_elem->id() != coarse_elem->id())
        if (_must_embedding)
          mooseError("Input mesh is not nested in the coarse mesh in CoarseMeshElementID.");
    }

    elem->set_extra_integer(coarse_id, coarse_elem->id());
  }

  return mesh;
}
