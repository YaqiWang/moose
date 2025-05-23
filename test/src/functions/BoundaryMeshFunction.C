//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "BoundaryMeshFunction.h"

#include "FEProblem.h"

#include "libmesh/boundary_info.h"
#include "libmesh/mesh_serializer.h"

registerMooseObject("MooseTestApp", BoundaryMeshFunction);

InputParameters
BoundaryMeshFunction::validParams()
{
  InputParameters params = Function::validParams();
  // for an iterior boundary side, the associated elements are inside of the transport domain
  params += BoundaryRestrictableRequired::validParams();
  return params;
}

BoundaryMeshFunction::BoundaryMeshFunction(const InputParameters & parameters)
  : Function(parameters),
    BoundaryRestrictableRequired(this, false),
    _subproblem(*getCheckedPointerParam<SubProblem *>("_subproblem")),
    _mesh(_subproblem.mesh()),
    _tid(getParam<THREAD_ID>("_tid")),
    _boundary_mesh(std::make_unique<DistributedMesh>(comm(), _mesh.dimension() - 1))
{
  auto & boundary_info = _mesh.getMesh().get_boundary_info();
  boundary_info.sync(boundaryIDs(), dynamic_cast<UnstructuredMesh &>(*_boundary_mesh));
  MeshSerializer tm(*_boundary_mesh);
  _boundary_mesh->print_info();
}

Real
BoundaryMeshFunction::value(Real /*t*/, const Point & p) const
{
  return p(0);
}
