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

#include "DirectCoupledAux.h"

template<>
InputParameters validParams<DirectCoupledAux>()
{
  InputParameters params = validParams<AuxKernel>();

  MooseEnum operators("+, -, *", "+");

  params.addRequiredCoupledVar("coupled", "Coupled Value for Calculation");

  params.addParam<Real>("value", 0.0, "A value to use in the binary arithmetic operation of this coupled auxkernel");
  params.addParam<MooseEnum>("operator", operators, "The binary operator to use in the calculation");
  return params;
}

DirectCoupledAux::DirectCoupledAux(const std::string & name, InputParameters parameters) :
    AuxKernel(name, parameters),
    _value(getParam<Real>("value")),
    _operator(getParam<MooseEnum>("operator")),
    _coupled(coupled("coupled")),
    _coupled_sln(coupledSln("coupled"))
{
  if (_var.feType() != getVar("coupled")->feType())
    mooseError("coupled variable is not in the same type");

  if ((_var.feType().family != MONOMIAL)      &&
      (_var.feType().family != LAGRANGE)      &&
      (_var.feType().family != HIERARCHIC)    &&
      (_var.feType().family != L2_LAGRANGE)   &&
      (_var.feType().family != L2_HIERARCHIC))
    mooseError("Direct evaluation is not implemented for the FEFamily");

  /*
   * Note: this aux kernel does not requre a numerical quadrature, so the system
   *       quadrature order does not have to be greater than the default quadrature
   *       order of the variable this kernel is operating on. However, the variable
   *       being coupled in probably need the quadrature, so it does not hurt to
   *       let the variable require the qudrature with the order.
   */
}

void
DirectCoupledAux::compute()
{
  if (isNodal())
  {
    if (_var.isNodalDefined())
    {
      _qp = 0;
      Real value = computeValue();
      _var.setNodalValue(value);
    }
  }
  else
  {
    _n_local_dofs = _var.numberOfDofs();
    _local_sol.resize(_n_local_dofs);
    for (_qp=0; _qp<_n_local_dofs; _qp++)
      _local_sol(_qp) = _coupled_sln[_qp];

    // different shape functions need different treatments
    switch (_var.feType().family)
    {
    case MONOMIAL:
    {
      if (_operator == "+")
        _local_sol(0) += _value;
      else if (_operator == "-")
        _local_sol(0) -= _value;
      else if (_operator == "*")
        _local_sol *= _value;
      break;
    }
    case L2_LAGRANGE:
    {
      if (_operator == "+")
        for (_qp=0; _qp<_n_local_dofs; _qp++)
          _local_sol(_qp) += _value;
      else if (_operator == "-")
        for (_qp=0; _qp<_n_local_dofs; _qp++)
          _local_sol(_qp) -= _value;
      else if (_operator == "*")
        _local_sol *= _value;
      break;
    }
    case L2_HIERARCHIC:
    {
      //FIXME: not implemented
      mooseError("L2_HIERARCHIC can not be directly evaluated");
      break;
    }
    }

    _var.setNodalValue(_local_sol);
  }
}

Real
DirectCoupledAux::computeValue()
{
  // do nothing
  return 0;
}
