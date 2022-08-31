#ifndef DIRECTIONSAMPLE_H
#define DIRECTIONSAMPLE_H

#pragma once

#include <sutil/vec_math.h>
#include <sutil/Preprocessor.h>

struct DirectionSample
{
    float3 dir;
    float3 weight;
    float pdf;

    SUTIL_INLINE SUTIL_HOSTDEVICE DirectionSample() = default;
    SUTIL_INLINE SUTIL_HOSTDEVICE DirectionSample(const float3 &dir_)
    : dir(dir_)
    {
    }
};

#endif
