/*
   contains global variables for anything rt related
   contained within namespace rt
*/

#ifndef GLOBALS_H
#define GLOBALS_H

#include <algorithm>

#include "Vector.h"

namespace rt
{
    // GLOBAL VARIABLES
    constexpr int CANVAS_WIDTH = 800;
    constexpr int CANVAS_HEIGHT = 600;
    constexpr float aspectRatio = CANVAS_HEIGHT / CANVAS_WIDTH;

    constexpr float PI = 3.14159265358979323846f;
    constexpr float TWOPI = 6.283185482025146484375f;
    constexpr float ONE_OVER_PI = 0.31830988618379067154f;
    constexpr float ONE_OVER_TWOPI = 0.15915494309189533577f;
    
    constexpr float RAY_EPSILON = 1e-3f;

    constexpr int SPECTRUM_COLORSAMPLES = 3;

    // GLOBAL INLINES
    // solves quadratic equation given A, B, C, and t0 t1 variables
    // returns true if there were solutions and false otherwise
    constexpr bool solveQuadratic(float A, float B, float C, float* t0, float* t1)
    {
        float discriminant = B*B - 4.0f * A * C;
        if(discriminant < 0.0f) return false;
        float discriminant_sqrt = sqrtf(discriminant);

        float q = (B < 0) ? -0.5f * (B - discriminant_sqrt) : -0.5f * (B + discriminant_sqrt);

        *t0 = q / A;
        *t1 = C / q;

        if(*t0 > *t1) std::swap(*t0, *t1);
        
        return true;
    }
    // clamps value between (inclusive) [min, max]
    constexpr float clamp(float value, float min, float max)
    {
        if (value <= min)
            return min;
        else if (value >= max)
            return max;

        return value;
    }
    // computes i % mod for all integers
    constexpr int mod(int i, int mod)
    {
        int n = int(i / mod);
        i -= n * mod;
        
        if(i < 0) i += mod;
        
        return i;
    }
    // converts degrees to radians
    constexpr float radians(float degrees)
    {
        return (PI / 180.0f) * degrees;
    }
    constexpr float degrees(float radians)
    {
        return (180.0f / PI) * radians;
    }
    
    // returns a (unit) vector pointed at a point on a sphere
    // using spherical coordinates
    inline Vector sphericalDirection(float sinTheta, float cosTheta, float phi)
    {
        return Vector(
                sinTheta * cosf(phi),
                sinTheta * sinf(phi),
                cosTheta
        );
    }
    /*inline Vector sphericalDirection(float sinTheta, float cosTheta, float phi, const Vector& x, const Vector& y, const Vector& z)
    {
        return x * sinTheta * cosf(phi) +
        y * sinTheta * sinf(phi) +
        z * cosTheta;
    }*/
    
    // returns theta component of Vector v (in spherical coords)
    inline float sphericalTheta(const Vector& v)
    {
        return acosf(v.z);
    }
    // returns 
    inline float sphericalPhi(const Vector& v)
    {
        float p = atan2f(v.y, v.x);
        
        return (p < 0.0f) ? p + TWOPI : p;
    }
    
    inline bool solveLinearSystem2x2(const float A[2][2], const float B[2], float x[2])
    {
        float det = A[0][0] * A[1][1] - A[0][1] * A[1][0];
        
        if(fabsf(det) < 1e-5f)
            return false;
        
        float det_inv = 1.0f / det;
        x[0] = (A[1][0] * B[0] - A[0][1] * B[1]) * det_inv;
        x[1] = (A[0][0] * B[1] - A[1][0] * B[0]) * det_inv;
        
        return true;
    }
} // rt

#endif