[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
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
    rate = -1
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
[]

[Outputs]
  exodus = true
  checkpoint = true
[]
