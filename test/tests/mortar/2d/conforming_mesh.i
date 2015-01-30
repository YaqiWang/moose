[Mesh]
  file = mesh_in2.e

  [./MortarInterfaces]
    [./middle]
      master = interface-diff0-to-diff1
      slave = interface-diff1-to-diff0
      subdomain = interface_diff0_diff1
    [../]
  [../]
[]

[Functions]
  [./exact_sln]
    type = ParsedFunction
    value = y
  [../]
  [./ffn]
    type = ParsedFunction
    value = 0
  [../]
[]

[Variables]
  [./u]
    order = FIRST
    family = LAGRANGE
    block = '1 2'
  [../]
  [./lm]
    order = FIRST
    family = LAGRANGE
    block = middle
  [../]
[]

[Kernels]
  [./diff]
    type = Diffusion
    variable = u
  [../]
  [./ffn]
    type = UserForcingFunction
    variable = u
    function = ffn
  [../]
[]

[Constraints]
  [./ced]
    type = EqualValueConstraint
    variable = lm
    interface = middle
    master_variable = u
  [../]
[]

[BCs]
  [./all]
    type = FunctionDirichletBC
    variable = u
    boundary = 'top bottom right left'
    function = exact_sln
  [../]
[]

[Postprocessors]
  [./l2_error]
    type = ElementL2Error
    variable = u
    function = exact_sln
    block = '1 2'
  [../]
[]

[Preconditioning]
  [./fmp]
    type = SMP
    full = true
    solve_type = 'NEWTON'
  [../]
[]

[Executioner]
  type = Steady
  nl_rel_tol = 1e-10
  l_tol = 1e-6
[]

[Outputs]
  output_initial = true
  exodus = true
  print_perf_log = true
  print_linear_residuals = true
[]
