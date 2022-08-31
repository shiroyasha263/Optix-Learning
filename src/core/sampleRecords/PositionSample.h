#ifndef POSITIONSAMPLE_H
#define POSITIONSAMPLE_H

#pragma once

#include <sutil/vec_math.h>
#include <sutil/Preprocessor.h>

struct PositionSample
{
    float3 pos;
    float3 weight;
    float pdf;

    SUTIL_INLINE SUTIL_HOSTDEVICE PositionSample() = default;
    SUTIL_INLINE SUTIL_HOSTDEVICE PositionSample(const float3 &pos_)
    : pos(pos_),
      weight({0.0f, 0.0f, 0.0f})
    {}
};


#endif
