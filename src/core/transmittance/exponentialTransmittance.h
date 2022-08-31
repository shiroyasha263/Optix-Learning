#ifndef EXPONENTIALTRANSMITTANCE_h
#define EXPONENTIALTRANSMITTANCE_h

#pragma once

#include <sutil/vec_math.h>
#include <sutil/Preprocessor.h>

#include <math/TangentFrame.h>
#include <math/Angle.h>

#include <sampling/UniformSampler.h>
class exponentialTransmittance
{
private:
    
public:
    SUTIL_INLINE SUTIL_HOSTDEVICE exponentialTransmittance(/* args */) = default;
    SUTIL_INLINE SUTIL_HOSTDEVICE float3 eval(const float3 &tau, bool startOnSurface, bool endOnSurface) const
    {
        if (startOnSurface && endOnSurface)
            return surfaceSurface(tau);
        else if (!startOnSurface && !endOnSurface)
            return mediumMedium(tau);
        else
            return mediumSurface(tau);
    }
    SUTIL_INLINE SUTIL_HOSTDEVICE float sample(UniformSampler *sampler, bool startOnSurface) const
    {
        return startOnSurface ? sampleSurface(sampler) : sampleMedium(sampler);
    }
    SUTIL_INLINE SUTIL_HOSTDEVICE float3 surfaceProbability(const float3 &tau, bool startOnSurface) const
    {
        return startOnSurface ? surfaceSurface(tau) : mediumSurface(tau);
    }
    SUTIL_INLINE SUTIL_HOSTDEVICE float3 mediumPdf(const float3 &tau, bool startOnSurface) const
    {
        return startOnSurface ? surfaceMedium(tau) : mediumMedium(tau);
    }


    SUTIL_INLINE SUTIL_HOSTDEVICE float3 surfaceSurface(const float3 &tau) const;
    SUTIL_INLINE SUTIL_HOSTDEVICE float3 surfaceMedium(const float3 &tau) const;
    SUTIL_INLINE SUTIL_HOSTDEVICE float3 mediumSurface(const float3 &tau) const;
    SUTIL_INLINE SUTIL_HOSTDEVICE float3 mediumMedium(const float3 &tau) const;

    SUTIL_INLINE SUTIL_HOSTDEVICE float sampleSurface(UniformSampler *sampler) const;
    SUTIL_INLINE SUTIL_HOSTDEVICE float sampleMedium(UniformSampler *sampler) const;
};

SUTIL_INLINE SUTIL_HOSTDEVICE float3 exponentialTransmittance::surfaceSurface(const float3 &tau) const
{
    return expf(-tau);
}
SUTIL_INLINE SUTIL_HOSTDEVICE float3 exponentialTransmittance::surfaceMedium(const float3 &tau) const
{
    return expf(-tau);
}
SUTIL_INLINE SUTIL_HOSTDEVICE float3 exponentialTransmittance::mediumSurface(const float3 &tau) const
{
    return expf(-tau);
}
SUTIL_INLINE SUTIL_HOSTDEVICE float3 exponentialTransmittance::mediumMedium(const float3 &tau) const
{
    return expf(-tau);
}

SUTIL_INLINE SUTIL_HOSTDEVICE float exponentialTransmittance::sampleSurface(UniformSampler *sampler) const
{
    return -logf(1.0f - sampler->next1D());
}
SUTIL_INLINE SUTIL_HOSTDEVICE float exponentialTransmittance::sampleMedium(UniformSampler *sampler) const
{
    return -logf(1.0f - sampler->next1D());
}
#endif
