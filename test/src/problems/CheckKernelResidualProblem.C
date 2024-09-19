//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "CheckKernelResidualProblem.h"
#include "NonlinearSystem.h"

registerMooseObject("MooseTestApp", CheckKernelResidualProblem);

InputParameters
CheckKernelResidualProblem::validParams()
{
  InputParameters params = FEProblem::validParams();
  params.addParam<std::vector<std::string>>(
      "evaluate_residual_norm_for_kernels",
      "A list of kernels that we want to show their contribution to the residual norm");
  return params;
}

CheckKernelResidualProblem::CheckKernelResidualProblem(const InputParameters & params) : FEProblem(params),
    _residual_dbg_on(isParamValid("evaluate_residual_norm_for_kernels"))
{
  if (_residual_dbg_on)
  {
    _residual_dbg_tag = addVectorTag("residual_debug");
    _residual_debug =
        &getNonlinearSystemBase(0).addVector(_residual_dbg_tag, false, GHOSTED);
    const auto & kernels = getParam<std::vector<std::string>>("evaluate_residual_norm_for_kernels");
    for (const auto & kernel : kernels)
      _kernels.insert(kernel);
  }
}

void
CheckKernelResidualProblem::addKernel(const std::string & kernel_name,
                          const std::string & name,
                          InputParameters & parameters)
{
  if (_kernels.count(name) > 0)
    parameters.set<std::vector<TagName>>("extra_vector_tags").push_back("residual_debug");
  FEProblem::addKernel(kernel_name, name, parameters);
}

void
CheckKernelResidualProblem::computeResidualTags(const std::set<TagID> & tags)
{
  auto atags = tags;
  if (_residual_dbg_on)
    atags.insert(_residual_dbg_tag);

  FEProblem::computeResidualTags(atags);

  if (_residual_dbg_on)
  {
    std::ostringstream ss;
    ss << " Kernel residual norm: " << std::setprecision(10) << _residual_debug->l2_norm();
    _console << ss.str() << std::endl;
  }
}
