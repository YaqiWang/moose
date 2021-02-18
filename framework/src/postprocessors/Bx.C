//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "Bx.h"

registerMooseObject("MooseApp", Bx);

#include "libmesh/quadrature.h"

InputParameters
Bx::validParams()
{
  InputParameters params = ElementPostprocessor::validParams();
  params.addRequiredCoupledVar("variable", "The name of the variable that this object operates on");
  params.addParam<Real>("coefficient", 1.0, "Coefficient of the term");
  return params;
}

Bx::Bx(const InputParameters & parameters)
  : ElementPostprocessor(parameters),
    _u(coupledValue("variable")),
    _test(getVar("variable", 0)->phi()),
    _coef(getParam<Real>("coefficient"))
{
}

void
Bx::initialize()
{
  _integral_value = 0;
}

void
Bx::execute()
{
  _integral_value += computeIntegral();
}

Real
Bx::getValue()
{
  gatherSum(_integral_value);
  return std::sqrt(_integral_value) * _coef;
}

void
Bx::threadJoin(const UserObject & y)
{
  const Bx & pps = static_cast<const Bx &>(y);
  _integral_value += pps._integral_value;
}

Real
Bx::computeIntegral()
{
  Real sum = 0;
  for (unsigned int i = 0; i < _test.size(); i++)
  {
    Real v = 0;
    for (unsigned int qp = 0; qp < _qrule->n_points(); qp++)
      v += _JxW[qp] * _coord[qp] * _test[i][qp] * _u[qp];
    sum += v * v;
  }
  return sum;
}
