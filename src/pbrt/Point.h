/*
    Point is a class that represents a single point in 3D space (x, y, z)
*/

#ifndef POINT_H
#define POINT_H

#include "Vector.h"

class Point
{
public:
    /* PUBLIC MEMBERS */
    float x, y, z;
    
    /* CONSTRUCTORS */
    Point() :
        x(0.0f),
        y(0.0f),
        z(0.0f)
    {}
    Point(float x, float y, float z) :
        x(x),
        y(y),
        z(z)
    {}
    
    /* INLINE OPERATOR OVERLOADS */
    inline Point operator+(const Vector& v) const
    {
        return Point(x + v.x, y + v.y, z + v.z);
    }
    inline Point& operator+=(const Vector& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        
        return *this;
    }

    inline Point operator-(const Vector& v) const
    {
        return Point(x - v.x, y - v.y, z - v.z);
    }
    inline Point& operator-=(const Vector& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        
        return *this;
    }
    inline Vector operator-(const Point& p) const
    {
        return Vector(x - p.x, y - p.y, z - p.z);
    }
    
    float operator[](const int i) const
    {
        if(i == 0) return x;
        else if(i == 1) return y;
        else if(i == 2) return z;
        else
        {
            printf("Point has no member of index %i!\n", i);
            
            return NAN;
        }
    }
};

#endif // POINT_H