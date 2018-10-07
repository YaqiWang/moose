[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 2
  ny = 2
  xmin = 0
  xmax = 1
  ymin = 0
  ymax = 1
[]

[Variables]
  [./u]
    initial_condition = 0.367879441171442
  [../]
[]

[Kernels]
  [./time]
    type = CoefTimeDerivative
    variable = u
    mat_prop_coef = time_prop
    prop_in_derivative = false
  [../]
  [./reaction]
    type = CoefReaction
    variable = u
    mat_prop_coef = time_prop
  [../]
  [./reaction2]
    type = CoefReaction
    variable = u
  [../]
[]

[Materials]
  [./time_prop]
    type = GenericFunctionMaterial
    prop_names = time_prop
    prop_values = time_function
  [../]
[]

[Functions]
  [./time_function]
    type = ParsedFunction
    value = 't'
  [../]
[]

[Postprocessors]
  [./uint]
    type = ElementIntegralVariablePostprocessor
    variable = u
    execute_on = 'initial timestep_end'
  [../]
[]

[Executioner]
  type = Transient
  scheme = crank-nicolson
  start_time = 1
  end_time = 2
  dt = 0.0003125
  timestep_tolerance = 1e-10
  nl_abs_tol = 1e-12
[]

[Outputs]
  exodus = true
  csv = true
[]
