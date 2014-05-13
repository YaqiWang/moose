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

#include "AddAuxMaterialAction.h"
#include "FEProblem.h"

template<>
InputParameters validParams<AddAuxMaterialAction>()
{
  InputParameters params = validParams<MooseObjectAction>();
  params.addRequiredParam<std::string >("added_material_property", "something");
  return params;
}

AddAuxMaterialAction::AddAuxMaterialAction(const std::string & name, InputParameters params) :
    MooseObjectAction(name, params)
{
}

void
AddAuxMaterialAction::act()
{
  std::vector<SubdomainName> blk = _problem->getMaterialPropertyBlockNames(_pars.get<std::string>("added_material_property"));
  std::cout << "before blocks=";
  for (unsigned int ii=0; ii<blk.size(); ii++)
    std::cout << blk[ii] << " ";
  std::cout << std::endl;

  _problem->addMaterial(_type, getShortName(), _moose_object_pars);

  blk = _problem->getMaterialPropertyBlockNames(_pars.get<std::string>("added_material_property"));
  std::cout << "after blocks=";
  for (unsigned int ii=0; ii<blk.size(); ii++)
    std::cout << blk[ii] << " ";
  std::cout << std::endl;
}
