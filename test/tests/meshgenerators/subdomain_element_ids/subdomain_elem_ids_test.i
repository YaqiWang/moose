[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 10
    ny = 10
    xmax = 1
    ymax = 1
    subdomain_ids = '
      0 0 0 0 0 0 0 0 0 0
      0 0 0 0 0 0 0 0 0 0
      1 1 1 1 1 1 1 1 1 1
      1 1 1 1 1 1 1 1 1 1
      2 2 2 2 2 2 2 2 2 2
      2 2 2 2 2 2 2 2 2 2
      3 3 3 3 3 3 3 3 3 3
      3 3 3 3 3 3 3 3 3 3
      4 4 4 4 4 4 4 4 4 4
      4 4 4 4 4 4 4 4 4 4'
  []

  [subdomain_ids]
    type = SubdomainElementIDs
    input = gmg
    subdomains = '0 1 2 3'
    extra_id_names = 'test_id1 test_id2 test_id3'
    extra_ids = '
                  1 3 5 4;
                  2 9 10 11;
                  0 8 1 2
                '
    default_extra_ids = '-1 0 0'
  []
[]

[Problem]
  kernel_coverage_check = false
  solve = false
[]

[AuxVariables]
  [test_id1]
    family = MONOMIAL
    order = CONSTANT
  []
  [test_id2]
    family = MONOMIAL
    order = CONSTANT
  []
  [test_id3]
    family = MONOMIAL
    order = CONSTANT
  []
[]

[AuxKernels]
  [test_id1]
    type = ElemExtraIDAux
    variable = test_id1
    extra_id_name = test_id1
  []
  [test_id2]
    type = ElemExtraIDAux
    variable = test_id2
    extra_id_name = test_id2
  []
  [test_id3]
    type = ElemExtraIDAux
    variable = test_id3
    extra_id_name = test_id3
  []
[]

[Executioner]
  type = Steady
[]

[Outputs]
  exodus = true
[]
