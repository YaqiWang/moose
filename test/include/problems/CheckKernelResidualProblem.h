//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "FEProblem.h"

class CheckKernelResidualProblem : public FEProblem
{
public:
  static InputParameters validParams();

  CheckKernelResidualProblem(const InputParameters & params);

  virtual void addKernel(const std::string & kernel_name,
                         const std::string & name,
                         InputParameters & parameters) override;
  virtual void computeResidualTags(const std::set<TagID> & tags) override;

protected:
  bool _residual_dbg_on;
  TagID _residual_dbg_tag;
  NumericVector<Number> * _residual_debug;
  std::set<std::string> _kernels;
};
