#ifndef LAMBERTBSDF_H
#define LAMBERTBSDF_H
#pragma once

#include <sampling/SamplingFunctions.h>

#include <math/Angle.h>
#include <sampleRecords/SurfaceScatterEvent.h>
#include <sampling/UniformSampler.h>

class LambertBsdf
{
private:
    /* data */
    float3 albedo;

public:
    SUTIL_INLINE SUTIL_HOSTDEVICE LambertBsdf() { albedo = make_float3(0); }
    SUTIL_INLINE SUTIL_HOSTDEVICE LambertBsdf(float3 albedo) : albedo(albedo){};
    SUTIL_INLINE SUTIL_HOSTDEVICE void setAlbedo(float3 albedo);
    SUTIL_HOSTDEVICE float3 eval(SurfaceScatterEvent &event);
    SUTIL_HOSTDEVICE bool sample(SurfaceScatterEvent &event, UniformSampler *sampler);
    SUTIL_HOSTDEVICE float pdf(SurfaceScatterEvent &event);
};

SUTIL_INLINE SUTIL_HOSTDEVICE void LambertBsdf::setAlbedo(float3 get_albedo)
{
    albedo = get_albedo;
}

SUTIL_HOSTDEVICE float3 LambertBsdf::eval(SurfaceScatterEvent &event)
{
    float3 f;

    if (event.in_dir.z <= 0.0f || event.out_dir.z <= 0.0f)
        return make_float3(0.0f);
    f = albedo * INV_PI * event.out_dir.z;
    return f;
}

SUTIL_HOSTDEVICE bool LambertBsdf::sample(SurfaceScatterEvent &event, UniformSampler *sampler)
{

    if (event.in_dir.z <= 0.0f)
        return false;
    event.out_dir = cosineHemisphere(sampler->next2D());
    event.pdf = cosineHemispherePdf(event.out_dir);
    event.weight = albedo;
    return true;
}

SUTIL_HOSTDEVICE float LambertBsdf::pdf(SurfaceScatterEvent &event)
{
    if (event.in_dir.z <= 0.0f || event.out_dir.z <= 0.0f)
        return 0.0f;
    return cosineHemispherePdf(event.out_dir);
}

#endif