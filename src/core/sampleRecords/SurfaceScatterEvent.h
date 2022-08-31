#ifndef SURFACESCATTEREVENT_H
#define SURFACESCATTEREVENT_H

#pragma once

#include <sutil/vec_math.h>
#include <sutil/Preprocessor.h>

#include <math/TangentFrame.h>

struct SurfaceScatterEvent
{
    /* Intersection Info */
    float3 pos;
    float3 normal;

    TangentFrame frame;
    float3 in_dir, out_dir;
    float3 weight;

    float pdf;
    bool geometricBackside;

    SUTIL_INLINE SUTIL_HOSTDEVICE SurfaceScatterEvent() = default;
    SUTIL_INLINE SUTIL_HOSTDEVICE SurfaceScatterEvent(float3 pos, float3 normal, float3 frame, float3 in_dir): pos(pos), normal(normal), frame(frame), in_dir(in_dir) {}
};


#endif