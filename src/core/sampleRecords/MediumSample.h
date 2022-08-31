#ifndef MEDIUMSAMPLE_H
#define MEDIUMSAMPLE_H

#pragma once

#include <sutil/vec_math.h>

struct MediumSample
{
    float t;
    float3 weight;
    float pdf;
};


#endif
