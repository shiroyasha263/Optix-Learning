#ifndef HOMOGENEOUSMEDIUM_H
#define HOMOGENEOUSMEDIUM_H

#pragma once

#include <sutil/vec_math.h>
#include <sutil/Preprocessor.h>

#include <sampleRecords/DirectionSample.h>
#include <sampleRecords/PositionSample.h>
#include <math/TangentFrame.h>
#include <math/Angle.h>
#include <math/Ray.h>

#include <sampling/UniformSampler.h>
#include <transmittance/exponentialTransmittance.h>
#include <phasefunctions/isotropicPhaseFunction.h>
#include <sampleRecords/MediumSample.h>

class HomogeneousMedium
{
private:
    float sigma_a, sigma_s;
    float sigma_t;

public:
    // Transmittance
    exponentialTransmittance *transmittance_function;
    isotropicPhaseFunction *phase_function;
    // Phase function

    SUTIL_INLINE SUTIL_HOSTDEVICE HomogeneousMedium(float sigma_a_, float sigma_s_)
        : sigma_a(sigma_a_), sigma_s(sigma_s_)
    {
        sigma_t = sigma_a_ + sigma_s_;
        transmittance_function = new exponentialTransmittance();
        phase_function = new isotropicPhaseFunction();
    }
    SUTIL_HOSTDEVICE float sigmaT() { return sigma_t; }
    SUTIL_HOSTDEVICE bool sampleDistance(UniformSampler *sampler,
                                         MediumSample &sample,
                                         bool first_scatter) const;
    SUTIL_HOSTDEVICE bool getWeights(Ray &ray,
                                     MediumSample &sample,
                                     bool first_scatter,
                                     bool endOnSurface) const;
    SUTIL_HOSTDEVICE float3 transmittance(const Ray &ray, bool startOnSurface, bool endOnSurface) const;
    SUTIL_HOSTDEVICE float pdf(const Ray &ray, bool startOnSurface, bool endOnSurface) const;
};

SUTIL_HOSTDEVICE bool HomogeneousMedium::sampleDistance(UniformSampler *sampler,
                                                        MediumSample &sample,
                                                        bool first_scatter) const
{
    float t = transmittance_function->sample(sampler, first_scatter) / sigma_t;
    sample.t = t;
    float3 tau = make_float3(sample.t * sigma_t);
    sample.weight = transmittance_function->eval(tau, first_scatter, false);
    sample.pdf = (sigma_t * transmittance_function->mediumPdf(tau, first_scatter)).x;
    sample.weight *= sigma_s;
    sample.weight /= sample.pdf;

    return true;
}

SUTIL_HOSTDEVICE bool HomogeneousMedium::getWeights(Ray &ray,
                                                    MediumSample &sample,
                                                    bool first_scatter,
                                                    bool endOnSurface) const
{
    sample.t = ray.farT;
    float3 tau = make_float3(sample.t * sigma_t);
    sample.weight = transmittance_function->eval(tau, first_scatter, endOnSurface);
    sample.pdf = (sigma_t * transmittance_function->mediumPdf(tau, first_scatter)).x;
    sample.weight *= sigma_s;
    sample.weight /= sample.pdf;

    return true;
}

SUTIL_HOSTDEVICE float3 HomogeneousMedium::transmittance(const Ray &ray, bool startOnSurface,
                                                         bool endOnSurface) const
{
    if (ray.farT == __FLT_MAX__)
        return make_float3(0.0f);
    else
        return transmittance_function->eval(make_float3(sigma_t * ray.farT), startOnSurface, endOnSurface);
}

SUTIL_HOSTDEVICE float HomogeneousMedium::pdf(const Ray &ray, bool startOnSurface, bool endOnSurface) const
{

    float3 tau = make_float3(ray.farT * sigma_t);
    if (endOnSurface)
        return transmittance_function->surfaceProbability(tau, startOnSurface).x;
    else
        return (sigma_t * transmittance_function->mediumPdf(tau, startOnSurface)).x;
}
#endif
