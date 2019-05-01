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
#include "TaggingInterface.h"

#include "libmesh/dense_vector.h"
#include "libmesh/dense_matrix.h"

// Forward declarations
class MooseObject;
class Assembly;

template <typename T>
class TaggingAssemblyInterface
{
public:
  TaggingAssemblyInterface(const MooseObject * moose_object);
  virtual ~TaggingAssemblyInterface();

  /**
   * Prepare data for computing element residual the according to active tags.
   * Residual blocks for different tags will be extracted from Assembly.
   * A local residual will be zeroed. It should be called
   * right before the local element vector is computed.
   */
  void prepareVectorTag(Assembly & assembly, unsigned int ivar);

  /**
   * Prepare data for computing element residual the according to active tags
   * for DG and interface kernels.
   * Residual blocks for different tags will be extracted from Assembly.
   * A local residual will be zeroed. It should be called
   * right before the local element vector is computed.
   */
  void prepareVectorTagNeighbor(Assembly & assembly, unsigned int ivar);

  /**
   * Prepare data for computing element jacobian according to the ative tags.
   * Jacobian blocks for different tags will be extracted from Assembly.
   * A local Jacobian will be zeroed. It should be called
   * right before the local element matrix is computed.
   */
  void prepareMatrixTag(Assembly & assembly, unsigned int ivar, unsigned int jvar);

  /**
   * Prepare data for computing element jacobian according to the ative tags
   * for DG and interface kernels.
   * Jacobian blocks for different tags will be extracted from Assembly.
   * A local Jacobian will be zeroed. It should be called
   * right before the local element matrix is computed.
   */
  void prepareMatrixTagNeighbor(Assembly & assembly,
                                unsigned int ivar,
                                unsigned int jvar,
                                Moose::DGJacobianType type);

  /**
   * Local residual blocks  will be appended by adding the current local kernel residual.
   * It should be called after the local element vector has been computed.
   */
  void accumulateTaggedLocalResidual();

  /**
   * Local residual blocks will assigned as the current local kernel residual.
   * It should be called after the local element vector has been computed.
   */
  void assignTaggedLocalResidual();

  /**
   * Local Jacobian blocks  will be appended by adding the current local kernel Jacobian.
   * It should be called after the local element matrix has been computed.
   */
  void accumulateTaggedLocalMatrix();

  /**
   * Local Jacobian blocks will assigned as the current local kernel Jacobian.
   * It should be called after the local element matrix has been computed.
   */
  void assignTaggedLocalMatrix();

private:
  /// Moose objct this tag works on
  const TaggingInterface & _tag_interface;

  /// The vectors this Kernel will contribute to
  const std::set<TagID> & _vector_tags_ref;

  /// The matrices this Kernel will contribute to
  const std::set<TagID> & _matrix_tags_ref;

protected:
  typedef typename OutputTools<T>::OutputData DataType;

  /// Residual blocks Vectors For each Tag
  std::vector<DenseVector<DataType> *> _re_blocks;

  /// Kernel blocks Vectors For each Tag
  std::vector<DenseMatrix<Number> *> _ke_blocks;

  /// Holds residual entries as they are accumulated by this Kernel
  DenseVector<DataType> _local_re;

  /// Holds jacobian entries as they are accumulated by this Kernel
  DenseMatrix<Number> _local_ke;
};
