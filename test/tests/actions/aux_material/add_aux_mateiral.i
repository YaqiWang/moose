[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 2
  ny = 2
  nz = 0
  zmin = 0
  zmax = 0
  elem_type = QUAD4
[]

[Variables]
  active = 'u'

  [./u]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  active = 'diff'

  [./diff]
    type = Diffusion
    variable = u
  [../]
[]

[BCs]
  active = 'left right'

  [./left]
    type = DirichletBC
    variable = u
    boundary = 0
    value = 0
  [../]

  [./right]
    type = DirichletBC
    variable = u
    boundary = 2
    value = 1
  [../]
[]

[Materials]
  [./mat]
    type = GenericConstantMaterial
    block = 0
    prop_names = 'prop1 prop2'
    prop_values = '1 2'
  [../]
[]

[AuxMaterials]
  [./auxmat1]
    type = AuxConstantMaterial
    boundary = left
    prop_names = prop3
    prop_values = 1000

    added_material_property = prop1
  [../]
  [./auxmat2]
    type = AuxConstantMaterial
    boundary = left
    prop_names = prop4
    prop_values = 10

    added_material_property = prop1
  [../]
[]

[Executioner]
  type = Steady

  # Preconditioned JFNK (default)
  solve_type = 'PJFNK'
[]

[Outputs]
  file_base = out
  output_initial = true
  exodus = true
  [./console]
    type = Console
    linear_residuals = true
    perf_log = true
  [../]
[]

[Debug]
  show_actions = true
  show_top_residuals = 5
[]
