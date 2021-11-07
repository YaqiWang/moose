//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "MeshGenerator.h"

class CoarseMeshElementID : public MeshGenerator
{
public:
  static InputParameters validParams();
  CoarseMeshElementID(const InputParameters & parameters);

  virtual std::unique_ptr<MeshBase> generate() override;

protected:
  /// input mesh for adding element IDs
  std::unique_ptr<MeshBase> & _input;
  /// name of the coarse mesh file
  std::unique_ptr<MeshBase> & _coarse_mesh;
  /// coarse element ID name
  const std::string _coarse_id_name;
  /// whether the input mesh must be embedded in the coarse mesh
  const bool _must_embedding;
};
