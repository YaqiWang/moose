//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "MooseObject.h"
#include "MeshMetaDataInterface.h"
#include "MooseApp.h"

// Included so mesh generators don't need to include this when constructing MeshBase objects
#include "MooseMesh.h"

#include "libmesh/mesh_base.h"
#include "libmesh/parameters.h"

// Forward declarations
class MeshGenerator;
class MooseMesh;

template <>
InputParameters validParams<MeshGenerator>();

/**
 * MeshGenerators are objects that can modify or add to an existing mesh.
 */
class MeshGenerator : public MooseObject, public MeshMetaDataInterface
{
public:
  /**
   * Constructor
   *
   * @param parameters The parameters object holding data for the class to use.
   */
  static InputParameters validParams();

  MeshGenerator(const InputParameters & parameters);

  /**
   * Generate / modify the mesh
   *
   */
  virtual std::unique_ptr<MeshBase> generate() = 0;

  /**
   * Return the MeshGenerators that must run before this MeshGenerator
   */
  std::vector<std::string> & getDependencies() { return _depends_on; }

protected:
  /**
   * Methods for writing out attributes to the mesh meta-data store, which can be retrieved from
   * most other MOOSE systems and is recoverable.
   */
  template <typename T>
  T & declareMeshProperty(const std::string & data_name);

  template <typename T>
  T & declareMeshProperty(const std::string & data_name, const T & init_value);

  /**
   * Takes the name of a MeshGeneratorName parameter and then gets a pointer to the
   * Mesh that MeshGenerator is going to create.
   *
   * NOTE: You MUST catch this by reference!
   *
   * @return The Mesh generated by that MeshGenerator
   */
  std::unique_ptr<MeshBase> & getMesh(const std::string & input_mesh_generator_parameter_name);

  /**
   * Takes the name of another MeshGenerator directly.
   *
   * NOTE: You MUST catch this by reference!
   *
   * @return The Mesh generated by that MeshGenerator
   */
  std::unique_ptr<MeshBase> &
  getMeshByName(const MeshGeneratorName & input_mesh_generator_parameter_name);

  /*
   * Whether or not this mesh generator is the final.
   * Note: Final mesh generator owns the final mesh base in type of libMesh::MeshBase.
   *       This function should only be called in generate function.
   */
  bool isFinal() const { return _app.finalMeshGeneratorName() == _name; }

  /// References to the mesh and displaced mesh (currently in the ActionWarehouse)
  std::shared_ptr<MooseMesh> & _mesh;

private:
  /// A list of generators that are required to run before this generator may run
  std::vector<std::string> _depends_on;

  /// A nullptr to use for when inputs aren't specified
  std::unique_ptr<MeshBase> _null_mesh = nullptr;
};

template <typename T>
T &
MeshGenerator::declareMeshProperty(const std::string & data_name)
{
  std::string full_name =
      std::string(MeshMetaDataInterface::SYSTEM) + "/" + name() + "/" + data_name;

  // Here we will create the RestartableData even though we may not use this instance.
  // If it's already in use, the App will return a reference to the existing instance and we'll
  // return that one instead. We might refactor this to have the app create the RestartableData
  // at a later date.
  auto data_ptr =
      libmesh_make_unique<RestartableData<T>>(full_name, _app.actionWarehouse().mesh().get());
  auto & restartable_data_ref = static_cast<RestartableData<T> &>(
      _app.registerRestartableData(full_name, std::move(data_ptr), 0, true, false));

  return restartable_data_ref.get();
}

template <typename T>
T &
MeshGenerator::declareMeshProperty(const std::string & data_name, const T & init_value)
{
  T & data = declareMeshProperty<T>(data_name);
  data = init_value;

  return data;
}
