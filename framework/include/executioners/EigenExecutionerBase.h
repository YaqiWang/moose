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

#ifndef EIGENEXECUTIONERBASE_H
#define EIGENEXECUTIONERBASE_H

#include "Executioner.h"
#include "FEProblem.h"

// Forward Declarations
class EigenExecutionerBase;

template<>
InputParameters validParams<EigenExecutionerBase>();

/**
 * This class provides reusable routines for eigenvalue executioners.
 */
class EigenExecutionerBase: public Executioner
{
public:

  /**
   * Constructor
   *
   * @param name The name given to the Executioner in the input file.
   * @param parameters The parameters object holding data for the class to use.
   * @return Whether or not the solve was successful.
   */
  EigenExecutionerBase(const std::string & name, InputParameters parameters);

  virtual ~EigenExecutionerBase();

  virtual Problem & problem() { return _problem; }

  /**
   * Initialization
   */
  virtual void init();

  /**
   * Make sure time kernel is not presented
   */
  virtual void checkIntegrity();

  /**
   * Add postprocessors to report problem parameters including the eigenvalue
   */
  virtual void addRealParameterReporter(const std::string & param_name);

  /**
   * Perform inverse power iterations with the initial guess of the solution
   *
   * @param min_iter The minimum number of power iterations.
   * @param max_iter The maximum number of power iterations.
   * @param pfactor The factor on reducing the residual norm of each power iteration.
   * @param cheb_on To turn the Chebyshev acceleration on.
   * @param tol_eig Tolerance on the difference of the eigenvalue of two successive iterations.
   * @param tol_x Tolerance on the difference of the solution norm of two successive iterations.
   * @param echo True to make screen printouts.
   * @param output_convergence True to call MOOSE output system to output iteration history.
   * @param time_base Used to set time for MOOSE output system.
   * @param k Eigenvalue, input as the initial guess.
   * @param initial_res The initial residual.
   */
  virtual void inversePowerIteration(unsigned int min_iter, unsigned int max_iter, Real pfactor,
                                     bool cheb_on, Real tol_eig, Real tol_x, bool echo,
                                     bool output_convergence, Real time_base,
                                     Real & k, Real & initial_res);

  /**
   * Override this for actions that should take place before linear solve of each inverse power iteration
   */
  virtual void preStep();

  /**
   * Override this for actions that should take place after linear solve of each inverse power iteration
   */
  virtual void postStep();

  /**
   * Normalize the solution vector based on the postprocessor value for normalization
   *
   * @param force Force the re-evaluation of the postprocessor for normalization.
   * Returns the scaling factor just applied.
   */
  virtual Real normalizeSolution(bool force=true);

  /**
   * Perform nonlinear solve with the initial guess of the solution
   *
   * @param rel_tol Relative tolerance on system residual.
   * @param abs_tol Absolute tolerance on system residual.
   * @param pfactor The factor on reducing the residual norm of each linear iteration.
   * @param k Eigenvalue, input as the initial guess.
   */
  virtual void nonlinearSolve(Real rel_tol, Real abs_tol, Real pfactor, Real & k);

  /**
   * Perform nonlinear solve with the initial guess from several free power iterations
   *
   * @param rel_tol Relative tolerance on system residual.
   * @param abs_tol Absolute tolerance on system residual.
   * @param pfactor The factor on reducing the residual norm of each linear iteration.
   * @param fpi Number of free power iteration
   * @param k Eigenvalue, input as the initial guess.
   */
  virtual void combinedSolve(Real rel_tol, Real abs_tol, Real pfactor, unsigned int fpi, Real & k);

  /**
   * Get differentiation of eigenvalue with respect a parameter at a value
   *
   * @param p Value of the parameter.
   * @param free_iter Number of free power iterations on solving at p.
   * @param abs_tol How tightly the solves need to be done.
   * @param pfactor Linear relative tolerance
   * @param output True to ouput solutions at p with MOOSE output system.
   * @param tp Time used for outputting.
   * @param ev Eigenvalue at p.
   * Returns de/dp at p.
   */
  virtual Real eigenvalueCoefficient(Real p, unsigned int free_iter, Real abs_tol, Real pfactor,
                                     bool output, Real tp, Real & ev);

  /**
   * Set the system prarameter for evaluating the differentiation of eigenvalue
   */
  virtual void setSystemParameter(Real p);

protected:

  /**
   * Print eigenvalue
   */
  virtual void printEigenvalue();

  // the fe problem
  FEProblem & _problem;

  // eigenvalue
  Real & _eigenvalue;

  // postprocessor for eigenvalue
  const Real & _source_integral;
  const Real & _source_integral_old;
  ExecFlagType _bx_execflag;

  // postprocessor for evaluating |x-xprevious|
  Real * _solution_diff;
  ExecFlagType _xdiff_execflag;

  // postprocessor for normalization
  Real & _normalization;
  ExecFlagType _norm_execflag;

  // to control custom userobject evaluation
  const bool _run_custom_uo;

  // Chebyshev acceleration
  class Chebyshev_Parameters
  {
  public:
    Chebyshev_Parameters ();
    void reinit ();

    const unsigned int n_iter;    // minimum number of accelerated iteration each cycle
    const unsigned int fsmooth;   // number of unaccelerated iteration each cycle
    unsigned int finit;           // number of unaccelerated iteration before Chebyshev

    unsigned int lgac;            // doing acceleration or not
    unsigned int icheb;           // number of acceleration in current cycle
    unsigned int iter_begin;      // starting number of current acceleration cycle
    double error_begin;           // starting flux error of current acceleration cycle
    double flux_error_norm_old;   // flux error of previous power iteration
    double ratio;                 // estimation of dominant ratio
    double ratio_new;             // new estimated dominant ratio
    unsigned int icho;            // improved ratio estimation
  };
  Chebyshev_Parameters  chebyshev_parameters;
  void chebyshev(unsigned int iter);

  // DoF indices of the eigen system, empty if the eigen system is the entire system
  std::set<dof_id_type> _var_indices;

  // save old flux so that it wont be accidentally changed
  // FIXME: can be removed in the future when STEP is available.
  NumericVector<Real> * _sys_sol_old;
  NumericVector<Real> * _sys_sol_older;
  NumericVector<Real> * _aux_sol_old;
  NumericVector<Real> * _aux_sol_older;
};

#endif //EIGENEXECUTIONERBASE_H
