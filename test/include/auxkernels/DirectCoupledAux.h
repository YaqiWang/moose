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

#ifndef DIRECTCOUPLEDAUX_H
#define DIRECTCOUPLEDAUX_H

#include "AuxKernel.h"


//Forward Declarations
class DirectCoupledAux;

template<>
InputParameters validParams<DirectCoupledAux>();

/**
 * Coupled auxiliary value
 */
class DirectCoupledAux : public AuxKernel
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  DirectCoupledAux(const std::string & name, InputParameters parameters);

  virtual ~DirectCoupledAux() {}

protected:
  virtual void compute();
  virtual Real computeValue();

  Real _value;                                  ///< The value being set for this kernel
  MooseEnum _operator;                          ///< Operator being applied on this variable and coupled variable

  int _coupled;                                 ///< The number of the coupled variable
  VariableValue & _coupled_sln;                 ///< Coupled variable
};

#endif //DIRECTCOUPLEDAUX_H
