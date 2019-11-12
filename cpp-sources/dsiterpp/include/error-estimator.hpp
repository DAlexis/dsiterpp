#ifndef EEROR_ESTIMATOR_HPP_INCLUDED
#define EEROR_ESTIMATOR_HPP_INCLUDED

#include <limits>

class IIntegrator;

struct IntegrationError
{
    double max_abs_error = 0.0;
    double max_rel_error = 0.0;
};

class IErrorEstimator
{
public:
    virtual ~IErrorEstimator() {}

    virtual void set_integrator(IIntegrator* integrator) = 0;

    /**
     * Calculate delta at point t with step dt and estimate errors
     */
    virtual void calculate_delta_and_estimate(double t, double dt) = 0;

    virtual const IntegrationError& get_error() = 0;
};

class ErrorEstimatorBase : public IErrorEstimator
{
public:
    void set_integrator(IIntegrator* integrator) override;
    const IntegrationError& get_error() override;

protected:
    IIntegrator* m_integrator = nullptr;
    IntegrationError m_error;
};

#endif // EEROR_ESTIMATOR_HPP_INCLUDED
