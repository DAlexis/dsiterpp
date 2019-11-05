#include "exponent-time-iterable.hpp"
#include "include/euler-explicit.hpp"
#include "include/runge-error-estimator.hpp"
#include <cmath>

#include "gtest/gtest.h"

TEST(AutoStepAdj, EulerBasedTest)
{
    EulerExplicitIterator euler;
    RungeErrorEstimator estimator(1);

    ExponentProblem exp_problem(&euler, 1.0);
    exp_problem.time_iterator.set_error_estimator(&estimator);
    exp_problem.time_iterator.step_adj_pars().autoStepAdjustment = true;
    exp_problem.time_iterator.step_adj_pars().rel_error_per_step_refining_treshold = 0.01;
    exp_problem.time_iterator.step_adj_pars().rel_error_per_step_coarsening_treshold = 0.0001;

    double time_limit = 3.0;
    exp_problem.iterate(time_limit);

    ASSERT_NEAR(exp(time_limit), exp_problem.value(), 0.01);
}
