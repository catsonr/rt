/*
    Shape.h defines the abstract Shape class, which holds information every shape would have
    
    Shapes operate within their own object coordiante space
    they hold Transforms that go from object_to_world and back
*/

#ifndef SHAPE_H
#define SHAPE_H

#include "Transform.h"
#include "DifferentialGeometry.h"

class Shape
{
public:
    /* PUBLIC MEMBERS */
    const Transform object_to_world, world_to_object;
    const bool reverseOrientation, transformSwapsHandedness;
    
    /* CONSTRUCTORS */
    Shape(const Transform& object_to_world, bool reverseOrientation = false) :
        object_to_world(object_to_world),
        world_to_object(object_to_world.getInverse()),
        reverseOrientation(reverseOrientation),
        transformSwapsHandedness( object_to_world.swapsHandedness() )
    {}
    
    /* VIRTUAL METHODS */
    virtual Bbox objectBound() const = 0;
    virtual bool intersect(const Ray& ray, float* t_hit, DifferentialGeometry* dg) const = 0;
    virtual bool doesIntersect(const Ray& ray) const = 0; // aka IntersectP() in pbrt

    virtual Bbox worldBound() const
    {
        return object_to_world( objectBound() );
    }
    virtual bool isIntersectable() const
    {
        return true; // if you want this to be false, impliment refine() at (pg. 92) of pbrt 2nd ed.
    }
};

#endif // Shape