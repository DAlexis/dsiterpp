#include "dsiterpp/error-estimator.hpp"

using namespace dsiterpp;

void ErrorEstimatorBase::set_integrator(IIntegrator* integrator)
{
    m_integrator = integrator;
}

const IntegrationError& ErrorEstimatorBase::get_error()
{
    return m_error;
}
