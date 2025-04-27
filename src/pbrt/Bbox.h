/*
   the Bbox class is a rectangular 3D bounding box
   it is defined by two Points, which can be thought of as the bottom left and top right of the bounding box
   (which will be referred to as p_min and p_max)
   
   if p_min <= p <= p_max
    p is in the Bbox :)
*/

#ifndef BBOX_H
#define BBOX_H

#include "Point.h"
class Ray;

class Bbox
{
public:
    /* PUBLIC MEMBERS */
    Point p_min, p_max;

    /* CONSTRUCTORS */
    Bbox();
    Bbox(const Point& p);
    Bbox(const Point& p1, const Point& p2);
    
    /* PUBLIC METHODS */
    // returns true if the given Bbox b overlaps at all with this Bbox 
    bool overlaps(const Bbox& box) const;
    // returns true if the Point p is within this Bbox
    bool containsPoint(const Point& p) const;
    // returns true if Ray ray intersects this Bbox. if it does, t_hit0 and t_hit1 are updated accordingly 
    bool intersectsP(const Ray& ray, float* t_hit0, float* t_hit1) const;

    // returns the volume of the Bbox
    float volume() const;

    // returns which of the three axes is the longest
    // x=0,  y=1,  z=2
    int maximumExtent() const;
    
    // creates a new Bbox that contains both box and the Point p
    static Bbox Union(const Bbox& box, const Point& p);
    // creates a new Bbox that contains both box1 and box2
    static Bbox Union(const Bbox& box1, const Bbox& box2);

    // expands this Bbox by a constant factor delta 
    void expand(float delta);
};

#endif // BBOX_H