#ifndef ISOTROPICPHASEFUNCTION_H
#define ISOTROPICPHASEFUNCTION_H

#pragma once

#include <sutil/vec_math.h>
#include <sutil/Preprocessor.h>

#include <sampleRecords/DirectionSample.h>
#include <sampleRecords/PositionSample.h>
#include <math/TangentFrame.h>
#include <math/Angle.h>

#include <sampling/UniformSampler.h>
#include <sampleRecords/PhaseSample.h>


static SUTIL_INLINE SUTIL_HOSTDEVICE float3 uniformSphere(const float2 &xi)
{
    float phi = xi.x*TWO_PI;
    float z = xi.y*2.0f - 1.0f;
    float r = sqrtf(fmaxf(1.0f - z*z, 0.0f));

    return make_float3(
        cosf(phi)*r,
        sinf(phi)*r,
        z
    );
}

static SUTIL_INLINE SUTIL_HOSTDEVICE float uniformSpherePdf()
{
    return INV_FOUR_PI;
}

class isotropicPhaseFunction
{
private:
    /* data */
public:
    SUTIL_INLINE SUTIL_HOSTDEVICE isotropicPhaseFunction(/* args */) = default;
    SUTIL_INLINE SUTIL_HOSTDEVICE float3 eval() const;
    SUTIL_INLINE SUTIL_HOSTDEVICE bool sample(UniformSampler *sampler, PhaseSample &sample) const;
    SUTIL_INLINE SUTIL_HOSTDEVICE float pdf() const;
};

SUTIL_INLINE SUTIL_HOSTDEVICE float3 isotropicPhaseFunction::eval() const
{
    return make_float3(INV_FOUR_PI);
}

SUTIL_INLINE SUTIL_HOSTDEVICE bool isotropicPhaseFunction::sample(UniformSampler *sampler, PhaseSample &sample) const
{
    sample.dir = uniformSphere(sampler->next2D());
    sample.weight = make_float3(1.0f);
    sample.pdf = uniformSpherePdf();
    return true;
}

SUTIL_INLINE SUTIL_HOSTDEVICE float isotropicPhaseFunction::pdf() const
{
    return uniformSpherePdf();
}

#endif
