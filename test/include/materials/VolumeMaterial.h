//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "Material.h"

class VolumeMaterial : public Material
{
public:
  static InputParameters validParams();

  VolumeMaterial(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

  std::vector<std::string> _prop_names;
  std::vector<Real> _prop_values;

  unsigned int _num_props;

  std::vector<MaterialProperty<Real> *> _properties;
};
