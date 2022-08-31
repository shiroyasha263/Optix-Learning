#ifndef TANGENTSPACE_HP
#define TANGENTSPACE_HP

#pragma once

#include <sutil/vec_math.h>
#include <sutil/Preprocessor.h>
#include <sutil/Matrix.h>

struct TangentFrame
{
    float3 normal, tangent, bitangent;

    SUTIL_INLINE SUTIL_HOSTDEVICE TangentFrame() = default;

    SUTIL_INLINE SUTIL_HOSTDEVICE TangentFrame(const float3 &n, const float3 &t, const float3 &b)
        : normal(n), tangent(t), bitangent(b)
    {
    }

    SUTIL_INLINE SUTIL_HOSTDEVICE TangentFrame(const float3 &n)
        : normal(n)
    {
        // [Duff et al. 17] Building An Orthonormal Basis, Revisited. JCGT. 2017.
        float sign = copysignf(1.0f, normal.z);
        const float a = -1.0f / (sign + normal.z);
        const float b = normal.x * normal.y * a;
        tangent = normalize(float3({1.0f + sign * normal.x * normal.x * a, sign * b, -sign * normal.x}));
        bitangent = normalize(float3({b, sign + normal.y * normal.y * a, -normal.y}));
    }

    SUTIL_INLINE SUTIL_HOSTDEVICE float3 toLocal(const float3 &p) const
    {
        return float3({dot(tangent, p),
                       dot(bitangent, p),
                       dot(normal, p)});
    }

    SUTIL_INLINE SUTIL_HOSTDEVICE float3 toGlobal(const float3 &p) const
    {
        return tangent * p.x + bitangent * p.y + normal * p.z;
    }

    SUTIL_INLINE SUTIL_HOSTDEVICE sutil::Matrix<4, 4> toMatrix() const
    {
        float mat[16];
        mat[0] = tangent.x;
        mat[1] = bitangent.x;
        mat[2] = normal.x;
        mat[3] = 0;
        mat[4] = tangent.y;
        mat[5] = bitangent.y;
        mat[6] = normal.y;
        mat[7] = 0;
        mat[8] = tangent.z;
        mat[9] = bitangent.z;
        mat[10] = normal.z;
        mat[11] = 0;
        mat[12] = 0;
        mat[13] = 0;
        mat[14] = 0;
        mat[15] = 0;
        return sutil::Matrix<4, 4>(mat);
    }
};

#endif