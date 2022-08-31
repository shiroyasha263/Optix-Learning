#ifndef PHASESAMPLE_H
#define PHASESAMPLE_H

#pragma once

#include <sutil/vec_math.h>


struct PhaseSample
{
    float3 dir;
    float3 weight;
    float pdf;
};


#endif
