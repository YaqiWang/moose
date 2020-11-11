//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "ScalarKernel.h"

// Forward Declarations
class ODEKernel;

template <>
InputParameters validParams<ODEKernel>();

/**
 *
 */
class ODEKernel : public ScalarKernel
{
public:
  static InputParameters validParams();

  ODEKernel(const InputParameters & parameters);

  virtual void reinit() override;
  virtual void computeResidual() override;
  virtual void computeJacobian() override;
  virtual void computeOffDiagJacobianScalar(unsigned int jvar) override;

protected:
  virtual Real computeQpResidual() = 0;
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobianScalar(unsigned int jvar)
  {
    // Backwards compatibility
    return computeQpOffDiagJacobian(jvar);
  }

  virtual Real computeQpOffDiagJacobian(unsigned int) { return 0; }
};
