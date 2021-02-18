//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "ElementPostprocessor.h"

class Bx : public ElementPostprocessor
{
public:
  static InputParameters validParams();

  Bx(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void execute() override;
  virtual void threadJoin(const UserObject & y) override;
  virtual Real getValue() override;

protected:
  virtual Real computeIntegral();

  const VariableValue & _u;
  const VariableTestValue & _test;
  const Real & _coef;
  Real _integral_value;
};
