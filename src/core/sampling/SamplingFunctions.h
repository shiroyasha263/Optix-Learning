#ifndef SAMPLINGFUNCTIONS_H
#define SAMPLINGFUNCTIONS_H

#pragma once

#include <sutil/vec_math.h>
#include <sutil/Preprocessor.h>

#include <math/Angle.h>

static SUTIL_INLINE SUTIL_HOSTDEVICE float3 cosineHemisphere(const float2 &xi)
{
    float phi = xi.x * TWO_PI;
    float r = sqrtf(xi.y);

    return make_float3(
        cosf(phi) * r,
        sinf(phi) * r,
        sqrtf(fmaxf(1.0f - xi.y, 0.0f)));
}

static SUTIL_INLINE SUTIL_HOSTDEVICE float cosineHemispherePdf(const float3 &p)
{
    return fabs(p.z) * INV_PI;
}

static SUTIL_INLINE SUTIL_HOSTDEVICE float dielectricReflectance(float eta, float cosThetaI, float &cosThetaT)
{
    if (cosThetaI < 0.0f)
    {
        eta = 1.0f / eta;
        cosThetaI = -cosThetaI;
    }
    float sinThetaTSq = eta * eta * (1.0f - cosThetaI * cosThetaI);
    if (sinThetaTSq > 1.0f)
    {
        cosThetaT = 0.0f;
        return 1.0f;
    }
    cosThetaT = sqrtf(fmaxf(1.0f - sinThetaTSq, 0.0f));

    float Rs = (eta * cosThetaI - cosThetaT) / (eta * cosThetaI + cosThetaT);
    float Rp = (eta * cosThetaT - cosThetaI) / (eta * cosThetaT + cosThetaI);

    return (Rs * Rs + Rp * Rp) * 0.5f;
}

static inline bool checkReflectionConstraint(const float3 &in_dir, const float3 &out_dir)
{
    // Comparing with dirac acceptatnce threshold
    return fabsf(in_dir.z * out_dir.z - in_dir.x * out_dir.x - in_dir.y * out_dir.y - 1.0f) < 1e-3f;
}

static inline bool checkRefractionConstraint(const float3 &in_dir, const float3 &out_dir, float eta, float cosThetaT)
{
    float dotP = -in_dir.x * out_dir.x * eta - in_dir.y * out_dir.y * eta - copysignf(cosThetaT, in_dir.z) * out_dir.z;
    // Comparing with dirac acceptatnce threshold
    return fabsf(dotP - 1.0f) < 1e-3f;
}
#endif