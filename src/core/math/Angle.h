#ifndef ANGLE_H
#define ANGLE_H

#pragma once

#include <sutil/Preprocessor.h>


#define PI          3.1415926536f
#define PI_HALF     PI*0.5f
#define TWO_PI      PI*2.0f
#define FOUR_PI     PI*4.0f
#define INV_PI      1.0f/PI
#define INV_TWO_PI  0.5f*INV_PI
#define INV_FOUR_PI 0.25f*INV_PI
#define SQRT_PI     1.77245385091f
#define INV_SQRT_PI 1.0f/SQRT_PI

class Angle
{
public:
    static SUTIL_INLINE SUTIL_HOSTDEVICE constexpr float radToDeg(float a)
    {
        return a*(180.0f/ PI);
    }

    static SUTIL_INLINE SUTIL_HOSTDEVICE constexpr float degToRad(float a)
    {
        return a*(PI/180.0f);
    }
};


#endif