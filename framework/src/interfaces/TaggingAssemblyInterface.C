//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "TaggingAssemblyInterface.h"
#include "Conversion.h"
#include "FEProblem.h"
#include "Assembly.h"

#include "libmesh/dense_vector.h"

template <typename T>
TaggingAssemblyInterface<T>::TaggingAssemblyInterface(const MooseObject * moose_object)
  : _tag_interface(dynamic_cast<const TaggingInterface &>(*moose_object)),
    _vector_tags_ref(_tag_interface.getVectorTags()),
    _matrix_tags_ref(_tag_interface.getMatrixTags())
{
  _re_blocks.resize(_vector_tags_ref.size());
  _ke_blocks.resize(_matrix_tags_ref.size());
}

template <typename T>
void
TaggingAssemblyInterface<T>::prepareVectorTag(Assembly & assembly, unsigned int ivar)
{
  _re_blocks.resize(_vector_tags_ref.size());
  mooseAssert(_vector_tags_ref.size() >= 1, "we need at least one active tag");
  auto vector_tag = _vector_tags_ref.begin();
  for (MooseIndex(_vector_tags_ref) i = 0; i < _vector_tags_ref.size(); i++, ++vector_tag)
    _re_blocks[i] = &assembly.residualBlock(ivar, *vector_tag);

  _local_re.resize(_re_blocks[0]->size());
  _local_re.zero();
}

template <typename T>
void
TaggingAssemblyInterface<T>::prepareVectorTagNeighbor(Assembly & assembly, unsigned int ivar)
{
  _re_blocks.resize(_vector_tags_ref.size());
  mooseAssert(_vector_tags_ref.size() >= 1, "we need at least one active tag");
  auto vector_tag = _vector_tags_ref.begin();
  for (MooseIndex(_vector_tags_ref) i = 0; i < _vector_tags_ref.size(); i++, ++vector_tag)
    _re_blocks[i] = &assembly.residualBlockNeighbor(ivar, *vector_tag);

  _local_re.resize(_re_blocks[0]->size());
  _local_re.zero();
}

template <typename T>
void
TaggingAssemblyInterface<T>::prepareMatrixTag(Assembly & assembly,
                                              unsigned int ivar,
                                              unsigned int jvar)
{
  _ke_blocks.resize(_matrix_tags_ref.size());
  mooseAssert(_matrix_tags_ref.size() >= 1, "we need at least one active tag");
  auto mat_vector = _matrix_tags_ref.begin();
  for (MooseIndex(_matrix_tags_ref) i = 0; i < _matrix_tags_ref.size(); i++, ++mat_vector)
    _ke_blocks[i] = &assembly.jacobianBlock(ivar, jvar, *mat_vector);

  _local_ke.resize(_ke_blocks[0]->m(), _ke_blocks[0]->n());
  _local_ke.zero();
}

template <typename T>
void
TaggingAssemblyInterface<T>::prepareMatrixTagNeighbor(Assembly & assembly,
                                                      unsigned int ivar,
                                                      unsigned int jvar,
                                                      Moose::DGJacobianType type)
{
  _ke_blocks.resize(_matrix_tags_ref.size());
  mooseAssert(_matrix_tags_ref.size() >= 1, "we need at least one active tag");
  auto mat_vector = _matrix_tags_ref.begin();
  for (MooseIndex(_matrix_tags_ref) i = 0; i < _matrix_tags_ref.size(); i++, ++mat_vector)
    _ke_blocks[i] = &assembly.jacobianBlockNeighbor(type, ivar, jvar, *mat_vector);

  _local_ke.resize(_ke_blocks[0]->m(), _ke_blocks[0]->n());
  _local_ke.zero();
}

template <typename T>
void
TaggingAssemblyInterface<T>::accumulateTaggedLocalResidual()
{
  for (auto & re : _re_blocks)
    *re += _local_re;
}

template <typename T>
void
TaggingAssemblyInterface<T>::assignTaggedLocalResidual()
{
  for (auto & re : _re_blocks)
    *re = _local_re;
}

template <typename T>
void
TaggingAssemblyInterface<T>::accumulateTaggedLocalMatrix()
{
  for (auto & ke : _ke_blocks)
    *ke += _local_ke;
}

template <typename T>
void
TaggingAssemblyInterface<T>::assignTaggedLocalMatrix()
{
  for (auto & ke : _ke_blocks)
    *ke = _local_ke;
}

template <typename T>
TaggingAssemblyInterface<T>::~TaggingAssemblyInterface()
{
}

template class TaggingAssemblyInterface<Real>;
template class TaggingAssemblyInterface<RealVectorValue>;
