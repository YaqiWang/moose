[Mesh]
  [fmg]
    type = FileMeshGenerator
    file = openmc_bc_in.e
  []
[]

[Functions]
  [bmesh]
    type = BoundaryMeshFunction
    boundary = interface
  []
[]

[Problem]
  solve = false
  kernel_coverage_check = false
[]

[Executioner]
  type = Steady
[]
