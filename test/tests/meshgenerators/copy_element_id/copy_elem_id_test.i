[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 10
    ny = 10
    xmax = 1
    ymax = 1
    extra_element_integers = test_id
  []

  [subdomains]
    type = SubdomainBoundingBoxGenerator
    input = gmg
    bottom_left = '0 0 0'
    block_id = 1
    top_right = '0.9 0.9 0'
    integer_name = test_id
  []

  [another_subdomains]
    type = SubdomainBoundingBoxGenerator
    input = subdomains
    bottom_left = '0 0 0'
    block_id = 2
    top_right = '0.9 0.9 0'
    location = OUTSIDE
    integer_name = test_id
  []

  [copy_test_id]
    type = CopyElementID
    input = another_subdomains
    source_element_id = test_id
    target_element_ids = 'test_id1 test_id2'
  []
[]

[Problem]
  kernel_coverage_check = false
  solve = false
[]

[AuxVariables]
  [test_id]
    family = MONOMIAL
    order = CONSTANT
  []
  [test_id1]
    family = MONOMIAL
    order = CONSTANT
  []
  [test_id2]
    family = MONOMIAL
    order = CONSTANT
  []
[]

[AuxKernels]
  [test_id]
    type = ElemExtraIDAux
    variable = test_id
    extra_id_name = test_id
  []
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
[]

[Executioner]
  type = Steady
[]

[Outputs]
  exodus = true
[]
