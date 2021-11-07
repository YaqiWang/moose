//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "CoarseElementIDFixup.h"

#include "libmesh/elem.h"
#include "libmesh/mesh_serializer.h"

registerMooseObject("MooseApp", CoarseElementIDFixup);

InputParameters
CoarseElementIDFixup::validParams()
{
  InputParameters params = MeshGenerator::validParams();
  params.addRequiredParam<MeshGeneratorName>("input", "The mesh we want to modify");
  params.addRequiredParam<std::string>("source_element_id", "The element ID to be fixed up");
  params.addParam<bool>("verbose", false, "True to show what fine elements get ID reassignment");
  params.addParam<bool>(
      "order_ids_with_centroid",
      false,
      "True to reassign elements IDs based on the centroid of all elements with the same IDs");
  params.addClassDescription("Check and change element IDs so that all fine elements with the same "
                             "element ID are connected through sides.");
  return params;
}

CoarseElementIDFixup::CoarseElementIDFixup(const InputParameters & params)
  : MeshGenerator(params), _input(getMesh("input")), _verbose(getParam<bool>("verbose"))
{
}

std::unique_ptr<MeshBase>
CoarseElementIDFixup::generate()
{
  std::unique_ptr<MeshBase> mesh = std::move(_input);

  auto src_id_name = getParam<std::string>("source_element_id");
  if (!mesh->has_elem_integer(src_id_name))
    mooseError("The source element ID does not exist on the input mesh");

  // need to serialize the mesh for fixup
  MeshSerializer tm(*mesh);

  auto src_id = mesh->get_elem_integer_index(src_id_name);

  // a map from coarse element ids to fine element ids
  std::map<dof_id_type, std::set<dof_id_type>> map_coarse_to_fine;
  // a map from nodes to connected fine element ids
  std::map<dof_id_type, std::set<dof_id_type>> map_node_to_fine;
  // a set for coarse eleemnt IDs
  std::set<dof_id_type> coarse_ids;

  // build the two maps and one sets above by doing one element loop
  for (auto & elem : mesh->element_ptr_range())
  {
    dof_id_type id = elem->get_extra_integer(src_id);
    if (id == DofObject::invalid_id)
      continue;

    map_coarse_to_fine[id].insert(elem->id());
    coarse_ids.insert(id);

    for (auto & node : elem->node_ref_range())
      map_node_to_fine[node.id()].insert(elem->id());
  }

  if (_verbose)
    _console << " Number of coarse elements before fixup: " << coarse_ids.size() << std::endl;

  while (!coarse_ids.empty())
  {
    const dof_id_type coarse_id = *coarse_ids.begin();
    coarse_ids.erase(coarse_id);
    const auto & fine_ids = map_coarse_to_fine[coarse_id];

    // the coarse element could contain no fine element due to fixup
    if (fine_ids.size() == 0)
      continue;

    // merge the coarse element with only a single fine element into its neighboring coarse element
    if (fine_ids.size() == 1)
    {
      Elem * elem = mesh->elem_ptr(*fine_ids.begin());
      for (auto neig : elem->neighbor_ptr_range())
        if (neig)
        {
          const dof_id_type newid = neig->get_extra_integer(src_id);
          if (_verbose)
            _console << " Single element with ID " << elem->id() << ": " << coarse_id << " => "
                     << newid << std::endl;
          elem->set_extra_integer(src_id, newid);
          map_coarse_to_fine[coarse_id].erase(elem->id());
          map_coarse_to_fine[newid].insert(elem->id());

          // put changed ids into re-consideration
          coarse_ids.insert(newid);
          break;
        }

      continue;
    }

    // fine element ids that has been visited
    std::set<dof_id_type> visited;
    // fine element buffer for visiting
    std::vector<const Elem *> buffer;
    // nodes of fine elements that have been visited
    std::set<dof_id_type> visited_nodes;
    // a copy of fine elements to track unvisited elements
    auto fine_ids_copy = fine_ids;

    while (!fine_ids_copy.empty())
    {
      buffer.push_back(mesh->elem_ptr(*fine_ids_copy.begin()));

      // nodes of all elements in this current visit
      std::set<dof_id_type> nodes;
      while (buffer.size() != 0)
      {
        const Elem * elem = buffer.back();
        visited.insert(elem->id());
        fine_ids_copy.erase(elem->id());
        buffer.pop_back();

        for (unsigned int n = 0; n < elem->n_nodes(); ++n)
          nodes.insert(elem->node_ptr(n)->id());

        // push connected elements through sides with the same coarse ID into the buffer
        for (auto neig : elem->neighbor_ptr_range())
          if (neig && neig->get_extra_integer(src_id) == coarse_id &&
              visited.count(neig->id()) == 0)
            buffer.push_back(neig);
      }

      // merge nodes
      if (!visited_nodes.empty())
      {
        std::set<dof_id_type> int_nodes;
        std::set_intersection(visited_nodes.begin(),
                              visited_nodes.end(),
                              nodes.begin(),
                              nodes.end(),
                              std::inserter(int_nodes, int_nodes.begin()));

        if (int_nodes.size() == 0)
          mooseError("Coarse elements with ID ", coarse_id, " has disconnected parts!");

        // we will do a simple merge by assigning all elements connecting to the intersection nodes
        // with the current coarse element ID
        // FIXME: checking the connection through line segments in 3D can avoid some unnecessary
        // assignment.
        for (const auto & id : int_nodes)
          for (const auto & elem_id : map_node_to_fine[id])
          {
            Elem * elem = mesh->elem_ptr(elem_id);
            auto old_id = elem->get_extra_integer(src_id);
            if (old_id != coarse_id)
            {
              if (_verbose)
                _console << " Element with ID " << elem->id() << ": " << old_id << " => "
                         << coarse_id << std::endl;
              elem->set_extra_integer(src_id, coarse_id);
              map_coarse_to_fine[old_id].erase(elem->id());
              map_coarse_to_fine[coarse_id].insert(elem->id());
              for (unsigned int n = 0; n < elem->n_nodes(); ++n)
                visited_nodes.insert(elem->node_ptr(n)->id());

              // put changed ids into re-consideration
              coarse_ids.insert(old_id);
            }
          }
      }
      for (auto & id : nodes)
        visited_nodes.insert(id);
    }
  }

  std::map<dof_id_type, Real> element_volume;
  std::map<dof_id_type, Point> element_centroid;
  for (auto & elem : mesh->element_ptr_range())
  {
    dof_id_type id = elem->get_extra_integer(src_id);
    if (id == DofObject::invalid_id)
      continue;

    element_volume[id] += elem->volume();
    element_centroid[id] += elem->true_centroid() * elem->volume();
  }
  for (auto & pair : element_centroid)
    pair.second /= element_volume[pair.first];
  if (_verbose)
    _console << " Number of coarse elements after fixup: " << element_volume.size() << std::endl;

  if (getParam<bool>("order_ids_with_centroid"))
  {
    std::vector<std::pair<dof_id_type, Point>> sorted_centroids;
    sorted_centroids.reserve(element_centroid.size());
    for (const auto & it : element_centroid)
      sorted_centroids.push_back(std::make_pair(it.first, it.second));
    // Our sorting functor
    auto sort_fun = [](const std::pair<dof_id_type, Point> & lhs,
                       const std::pair<dof_id_type, Point> & rhs) -> bool {
      return lhs.second < rhs.second;
    };
    std::sort(sorted_centroids.begin(), sorted_centroids.end(), sort_fun);
    std::map<dof_id_type, dof_id_type> new_element_ids;
    for (dof_id_type i = 0; i < sorted_centroids.size(); ++i)
      new_element_ids[sorted_centroids[i].first] = i;

    for (auto & elem : mesh->element_ptr_range())
    {
      dof_id_type id = elem->get_extra_integer(src_id);
      if (id != DofObject::invalid_id)
        elem->set_extra_integer(src_id, new_element_ids[id]);
    }
    if (_verbose)
      _console << " Reassign IDs from 0 to " << new_element_ids.size() - 1
               << " to elements based on the sequence of the ordered IDs" << std::endl;
  }

  return mesh;
}
