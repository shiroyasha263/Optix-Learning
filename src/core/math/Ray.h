#ifndef RAY_H
#define RAY_H

#pragma once

#include <sutil/vec_math.h>
#include <sutil/Preprocessor.h>
#include <sutil/Matrix.h>
#define __FLT_MAX__ 3.40282346638528859811704183484516925e+38F

class Ray
{
public:
    float3 pos;
    float3 dir;
    float nearT;
    float farT;
    bool primaryRay;
    SUTIL_INLINE SUTIL_HOSTDEVICE Ray() = default;

    SUTIL_INLINE SUTIL_HOSTDEVICE Ray(const float3 &pos, const float3 &dir, float nearT = 1e-4f, float farT = __FLT_MAX__)
    : pos(pos), dir(dir), nearT(nearT), farT(farT), primaryRay(false)
    {
    }

    SUTIL_INLINE SUTIL_HOSTDEVICE Ray scatter(const float3 &newPos, const float3 &newDir, float newNearT, float newFarT) const
    {
        Ray ray(*this);
        ray.pos = newPos;
        ray.dir = newDir;
        ray.nearT = newNearT;
        ray.farT = newFarT;
        return ray;
    }

    SUTIL_INLINE SUTIL_HOSTDEVICE float3 hitpoint() const
    {
        return pos + dir*farT;
    }

    SUTIL_INLINE SUTIL_HOSTDEVICE void setPrimaryRay(bool value)
    {
        primaryRay = value;
    }
};

#endif /* RAY_HPP_ */
