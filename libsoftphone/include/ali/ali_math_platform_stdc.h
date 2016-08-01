#pragma once

#include "ali_math_common.h"
#include <math.h>
#include <float.h>

namespace ali
{

namespace math
{

// ******************************************************************
float const float_epsilon = FLT_EPSILON;
    // smallest such that 1.0 + FLT_EPSILON != 1.0
// ******************************************************************

// ******************************************************************
float const float_min = FLT_MIN;
// ******************************************************************

// ******************************************************************
float const float_max = FLT_MAX;
// ******************************************************************

// ******************************************************************
inline float sin( float f )
// ******************************************************************
{
#ifdef _WIN32_WCE
	return static_cast<float>(::sin(f));
#else
	return ::sinf(f);
#endif
}

// ******************************************************************
inline double sin( double f )
// ******************************************************************
{
	return ::sin(f);
}

// ******************************************************************
inline float cos( float f )
// ******************************************************************
{
#ifdef _WIN32_WCE
	return static_cast<float>(::cos(f));
#else
	return ::cosf(f);
#endif
}

// ******************************************************************
inline double cos( double f )
// ******************************************************************
{
	return ::cos(f);
}

// ******************************************************************
inline float asin( float f )
// ******************************************************************
{
#ifdef _WIN32_WCE
	return static_cast<float>(::asin(f));
#else
	return ::asinf(f);
#endif
}

// ******************************************************************
inline double asin( double f )
// ******************************************************************
{
	return ::asin(f);
}

// ******************************************************************
inline float acos( float f )
// ******************************************************************
{
#ifdef _WIN32_WCE
	return static_cast<float>(::acos(f));
#else
	return ::acosf(f);
#endif
}

// ******************************************************************
inline double acos( double f )
// ******************************************************************
{
	return ::acos(f);
}

// ******************************************************************
inline float atan2( float y, float x )
// ******************************************************************
{
    return ::atan2f(y, x);
}

// ******************************************************************
inline double atan2( double y, double x )
// ******************************************************************
{
    return ::atan2(y, x);
}

// ******************************************************************
inline float abs( float f )
// ******************************************************************
{
	return ::fabsf(f);
}

// ******************************************************************
inline double abs( double f )
// ******************************************************************
{
	return ::fabs(f);
}

// ******************************************************************
inline float sqrt( float f )
// ******************************************************************
{
	return ::sqrtf(f);
}

// ******************************************************************
inline double sqrt( double f )
// ******************************************************************
{
	return ::sqrt(f);
}

// ******************************************************************
inline float floor( float f )
// ******************************************************************
{
#ifdef _WIN32_WCE
	return static_cast<float>(::floor(f));
#else
	return ::floorf(f);
#endif
}

// ******************************************************************
inline double floor( double f )
// ******************************************************************
{
    return ::floor(f);
}

// ******************************************************************
inline float ceil( float f )
// ******************************************************************
{
#ifdef _WIN32_WCE
    return static_cast<float>(::ceil(f));
#else
    return ::ceilf(f);
#endif
}

// ******************************************************************
inline double ceil( double f )
// ******************************************************************
{
    return ::ceil(f);
}
    
// ******************************************************************
inline float round( float f )
// ******************************************************************
//  This function rounds f to the nearest integer, but rounds
//  halfway cases away from zero.
// ******************************************************************
{
    return f < 0.0f ? ceil(f - 0.5f) : floor(f + 0.5f);
}

// ******************************************************************
inline double round( double f )
// ******************************************************************
//  This function rounds f to the nearest integer, but rounds
//  halfway cases away from zero.
// ******************************************************************
{
    return f < 0.0 ? ceil(f - 0.5) : floor(f + 0.5);
}

// ******************************************************************
inline float log( float f )
// ******************************************************************
{
#ifdef _WIN32_WCE
	return static_cast<float>(::log(f));
#else
	return ::logf(f);
#endif
}

// ******************************************************************
inline double log( double f )
// ******************************************************************
{
	return ::log(f);
}

// ******************************************************************
inline float log10( float f )
// ******************************************************************
{
#ifdef _WIN32_WCE
	return static_cast<float>(::log10(f));
#else
	return ::log10f(f);
#endif
}

// ******************************************************************
inline double log10( double f )
// ******************************************************************
{
	return ::log10(f);
}

// ******************************************************************
inline float pow( float x, float y )
// ******************************************************************
{
#ifdef _WIN32_WCE
	return static_cast<float>(::pow(x,y));
#else
	return ::powf(x,y);
#endif
}

// ******************************************************************
inline double pow( double x, double y )
// ******************************************************************
{
	return ::pow(x, y);
}

// ******************************************************************
inline float exp( float x )
// ******************************************************************
{
#ifdef _WIN32_WCE
	return static_cast<float>(::exp(x));
#else
	return ::expf(x);
#endif
}

// ******************************************************************
inline double exp( double x )
// ******************************************************************
{
	return ::exp(x);
}

}   //  namespace math

}   //  namespace ali
