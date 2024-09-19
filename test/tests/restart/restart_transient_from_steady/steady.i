[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
[]

[Problem]
  type = CheckKernelResidualProblem
  evaluate_residual_norm_for_kernels = diff
[]

[Variables]
  [./u]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  [./diff]
    type = Diffusion
    variable = u
  [../]
[]

[BCs]
  [./left]
    type = DirichletBC
    variable = u
    boundary = 'left'
    value = 0
  [../]

  [./right]
    type = DirichletBC
    variable = u
    boundary = 'right'
    value = 1
  [../]
[]

[Postprocessors]
  [./unorm]
    type = ElementL2Norm
    variable = u
  [../]
[]

[Executioner]
  type = Steady
[]

[Outputs]
  exodus = true
  checkpoint = true
[]
