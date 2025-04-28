/*
    the DifferentialGeometry struct is a container for information about a location on the surface of some geometry (that has a valid parametric representation)
    this "location" is typically at a ray-object intersection
    
    DifferentialGeometry holds the following information:
        * the position of the 3D Point p 
        * the normal at Point p
        * the (u, v) coordinates of Point p
        * the parametric partial derivatives del_p/del_u and del_p/del_v
        * the parametric partial derivatives of change in surface normal (slope) del_n/del_u and del_n/del_v
        * a pointer to the Shape it lies on

    these values are expected to be in world space
*/

#ifndef DIFFERENTIALGEOMETRY_H
#define DIFFERENTIALGEOMETRY_H

#include "Point.h"
class Shape;

struct DifferentialGeometry
{
    /* PUBLIC MEMBERS */
    Point p;
    Normal nn; // normalized normal 
    float u, v;
    const Shape* shape;
    
    Vector dpdu, dpdv;
    Vector dndu, dndv;

    /* CONSTRUCTORS */
    DifferentialGeometry() :
        u(0.0f),
        v(0.0f),
        shape(nullptr)
    {}
    
    DifferentialGeometry(const Point& p,
        const Vector& dpdu, const Vector& dpdv,
        const Vector& dndu, const Vector& dndv,
        float u, float v, Shape* shape
    ) :
        p(p),
        dpdu(dpdu),
        dpdv(dpdv),

        nn( Normal(normalize(cross(dpdu, dpdv))) ),
        u(u),
        v(v),
        shape(shape)
    {}
};

#endif // DIFFERENTIALGEOMETRY_H