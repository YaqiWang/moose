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

class CoarseElementIDFixup : public MeshGenerator
{
public:
  static InputParameters validParams();
  CoarseElementIDFixup(const InputParameters & params);

  std::unique_ptr<MeshBase> generate() override;

protected:
  /// Mesh that comes from another generator
  std::unique_ptr<MeshBase> & _input;
  /// Flag to indicate whether we want screen outputs from this generator
  const bool _verbose;
};
