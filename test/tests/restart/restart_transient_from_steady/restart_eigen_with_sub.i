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

[Problem]
  restart_file_base = eigen_with_sub_out_cp/LATEST
  skip_additional_restart_data = true
[]

[Variables]
  [u]
    order = FIRST
    family = LAGRANGE
  []
[]

[AuxVariables]
  [T]
    order = FIRST
    family = LAGRANGE
  []
  [power]
    order = FIRST
    family = LAGRANGE
  []
[]

[Kernels]
  [time]
    type = TimeDerivative
    variable = u
  []

  [diff]
    type = DiffMKernel
    variable = u
    mat_prop = diffusion
    offset = 0.0
  []

  [rhs]
    type = CoefReaction
    variable = u
    #
    # ideally to have the following parameter to replace extra_vector_tags
    # so that the kernel will multiply the eigenvalue for its residual contribution.
    # Because of this, this input should sustain the null transient.
    #
    # scale_with_eigenvalue = true
    #
    # hardcoding this value to see the null transient
    coefficient = -0.2767622728 # -1.0
  []
[]

[AuxKernels]
  [power_ak]
    type = NormalizationAux
    variable = power
    source_variable = u
    normalization = unorm
    normal_factor = 10
    execute_on = 'initial timestep_end'
  []
[]

[BCs]
  [homogeneous]
    type = DirichletBC
    variable = u
    boundary = '0 1 2 3'
    value = 0
  []
[]

[Materials]
  [dc]
    type = VarCouplingMaterial
    var = T
    block = 0
    base = 1.0
    coef = 1.0
  []
[]

[Executioner]
  type = Transient
  solve_type = PJFNK
  nl_abs_tol = 1e-8
  nl_rel_tol = 1e-6
  fixed_point_max_its = 10
  fixed_point_rel_tol = 1e-6

  start_time = 0
  end_time = 3
  dt = 1.0
[]

[Postprocessors]
  [unorm]
    type = ElementIntegralVariablePostprocessor
    variable = u
    execute_on = linear
  []
  [eigenvalue]
    type = Receiver
  []
[]

[Outputs]
  csv = true
  exodus =true
  checkpoint = true
  execute_on = 'timestep_end'
[]

[MultiApps]
  [sub]
    type = TransientMultiApp
    input_files = eigen_with_sub_sub.i
    execute_on = timestep_end
  []
[]

[Transfers]
  [T_from_sub]
    type = MultiAppShapeEvaluationTransfer
    from_multi_app = sub
    source_variable = T
    variable = T
  []
  [power_to_sub]
    type = MultiAppShapeEvaluationTransfer
    to_multi_app = sub
    source_variable = power
    variable = power
  []
[]
