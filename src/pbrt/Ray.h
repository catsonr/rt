/*
    Ray is a class that represents a mathematical ray. That is, an object with location in space (origin) and a direction
    Ray also has t_min and t_max for the range of values that t can take, which defines where along the infinite line defined by
    the origin and direction the ray (o + dt) is allowed to exist
    t is the time at which the ray exists; used for motion blur
    
    RayDifferential is a subclass of Ray that includes information about the small offsets in direction the ray can take (rays rx and ry)
    these offsets are used for antialiasing
*/

#ifndef RAY_H
#define RAY_H

#include "globals.h"
#include "Point.h"

class Ray
{
public:
    /* PUBLIC MEMBERS */
    Point o;
    Vector d;
    mutable float t_min, t_max;
    float t;
    
    /* CONSTRUCTORS */
    Ray() :
        o(Point()),
        d(Vector()),
        t_min(rt::RAY_EPSILON),
        t_max(INFINITY),
        t(0.0f)
    {}
    Ray(
        const Point& origin,
        const Vector& direction,
        float start = rt::RAY_EPSILON,
        float end = INFINITY,
        float t = 0.0f
    ) :
        o(origin),
        d(direction),
        t_min(start),
        t_max(end),
        t(t)
    {}
    
    /* INLINE OPERATOR OVERLOADS */
    inline Point operator()(float t) const
    {
        return o + d*t;
    }
};

class RayDifferential : public Ray
{
public:
    /* PUBLIC MEMBERS */
    bool hasDifferentials;
    Ray rx, ry;
    
    /* CONSTRUCTORS */
    RayDifferential() :
        hasDifferentials(false)
    {}
    RayDifferential(const Point& origin, const Vector& direction) :
        Ray(origin, direction),
        hasDifferentials(false)
    {}
    explicit RayDifferential(const Ray& ray) :
        Ray(ray),
        hasDifferentials(false)
    {}
};

#endif // RAY_H