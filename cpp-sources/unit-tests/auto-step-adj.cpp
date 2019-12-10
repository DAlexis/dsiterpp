#include "exponent-time-iterable.hpp"
#include "dsiterpp/euler-explicit.hpp"
#include "dsiterpp/runge-error-estimator.hpp"
#include <cmath>

#include "gtest/gtest.h"

using namespace dsiterpp;

TEST(AutoStepAdj, EulerBasedTest)
{
    double time_limit = 3.0;
    double allowed_result_relative_error = 0.01;
    EulerExplicitIterator euler;
    RungeErrorEstimator estimator;

    ExponentProblem exp_problem(&euler, 1.0);
    exp_problem.time_iterator.set_error_estimator(&estimator);
    exp_problem.time_iterator.step_adj_pars().autoStepAdjustment = true;

    exp_problem.time_iterator.step_adj_pars().setup_relative_deconvergence_speed(allowed_result_relative_error / time_limit);

    ASSERT_TRUE(exp_problem.time_iterator.step_adj_pars().is_self_consistent());

    exp_problem.iterate(time_limit);

    double ground_thruth = exp(time_limit);

    ASSERT_NEAR(ground_thruth, exp_problem.value(), allowed_result_relative_error * ground_thruth);
}
