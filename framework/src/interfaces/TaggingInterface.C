//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "TaggingInterface.h"
#include "Conversion.h"
#include "FEProblem.h"
#include "Assembly.h"

#include "libmesh/dense_vector.h"

template <>
InputParameters
validParams<TaggingInterface>()
{
  InputParameters params = emptyInputParameters();

  // These are the default names for tags, but users will be able to add their own
  MultiMooseEnum vtags("nontime time", "nontime", true);
  MultiMooseEnum mtags("nontime system", "system", true);

  params.addParam<MultiMooseEnum>(
      "vector_tags", vtags, "The tag for the vectors this Kernel should fill");

  params.addParam<MultiMooseEnum>(
      "matrix_tags", mtags, "The tag for the matrices this Kernel should fill");

  params.addParam<std::vector<TagName>>("extra_vector_tags",
                                        "The extra tags for the vectors this Kernel should fill");

  params.addParam<std::vector<TagName>>("extra_matrix_tags",
                                        "The extra tags for the matrices this Kernel should fill");

  params.addParamNamesToGroup("vector_tags matrix_tags extra_vector_tags extra_matrix_tags",
                              "Tagging");

  return params;
}

TaggingInterface::TaggingInterface(const MooseObject * moose_object)
  : _moose_object(*moose_object),
    _tag_params(_moose_object.parameters()),
    _tag_subproblem(*_tag_params.getCheckedPointerParam<SubProblem *>("_subproblem"))
{
  auto & vector_tag_names = _tag_params.get<MultiMooseEnum>("vector_tags");

  if (!vector_tag_names.isValid())
    mooseError("MUST provide at least one vector_tag for Kernel: ", _moose_object.name());

  for (auto & vector_tag_name : vector_tag_names)
    _vector_tags.insert(_tag_subproblem.getVectorTagID(vector_tag_name.name()));

  // Add extra vector tags. These tags should be created in the System already, otherwise
  // we can not add the extra tags
  auto & extra_vector_tags = _tag_params.get<std::vector<TagName>>("extra_vector_tags");

  for (auto & vector_tag_name : extra_vector_tags)
    _vector_tags.insert(_tag_subproblem.getVectorTagID(vector_tag_name));

  auto & matrix_tag_names = _tag_params.get<MultiMooseEnum>("matrix_tags");

  if (!matrix_tag_names.isValid())
    mooseError("MUST provide at least one matrix_tag for Kernel: ", _moose_object.name());

  for (auto & matrix_tag_name : matrix_tag_names)
    _matrix_tags.insert(_tag_subproblem.getMatrixTagID(matrix_tag_name.name()));

  auto & extra_matrix_tags = _tag_params.get<std::vector<TagName>>("extra_matrix_tags");

  for (auto & matrix_tag_name : extra_matrix_tags)
    _matrix_tags.insert(_tag_subproblem.getMatrixTagID(matrix_tag_name));
}

void
TaggingInterface::useVectorTag(const TagName & tag_name)
{
  if (!_tag_subproblem.vectorTagExists(tag_name))
    mooseError("Vector tag ", tag_name, " does not exsit in system");

  _vector_tags.insert(_tag_subproblem.getVectorTagID(tag_name));
}

void
TaggingInterface::useMatrixTag(const TagName & tag_name)
{
  if (!_tag_subproblem.matrixTagExists(tag_name))
    mooseError("Matrix tag ", tag_name, " does not exsit in system");

  _matrix_tags.insert(_tag_subproblem.getMatrixTagID(tag_name));
}

void
TaggingInterface::useVectorTag(TagID tag_id)
{
  if (!_tag_subproblem.vectorTagExists(tag_id))
    mooseError("Vector tag ", tag_id, " does not exsit in system");

  _vector_tags.insert(tag_id);
}

void
TaggingInterface::useMatrixTag(TagID tag_id)
{
  if (!_tag_subproblem.matrixTagExists(tag_id))
    mooseError("Matrix tag ", tag_id, " does not exsit in system");

  _matrix_tags.insert(tag_id);
}

TaggingInterface::~TaggingInterface() {}
