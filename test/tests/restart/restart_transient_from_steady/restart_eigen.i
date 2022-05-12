[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
[]

[Problem]
  restart_file_base = eigen_out_cp/LATEST
[]

[Variables]
  [u]
    order = FIRST
    family = LAGRANGE
  []
[]

[Kernels]
  [diff]
    type = Diffusion
    variable = u
  []
  [rhs]
    type = Reaction
    extra_vector_tags = 'eigen'
    variable = u
    # rate = -1
    rate = -9.9510429776
  []
[]

[BCs]
  [homogeneous]
    type = DirichletBC
    variable = u
    boundary = '0 2'
    value = 0
  []
  [eigen]
    type = EigenDirichletBC
    variable = u
    boundary = '0 2'
  []
[]

[Postprocessors]
  [unorm]
    type = ElementL2Norm
    variable = u
  []
[]

[VectorPostprocessors]
  [eigenvalues]
    type = Eigenvalues
  []
[]

[Executioner]
  type = Eigenvalue
  nl_abs_tol = 1e-10
[]

[Outputs]
  exodus = true
  checkpoint = true
[]
