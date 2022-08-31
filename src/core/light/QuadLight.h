#ifndef QUADLIGHT_H
#define QUADLIGHT_H

#pragma once

#include <sampleRecords/DirectionSample.h>
#include <sampleRecords/PositionSample.h>
#include <sampling/SamplingFunctions.h>
#include <math/TangentFrame.h>
#include <math/Angle.h>
#include <media/HomogeneousMedium.h>

#include <sampling/UniformSampler.h>

class QuadLight
{
private:
    /* Variables */
    float3 corner;
    float3 v1, v2;
    float3 normal;

    TangentFrame frame;
    float3 emission;
    float3 power;

    float area=1.0f;
    float inv_area=1.0f;
    float quad_width;
    float quad_height;
    HomogeneousMedium *out_medium = NULL;
    /* Functions */
public:
    /* Variables */
    /* Functions */
    SUTIL_INLINE SUTIL_HOSTDEVICE QuadLight(float3 _corner, float3 _v1, float3 _v2, float3 _emission);
    SUTIL_INLINE SUTIL_HOSTDEVICE float width() { return quad_width; }
    SUTIL_INLINE SUTIL_HOSTDEVICE float height() { return quad_height; }
    SUTIL_INLINE SUTIL_HOSTDEVICE float powerToRadianceFactor() const;
    SUTIL_INLINE SUTIL_HOSTDEVICE void setExtMedium(HomogeneousMedium *medium) { out_medium = medium; }
    SUTIL_INLINE SUTIL_HOSTDEVICE HomogeneousMedium *extMedium() { return out_medium; }
    SUTIL_HOSTDEVICE bool samplePosition(UniformSampler *sampler, PositionSample &sample) const;
    SUTIL_HOSTDEVICE bool sampleDirection(UniformSampler *sampler, const PositionSample &point, DirectionSample &sample) const;
};

SUTIL_INLINE SUTIL_HOSTDEVICE float QuadLight::powerToRadianceFactor() const
{
    return INV_PI * inv_area;
}

SUTIL_INLINE SUTIL_HOSTDEVICE QuadLight::QuadLight(float3 _corner, float3 _v1, float3 _v2, float3 _power)
{
    corner = _corner;
    v1 = _v1;
    v2 = _v2;
    normal = cross(v1, v2);
    area = length(normal);
    inv_area = 1.0f / area;

    emission = _power * powerToRadianceFactor();
    normal = normalize(normal);
    frame = TangentFrame(normal, normalize(v1), normalize(v2));
    quad_width = length(_v1);
    quad_height = length(_v2);
}

SUTIL_HOSTDEVICE bool QuadLight::samplePosition(UniformSampler *sampler, PositionSample &sample) const
{
    float2 p = sampler->next2D();
    sample.pos = corner + p.x * v1 + p.y * v2;
    sample.pdf = inv_area;
    sample.weight = PI * area * emission;

    return true;
}

SUTIL_HOSTDEVICE bool QuadLight::sampleDirection(UniformSampler *sampler, const PositionSample &point, DirectionSample &sample) const
{
    float3 dir = cosineHemisphere(sampler->next2D());
    sample.dir = frame.toGlobal(dir);
    sample.weight = make_float3(1.0f);
    sample.pdf = cosineHemispherePdf(dir);

    return true;
}

#endif