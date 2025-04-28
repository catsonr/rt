#include <algorithm>

#include "Bbox.h"
#include "Ray.h"

/* CONSTRUCTORS */
Bbox::Bbox() :
    p_min( Point(INFINITY, INFINITY, INFINITY) ),
    p_max( Point(-INFINITY, -INFINITY, -INFINITY) )
{}

Bbox::Bbox(const Point& p) :
    p_min(p),
    p_max(p)
{}

Bbox::Bbox(const Point& p1, const Point& p2)
{
    const float x_min = fmin(p1.x, p2.x);
    const float x_max = fmax(p1.x, p2.x);

    const float y_min = fmin(p1.y, p2.y);
    const float y_max = fmax(p1.y, p2.y);

    const float z_min = fmin(p1.z, p2.z);
    const float z_max = fmax(p1.z, p2.z);
    
    p_min = Point(x_min, y_min, z_min);
    p_max = Point(x_max, y_max, z_max);
}

/* PUBLIC METHODS */
bool Bbox::overlaps(const Bbox& box) const
{
    bool x = (p_max.x >= box.p_min.x) && (p_min.x <= box.p_max.x);
    bool y = (p_max.y >= box.p_min.y) && (p_min.y <= box.p_max.y);
    bool z = (p_max.z >= box.p_min.z) && (p_min.z <= box.p_max.z);
    
    return x && y && z;
}

bool Bbox::containsPoint(const Point& p) const
{
    return (
        p.x >= p_min.x && p.x <= p_max.x &&
        p.y >= p_min.y && p.y <= p_max.y &&
        p.z >= p_min.z && p.z <= p_max.z
    );
}

bool Bbox::intersectsP(const Ray& ray, float* t_hit0, float* t_hit1) const
{
    float t0 = ray.t_min;
    float t1 = ray.t_max;
    
    // test intersection agaisnt all xyz planes of the bbox
    for(int i = 0; i < 3; i++)
    {
        float raydirInv = 1.0f / ray.d[i];
        float t_near = (p_min[i] - ray.o[i]) * raydirInv;
        float t_far = (p_max[i] - ray.o[i]) * raydirInv;
        
        if(t_near > t_far) std::swap(t_near, t_far);
        
        // this differs from pbrt, but it's the same idea
        t0 = std::max(t0, t_near);
        t1 = std::min(t1, t_far);
        
        if(t0 > t1) return false;
    }
    
    if(t_hit0) *t_hit0 = t0;
    if(t_hit1) *t_hit1 = t1;
    
    return true;
}

float Bbox::volume() const
{
    return (p_max.x - p_min.x) * (p_max.y - p_min.y) * (p_max.z - p_min.z);
}

int Bbox::maximumExtent() const
{
    Vector d = p_max - p_min;
    
    if(d.x > d.y && d.x > d.z)
        return 0;
    else if(d.y > d.z)
        return 1;
    
    return 2;
}

Bbox Bbox::Union(const Bbox& box, const Point& p)
{
    Bbox newbox = box;
    
    newbox.p_min.x = fmin(box.p_min.x, p.x);
    newbox.p_min.y = fmin(box.p_min.y, p.y);
    newbox.p_min.z = fmin(box.p_min.z, p.z);
    
    newbox.p_max.x = fmax(box.p_max.x, p.x);
    newbox.p_max.y = fmax(box.p_max.y, p.y);
    newbox.p_max.z = fmax(box.p_max.z, p.z);
    
    return newbox;
}

Bbox Bbox::Union(const Bbox& box1, const Bbox& box2)
{
    Bbox newbox = box1;

    newbox.p_min.x = fmin(box1.p_min.x, box2.p_min.x);
    newbox.p_min.y = fmin(box1.p_min.y, box2.p_min.y);
    newbox.p_min.z = fmin(box1.p_min.z, box2.p_min.z);
    
    newbox.p_max.x = fmax(box1.p_max.x, box2.p_max.x);
    newbox.p_max.y = fmax(box1.p_max.y, box2.p_max.y);
    newbox.p_max.z = fmax(box1.p_max.z, box2.p_max.z);
    
    return newbox;
}

void Bbox::expand(float delta)
{
    Vector d(delta, delta, delta);

    p_min -= d;
    p_max += d;
}