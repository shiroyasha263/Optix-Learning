#ifndef DIELECTRICBSDH_H
#define DIELECTRICBSDH_H
#pragma once

#include <sampling/SamplingFunctions.h>

#include <math/Angle.h>
#include <sampleRecords/SurfaceScatterEvent.h>
#include <sampling/UniformSampler.h>

class DielectricBsdf
{
private:
    /* data */
    float3 albedo;
    float ior;
    float invIor;

public:
    SUTIL_INLINE SUTIL_HOSTDEVICE DielectricBsdf(float3 albedo, float ior) : albedo(albedo), ior(ior) { invIor = 1 / ior; };
    SUTIL_INLINE SUTIL_HOSTDEVICE void setAlbedo(float3 albedo);
    SUTIL_HOSTDEVICE float3 eval(SurfaceScatterEvent &event);
    SUTIL_HOSTDEVICE bool sample(SurfaceScatterEvent &event, UniformSampler *sampler);
    SUTIL_HOSTDEVICE float pdf(SurfaceScatterEvent &event);
    SUTIL_HOSTDEVICE float eta(SurfaceScatterEvent &event);

    SUTIL_INLINE SUTIL_HOSTDEVICE bool sample(SurfaceScatterEvent &event, UniformSampler *sampler, bool adjoint)
    {
        if (!sample(event, sampler))
            return false;

        // TODO: Try to find out the significance of adjoint used by author here.
        if (adjoint)
            event.weight *= 1;
        else
            event.weight *= make_float3(powf(eta(event), 2));

        return true;
    }
    SUTIL_INLINE SUTIL_HOSTDEVICE float3 eval(SurfaceScatterEvent &event, bool adjoint)
    {
        float3 f = eval(event);
        
        // TODO: Try to find out the significance of adjoint used by author here.
        if (adjoint)
            f *= 1;
        else
            f *= make_float3(powf(eta(event), 2));

        return f;
    }
};

SUTIL_INLINE SUTIL_HOSTDEVICE void DielectricBsdf::setAlbedo(float3 set_albedo)
{
    albedo = set_albedo;
}

SUTIL_HOSTDEVICE float3 DielectricBsdf::eval(SurfaceScatterEvent &event)
{
    // TODO: In Future, below two values will be set up arguments
    bool evalR = true;
    bool evalT = true;

    float eta = event.in_dir.z < 0.0f ? ior : invIor;
    float cosThetaT = 0.0f;
    float F = dielectricReflectance(eta, abs(event.in_dir.z), cosThetaT);

    if (event.in_dir.z * event.out_dir.z >= 0.0f)
    {
        if (evalR && checkReflectionConstraint(event.in_dir, event.out_dir))
            return F * albedo;
        else
            return make_float3(0.0f);
    }
    else
    {
        if (evalT && checkRefractionConstraint(event.in_dir, event.out_dir, eta, cosThetaT))
            return (1.0f - F) * albedo;
        else
            return make_float3(0.0f);
    }
}

SUTIL_HOSTDEVICE bool DielectricBsdf::sample(SurfaceScatterEvent &event, UniformSampler *sampler)
{
    // TODO: In Future, below two values will be set up arguments
    bool sampleR = true;
    bool sampleT = true;

    float eta = event.in_dir.z < 0.0f ? ior : invIor;
    float cosThetaT = 0.0f;
    float F = dielectricReflectance(eta, abs(event.in_dir.z), cosThetaT);

    float reflectionProbability;
    if (sampleR && sampleT)
        reflectionProbability = F;
    else if (sampleR)
        reflectionProbability = 1.0f;
    else if (sampleT)
        reflectionProbability = 0.0f;
    else
        return false;

    if (sampler->nextBoolean(reflectionProbability))
    {
        event.out_dir = make_float3(-event.in_dir.x, -event.in_dir.y, event.in_dir.z);
        event.pdf = reflectionProbability;
        event.weight = sampleT ? make_float3(1.0f) : make_float3(F);
    }
    else
    {
        if (F == 1.0f)
            return false;

        event.out_dir = make_float3(-event.in_dir.x * eta, -event.in_dir.y * eta, -copysignf(cosThetaT, event.in_dir.z));
        event.pdf = 1.0f - reflectionProbability;
        event.weight = sampleR ? make_float3(1.0f) : make_float3(1.0f - F);
    }
    event.weight *= albedo;

    return true;
}

SUTIL_HOSTDEVICE float DielectricBsdf::pdf(SurfaceScatterEvent &event)
{
    // TODO: In Future, below two values will be set up arguments
    bool sampleR = true;
    bool sampleT = true;

    float eta = event.in_dir.z < 0.0f ? ior : invIor;
    float cosThetaT = 0.0f;
    float F = dielectricReflectance(eta, fabsf(event.in_dir.z), cosThetaT);

    if (event.in_dir.z * event.out_dir.z >= 0.0f)
    {
        if (sampleR && checkReflectionConstraint(event.in_dir, event.out_dir))
            return sampleT ? F : 1.0f;
        else
            return 0.0f;
    }
    else
    {
        if (sampleT && checkRefractionConstraint(event.in_dir, event.out_dir, eta, cosThetaT))
            return sampleR ? 1.0f - F : 1.0f;
        else
            return 0.0f;
    }
    if (event.in_dir.z <= 0.0f || event.out_dir.z <= 0.0f)
        return 0.0f;
    return cosineHemispherePdf(event.out_dir);
}

SUTIL_HOSTDEVICE float DielectricBsdf::eta(SurfaceScatterEvent &event)
{
    if (event.in_dir.z * event.out_dir.z >= 0.0f)
        return 1.0f;
    else
        return event.in_dir.z < 0.0f ? ior : invIor;
}

#endif