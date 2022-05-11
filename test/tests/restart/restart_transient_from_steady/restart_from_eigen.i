[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
[]

[Problem]
  restart_file_base = eigen_out_cp/LATEST
  skip_additional_restart_data = true
[]

[Variables]
  [u]
    order = FIRST
    family = LAGRANGE
  []
[]

[Kernels]
  [diff]
    type = Diffusion
    variable = u
  []
  [rhs]
    type = Reaction
    variable = u
    #
    # ideally to have the following parameter to replace extra_vector_tags
    # so that the kernel will multiply the eigenvalue for its residual contribution.
    # Because of this, this input should sustain the null transient.
    #
    # scale_with_eigenvalue = true
    #
    # I am currently hard-coding the eigenvalue in 'rate' parameter to see the null transient.
    # rate = -1
    rate = -9.9510429776
  []

  [ie]
    type = TimeDerivative
    variable = u
  []
[]

[BCs]
  [homogeneous]
    type = DirichletBC
    variable = u
    boundary = '0 2'
    value = 0
  []
[]

[Postprocessors]
  [unorm]
    type = ElementL2Norm
    variable = u
  []
  [eigenvalue]
    type = Receiver
  []
[]

[Executioner]
  type = Transient
  start_time = 0.0
  num_steps = 5
  dt = .1
  nl_abs_tol = 1e-10
[]

[Outputs]
  exodus = true
[]
