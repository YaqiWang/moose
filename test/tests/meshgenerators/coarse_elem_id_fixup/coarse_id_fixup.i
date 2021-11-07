[Mesh]
  [fine_mesh]
    type = GeneratedMeshGenerator
    dim = 2
    xmax = 12
    xmin = -12
    nx = 12
    ymax = 12
    ymin = -12
    ny = 12
    subdomain_ids = '
     0 0 0 0 1 1 1 1 2 2 2 2
     0 0 0 1 0 1 1 1 2 2 2 2
     0 0 0 0 1 1 1 1 2 2 2 2
     0 0 0 0 1 1 1 1 2 2 2 2
     3 3 3 3 4 4 4 4 5 5 5 5
     3 3 3 3 4 4 4 4 5 5 5 5
     3 3 3 4 3 4 4 4 5 5 5 5
     3 3 3 3 4 4 4 5 4 5 5 5
     6 6 6 6 7 7 7 7 8 8 10 10
     6 6 6 6 7 7 7 7 8 8 10 10
     6 6 6 6 7 7 7 7 8 8 8 8
     6 6 6 6 7 7 7 7 8 8 8 9
     '
  []
  [copy_id]
    type = SubdomainElementIDs
    input = fine_mesh
    subdomains = '0 1 2 3 4 5 7 6 8 9 10'
    extra_id_names = 'material_id'
    extra_ids = '0 1 2 3 4 5 7 6 8 9 10'
  []
  [fixup]
    type = CoarseElementIDFixup
    input = copy_id
    source_element_id = material_id
    order_ids_with_centroid = true
    verbose = true
  []
[]

[Problem]
  kernel_coverage_check = false
  solve = false
[]

[AuxVariables]
  [material_id]
    family = MONOMIAL
    order = CONSTANT
  []
[]

[AuxKernels]
  [material_id]
    type = ElemExtraIDAux
    variable = material_id
    extra_id_name = material_id
  []
[]

[Executioner]
  type = Steady
[]

[Outputs]
  exodus = true
[]
