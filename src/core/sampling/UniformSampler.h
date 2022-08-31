#ifndef UNIFORMSAMPLER_H
#define UNIFORMSAMPLER_H

#pragma once

#if defined(__CUDA_ARCH__)
#else
#include <time.h>
#endif

#include <sutil/vec_math.h>
#include <sutil/Preprocessor.h>

#include <curand.h>
#include <curand_kernel.h>

static SUTIL_INLINE SUTIL_HOSTDEVICE float uintBitsToFloat(unsigned int i)
{
    union
    {
        float f;
        unsigned int i;
    } unionHack;
    unionHack.i = i;
    return unionHack.f;
}

// 2x-5x faster than i/float(UINT_MAX)
static SUTIL_INLINE SUTIL_HOSTDEVICE float normalizedUint(unsigned int i)
{
    return uintBitsToFloat((i >> 9u) | 0x3F800000u) - 1.0f;
}

class UniformSampler
{
private:
    unsigned long seed;
    unsigned long sequence;
    curandState_t state;

public:
    SUTIL_INLINE SUTIL_HOSTDEVICE UniformSampler() : UniformSampler(0xBA5EBA11){};
    SUTIL_INLINE SUTIL_HOSTDEVICE UniformSampler(unsigned long _seed, unsigned long _sequence = 0) : seed(_seed), sequence(_sequence)
    {
        // TODO: Uncomment below when suitable  
        // #if defined(__CUDA_ARCH__)
        // seed += clock();
        // #else
        // seed += time(0);
        // #endif

#if defined(__CUDA_ARCH__)
        curand_init(seed, sequence, 0, &state);
#else
        srand(seed);
#endif
    };

    SUTIL_INLINE SUTIL_HOSTDEVICE unsigned int nextI();
    SUTIL_INLINE SUTIL_HOSTDEVICE float next1D();
    SUTIL_INLINE SUTIL_HOSTDEVICE float2 next2D();
    SUTIL_INLINE SUTIL_HOSTDEVICE float nextF();
    SUTIL_INLINE SUTIL_HOSTDEVICE float2 next2F();
    SUTIL_INLINE SUTIL_HOSTDEVICE bool nextBoolean(float pTrue);
    SUTIL_INLINE SUTIL_HOSTDEVICE void setSeed(unsigned long set_seed)
    {
        seed = set_seed;
    }
};

SUTIL_INLINE SUTIL_HOSTDEVICE unsigned int UniformSampler::nextI()
{
// call random number generator specialized for cuda.
#if defined(__CUDA_ARCH__)
    return curand(&state);
#else
    return rand();
#endif
}

SUTIL_INLINE SUTIL_HOSTDEVICE float UniformSampler::nextF()
{
// call random number generator specialized for cuda.
#if defined(__CUDA_ARCH__)
    return curand_uniform(&state);
#else
    return float(rand()) / float(RAND_MAX);
#endif
}

SUTIL_INLINE SUTIL_HOSTDEVICE float UniformSampler::next1D()
{
    return normalizedUint(nextI());
}

SUTIL_INLINE SUTIL_HOSTDEVICE float2 UniformSampler::next2D()
{
    return float2({normalizedUint(nextI()), normalizedUint(nextI())});
}

SUTIL_INLINE SUTIL_HOSTDEVICE float2 UniformSampler::next2F()
{
    return float2({nextF(), nextF()});
}

SUTIL_INLINE SUTIL_HOSTDEVICE bool UniformSampler::nextBoolean(float pTrue)
{
    return next1D() < pTrue;
}

#endif