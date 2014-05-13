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

#ifndef AUXCONSTANTMATERIAL_H
#define AUXCONSTANTMATERIAL_H

#include "Material.h"


//Forward Declarations
class AuxConstantMaterial;

template<>
InputParameters validParams<AuxConstantMaterial>();

/**
 * This material automatically declares as material properties whatever is passed to it
 * through the parameters 'prop_names' and uses the values from 'prop_values' as the values
 * for those properties.
 *
 * This is not meant to be used in a production capacity... and instead is meant to be used
 * during development phases for ultimate flexibility.
 */
class AuxConstantMaterial : public Material
{
public:
  AuxConstantMaterial(const std::string & name, InputParameters parameters);

protected:
  virtual void computeQpProperties();

  std::vector<std::string> _prop_names;
  std::vector<Real> _prop_values;

  MaterialProperty<Real> * _added_prop;
  unsigned int _num_props;

  std::vector<MaterialProperty<Real> *> _properties;
};

#endif //AUXCONSTANTMATERIAL_H
