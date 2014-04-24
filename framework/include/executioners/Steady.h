/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef STEADY_H
#define STEADY_H

#include "Executioner.h"
#include "InputParameters.h"

// System includes
#include <string>

class Steady;
class FEProblem;

template<>
InputParameters validParams<Steady>();

/**
 * Steady executioners usually only call "solve()" on the NonlinearSystem once.
 */
class Steady: public Executioner
{
public:

  /**
   * Constructor
   *
   * @param name The name given to the Executioner in the input file.
   * @param parameters The parameters object holding data for the class to use.
   * @return Whether or not the solve was successful.
   */
  Steady(const std::string & name, InputParameters parameters);

  virtual ~Steady();

  virtual void init();

  /**
   * This controls the solve.
   */
  virtual void execute();

  /**
   * This will call solve() on the NonlinearSystem with AMR if AMR is turned on.
   */
  virtual void takeStep();

  /**
   * This will output the final solution.
   */
  virtual void postExecute();

  virtual Problem & problem();

  virtual void checkIntegrity();

protected:
  FEProblem & _problem;

#ifdef LIBMESH_ENABLE_AMR
  bool _output_amr_steps;
#endif
  int & _time_step;
  Real & _time;
};

#endif //STEADY_H
