/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "ElementMomentSum.h"

#include "MooseVariable.h"

template <>
InputParameters
validParams<ElementMomentSum>()
{
  InputParameters params = validParams<ElementIntegralVariablePostprocessor>();
  return params;
}

ElementMomentSum::ElementMomentSum(const InputParameters & parameters)
    : ElementIntegralVariablePostprocessor(parameters), _nodal_u(getVar("variable", 0)->nodalSln())
{
}

void
ElementMomentSum::execute()
{
  _integral_value += _nodal_u[0];
}
