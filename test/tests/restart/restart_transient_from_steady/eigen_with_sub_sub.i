[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 2
    xmin = 0
    xmax = 10
    ymin = 0
    ymax = 10
    nx = 8
    ny = 8
  []
[]

[Variables]
  [T]
    order = FIRST
    family = LAGRANGE
  []
[]

[AuxVariables]
  [power]
    order = FIRST
    family = LAGRANGE
  []
[]

[Kernels]
  [time]
    type = TimeDerivative
    variable = T
  []
  [diff_T]
    type = Diffusion
    variable = T
  []
  [src_T]
    type = CoupledForce
    variable = T
    v = power
  []
[]

[BCs]
  [homogeneousT]
    type = DirichletBC
    variable = T
    boundary = '0 1 2 3'
    value = 0
  []
[]

[Postprocessors]
  [tnorm]
    type = ElementL2Norm
    variable = T
  []
[]

[Executioner]
  type = Transient
  start_time = 0
  end_time = 100
  dt = 1.0
  nl_abs_tol = 1e-12
  nl_rel_tol = 1e-8
  steady_state_detection = true
  steady_state_tolerance = 1e-8
[]
