//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "MooseTypes.h"
#include "MultiMooseEnum.h"

#include "libmesh/dense_vector.h"
#include "libmesh/dense_matrix.h"

// Forward declarations
class InputParameters;
class MooseObject;
class TaggingInterface;
class SubProblem;

template <>
InputParameters validParams<TaggingInterface>();

class TaggingInterface
{
public:
  TaggingInterface(const MooseObject * moose_object);
  virtual ~TaggingInterface();

  void useVectorTag(const TagName & tag_name);

  void useMatrixTag(const TagName & tag_name);

  void useVectorTag(TagID tag_id);

  void useMatrixTag(TagID tag_id);

  bool isVectorTagged() { return _vector_tags.size() > 0; }

  bool isMatrixTagged() { return _matrix_tags.size() > 0; }

  const std::set<TagID> & getVectorTags() const { return _vector_tags; }

  const std::set<TagID> & getMatrixTags() const { return _matrix_tags; }

protected:
  /// The vectors this Kernel will contribute to
  std::set<TagID> _vector_tags;

  /// The matrices this Kernel will contribute to
  std::set<TagID> _matrix_tags;

  /// Moose objct this tag works on
  const MooseObject & _moose_object;

  /// Parameters from moose object
  const InputParameters & _tag_params;

  /// SubProblem that contains tag info
  SubProblem & _tag_subproblem;
};
