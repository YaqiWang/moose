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

#ifndef VOLUMEMATERIAL_H
#define VOLUMEMATERIAL_H

#include "Material.h"

//Forward Declarations
class VolumeMaterial;

template<>
InputParameters validParams<VolumeMaterial>();

class VolumeMaterial : public Material
{
public:
  VolumeMaterial(const std::string & name, InputParameters parameters);

protected:
  virtual void computeQpProperties();

  std::vector<std::string> _prop_names;
  std::vector<Real> _prop_values;

  unsigned int _num_props;

  std::vector<MaterialProperty<Real> *> _properties;
};

#endif //VOLUMEMATERIAL_H
