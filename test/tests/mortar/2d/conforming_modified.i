[Mesh]
 file = mesh_in.e

 [./MortarInterfaces]
   [./middle]
     master = interface-diff0-to-diff1
     slave = interface-diff1-to-diff0
#     subdomain = 3
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
  [./u1]
    order = FIRST
    family = LAGRANGE
    block = '1'
  [../]
  [./u2]
    order = FIRST
    family = LAGRANGE
    block = '2'
  [../]

  [./lm]
    order = FIRST
    family = LAGRANGE

####Note: this is the problematic line
#    block = interface_diff0_diff1
    block = middle
  [../]
[]

[Kernels]
  [./diff1]
    type = Diffusion
    variable = u1
    block = '1'
  [../]
  [./ffn1]
    type = UserForcingFunction
    variable = u1
    function = ffn
    block = '1'
  [../]
  [./diff2]
    type = Diffusion
    variable = u2
    block = '2'
  [../]
  [./ffn2]
    type = UserForcingFunction
    variable = u2
    function = ffn
    block = '2'
  [../]
[]

[Constraints]
  [./ced]
    type = EqualValueConstraint
    variable = lm
# here we are using the name of the mortar face
    interface = middle
    master_variable = u1
    slave_variable = u2
  [../]
[]

[BCs]
  [./all1]
    type = FunctionDirichletBC
    variable = u1
    boundary = 'bottom left_diff0_bnd right_diff0_bnd'
    function = exact_sln
  [../]
  [./all2]
    type = FunctionDirichletBC
    variable = u2
    boundary = 'top left_diff1_bnd right_diff1_bnd'
    function = exact_sln
  [../]

# Those two bcs are commented out. They should not be needed.
#  [./mortar1]
#    type = DiffusionFluxBC
#    variable = u1
#    boundary = 'interface-diff0-to-diff1'
#  [../]
#  [./mortar2]
#    type = DiffusionFluxBC
#    variable = u2
#    boundary = 'interface-diff1-to-diff0'
#  [../]
[]

[Postprocessors]
  [./l2_error1]
    type = ElementL2Error
    variable = u1
    function = exact_sln
    block = '1'
  [../]
  [./l2_error2]
    type = ElementL2Error
    variable = u2
    function = exact_sln
    block = '2'
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
  nl_rel_tol = 1e-11
  l_tol = 1e-10
[]

[Outputs]
  output_initial = true
  exodus = true
  [./console]
    type = Console
    linear_residuals = true
    perf_log = true
  [../]
[]
