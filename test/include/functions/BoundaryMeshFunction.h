//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "Function.h"
#include "BoundaryRestrictableRequired.h"

class BoundaryMeshFunction : public Function, public BoundaryRestrictableRequired
{
public:
  static InputParameters validParams();

  BoundaryMeshFunction(const InputParameters & parameters);

  using Function::value;
  virtual Real value(Real t, const Point & p) const override;

protected:
  /// The subproblem this function is used for
  SubProblem & _subproblem;
  /// The mesh of the subproblem
  MooseMesh & _mesh;
  /// Thread ID
  const THREAD_ID _tid;
  /// The boundary mesh built from boundaries
  std::unique_ptr<MeshBase> _boundary_mesh;
};
