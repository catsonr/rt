/*
    Vector is a class that represents a single 3D vector, and thus has three members:
        float x, y, z

    Normal is a class that represents a single 3D normal
        Normal is effectively a Vector, but they are treated explicitly
        as different types of data since they do not operate exactly
        the same, and should not be treated as the same thing
        they can however be cast back and forth between one another 
            e.g.: Normal n = Normal(v)
                  Vector v = Vector(n)
                  
    Vector.h also defines Normal
*/

#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <cmath>
#include <cassert>

// forward definitions
class Vector;
class Normal;

class Vector
{
public:
    /* PUBLIC MEMBERS */
    float x, y, z;
    
    /* CONSTRUCTORS */
    Vector() :
        x(0.0f),
        y(0.0f),
        z(0.0f)
    {}
    Vector(float x, float y, float z) :
        x(x),
        y(y),
        z(z)
    {}
    explicit Vector(const Normal& n);

    /* PUBLIC METHODS */
    float lengthSquared() const
    {
        return x*x + y*y + z*z;
    }

    float length() const
    {
        return sqrtf( lengthSquared() );
    }
    
    /* INLINE OPERATOR OVERLOADS */
    inline Vector operator+(const Vector& v) const
    {
        return Vector(x + v.x, y + v.y, z + v.z);
    }
    inline Vector& operator+=(const Vector& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;

        return *this;
    }

    inline Vector operator-(const Vector& v) const
    {
        return Vector(x - v.x, y - v.y, z - v.z);
    }
    inline Vector& operator-=(const Vector& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;

        return *this;
    }
    
    inline Vector operator*(float f) const
    {
        return Vector(x * f, y * f, z * f);
    }
    inline Vector& operator*=(float f)
    {
        x *= f;
        y *= f;
        z *= f;
        
        return *this;
    }

    inline Vector operator/(float f) const
    {
        return Vector(x / f, y / f, z / f);
    }
    inline Vector& operator/=(float f)
    {
        x /= f;
        y /= f;
        z /= f;
        
        return *this;
    }
    
    inline Vector operator-() const
    {
        return Vector(-x, -y, -z);
    }
    
    inline float operator[](const int i) const
    {
        if(i == 0) return x;
        else if(i == 1) return y;
        else if(i == 2) return z;
        else
        {
            printf("Vector has no member of index %i!\n", i);
            
            return NAN;
        }
    }
}; // Vector 

/* GLOBAL INLINE FUNCTIONS (that only use Vectors) */
inline float dot(const Vector& v1, const Vector& v2)
{
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

inline float dotAbs(const Vector& v1, const Vector& v2)
{
    return fabsf( dot(v1, v2) );
}

inline Vector cross(const Vector& v1, const Vector& v2)
{
    return Vector(
        v1.y*v2.z - v1.z*v2.y,
        v1.z*v2.x - v1.x*v2.z,
        v1.x*v2.y - v1.y*v2.x
    );
}

inline Vector normalize(const Vector& v)
{
    return v / v.length();
}



class Normal
{
public:
    /* PUBLIC MEMBERS */
    float x, y, z;
    
    /* CONSTRUCTORS */
    Normal() :
        x(0.0f),
        y(0.0f),
        z(0.0f)
    {}
    Normal(float x, float y, float z) :
        x(x),
        y(y),
        z(z)
    {}
    explicit Normal(const Vector& v);
    
    /* PUBLIC METHODS */
    inline float lengthSquared() const
    {
        return x*x + y*y + z*z;
    }
    inline float length() const
    {
       return sqrtf( lengthSquared() );
    }
    
    /* OPERATOR OVERLOADS */
    inline Normal operator+(const Normal& n) const
    {
        return Normal(x + n.x, y + n.y, z + n.z);
    }
    inline Normal& operator+=(const Normal& n)
    {
        x += n.x;
        y += n.y;
        z += n.z;
        
        return *this;
    }

    inline Normal operator-(const Normal& n) const
    {
        return Normal(x - n.x, y - n.y, z - n.z);
    }
    inline Normal& operator-=(const Normal& n)
    {
        x -= n.x;
        y -= n.y;
        z -= n.z;
        
        return *this;
    }

    inline Normal operator*(float f) const
    {
        return Normal(x*f, y*f, z*f);
    }
    inline Normal& operator*=(float f)
    {
        x *= f;
        y *= f;
        z *= f;
        
        return *this;
    }

    inline Normal operator/(float f) const
    {
        assert(f != 0);
        float inv = 1.0f / f;

        return Normal(x*inv, y*inv, z*inv);
    }
    inline Normal& operator/=(float f)
    {
        assert(f != 0);
        float inv = 1.0f / f;
        
        x *= inv;
        y *= inv;
        z *= inv;
        
        return *this;
    }
    
    inline Normal operator-() const
    {
        return Normal(-x, -y, -z);
    }
}; // Normal 

/* GLOBAL INLINE FUNCTIONS (that only use Normals) */
// note that Normal does not have a cross() function--the cross product of two normals does not behave as expected
inline float dot(const Normal& n1, const Normal& n2)
{
    return n1.x*n2.x + n1.y*n2.y + n1.z*n2.z;
}

inline float dotAbs(const Normal& n1, const Normal& n2)
{
    return fabsf( dot(n1, n2) );
}

inline Normal normalize(const Normal& n)
{
    return n / n.length();
}

/* EXPLICIT CONSTRUCTOR BODIES */
// defining the bodies here (or in a .cpp file, but i wanna keep everything here) breaks the circular definition cycle 
inline Vector::Vector(const Normal& n) :
    x(n.x),
    y(n.y),
    z(n.z)
{}

inline Normal::Normal(const Vector& v) :
    x(v.x),
    y(v.z),
    z(v.z)
{}

#endif // VECTOR_H