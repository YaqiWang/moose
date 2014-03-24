[Mesh]
  file = square.e
[]

[Variables]
  [./u]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[AuxVariables]
  [./one]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./up2]
    order = FIRST
    family = LAGRANGE
  [../]
  [./three]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./coupled_nine]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./coupled_fifteen]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./coupled]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./high_order]
    order = NINTH
    family = MONOMIAL
  [../]
  [./high_order_plus_2]
    order = NINTH
    family = MONOMIAL
  [../]
  [./u_l2]
    order = FIRST
    family = L2_LAGRANGE
  [../]
  [./u_l2_m2]
    order = FIRST
    family = L2_LAGRANGE
  [../]
  [./another_u_l2_m2]
    order = FIRST
    family = L2_LAGRANGE
  [../]
[]

[Kernels]
  # Coupling of nonlinear to Aux
  [./diff]
    type = Diffusion
    variable = u
  [../]
  [./force]
    type = CoupledForce
    variable = u
    v = one
  [../]
[]

[AuxKernels]
  # Simple Aux Kernel
  # Shows coupling of Element to Nodal
  # Shows coupling of Element to non-linear
  # Shows coupling of Element to non-linear grad
  [./constant]
    variable = one
    type = ConstantAux
    value = 1
  [../]
  [./coupled_nine]
    variable = coupled_nine
    type = DirectCoupledAux
    value = 3
    operator = *
    coupled = three
  [../]
  [./coupled_three]
    variable = three
    type = DirectCoupledAux
    value = 2
    operator = +
    coupled = one
  [../]
  [./coupled_fifteen]
    variable = coupled_fifteen
    type = DirectCoupledAux
    value = 5
    operator = *
    coupled = three
  [../]
  [./coupled]
    variable = coupled
    type = CoupledAux
    value = 2
    coupled = up2
  [../]
  [./u_plus_2]
    variable = up2
    type = DirectCoupledAux
    value = 2
    coupled = u
  [../]

  [./coupled_high_order]
    variable = high_order
    type = CoupledAux
    value = 2
    operator = +
    coupled = u
  [../]
  [./coupled_high_order2]
    variable = high_order_plus_2
    type = DirectCoupledAux
    value = 2
    operator = +
    coupled = high_order
  [../]

  [./coupled_lagrange]
    variable = u_l2
    type = CoupledAux
    value = 2
    operator = +
    coupled = u
  [../]
  [./coupled_lagrange2]
    variable = u_l2_m2
    type = DirectCoupledAux
    value = 2
    operator = *
    coupled = u_l2
  [../]
  [./another_coupled_lagrange2]
    variable = another_u_l2_m2
    type = CoupledAux
    value = 2
    operator = *
    coupled = u_l2
  [../]
[]

[BCs]
  [./left]
    type = DirichletBC
    variable = u
    boundary = 1
    value = 0
  [../]
  [./right]
    type = DirichletBC
    variable = u
    boundary = 2
    value = 1
  [../]
[]

[Executioner]
  type = Steady

  # Preconditioned JFNK (default)
  solve_type = 'PJFNK'
[]

[Outputs]
  file_base = out_direct
  output_initial = true
  interval = 1
  [./exodus]
    type = Exodus
    elemental_as_nodal = true
  [../]
  [./console]
    type = Console
    perf_log = true
  [../]
[]
