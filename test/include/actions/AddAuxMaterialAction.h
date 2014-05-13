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

#ifndef ADDAUXMATERIALACTION_H
#define ADDAUXMATERIALACTION_H

#include "MooseObjectAction.h"

class AddAuxMaterialAction;

template<>
InputParameters validParams<AddAuxMaterialAction>();


class AddAuxMaterialAction : public MooseObjectAction
{
public:
  AddAuxMaterialAction(const std::string & name, InputParameters params);

  virtual void act();
};

#endif // ADDAUXMATERIALACTION_H
