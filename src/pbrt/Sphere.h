/*
    Sphere is a subclass of Shape
    TODO: impliment more quadrics
    
    Spheres are centered at the object space origin and have a radius = radius
*/

#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape
{
private:
    /* PRIVATE MEMBERS */
    float radius;
    float phi_max;
    float z_min, z_max;
    float theta_min, theta_max;

public:
    /* CONSTRUCTORS */
    Sphere(const Transform& object_to_world, bool reverseOrientation, float radius, float z_min = -rt::TWOPI, float z_max = rt::TWOPI, float phi_max_radians = rt::TWOPI) :
        Shape(object_to_world, reverseOrientation),
        radius(radius),
        z_min( rt::clamp(fminf(z_min, z_max), -radius, radius) ),
        z_max( rt::clamp(fmaxf(z_min, z_max), -radius, radius) ),
        theta_min( acosf((*this).z_min / radius) ),
        theta_max( acosf((*this).z_max / radius) ),
        phi_max(phi_max_radians)
    {}
    
    // this can be greatly improved if theta_min is considered
    // theta_min in this implimentation is ignored, so the naive solution for a bounding box is used
    Bbox objectBound() const override
    {
        return Bbox(
            Point(-radius, -radius, z_min),
            Point(radius, radius, z_max)
        );
    }
    
    // performs ray-sphere intersection test and saves results
    // returns true if there's an intersection
    bool intersect(const Ray& ray, float* t_hit, DifferentialGeometry* dg) const override // implimentation at (pg. 99) of pbrt 2nd ed.
    {
        float phi;
        Point p_hit;
        
        // transform ray into object space
        Ray r_objspc;
        world_to_object(ray, &r_objspc);

        // compute quadratic sphere coefficients 
        float A = r_objspc.d.length();
        float B = 2 * (r_objspc.d.x*r_objspc.o.x + r_objspc.d.y*r_objspc.o.y + r_objspc.d.z*r_objspc.o.z);
        float C = (r_objspc.o.x*r_objspc.o.x + r_objspc.o.y*r_objspc.o.y + r_objspc.o.z*r_objspc.o.z) - radius*radius;

        // solve quadratic equation for t values
        float t0, t1;
        if( !rt::solveQuadratic(A, B, C, &t0, &t1) ) return false;
        
        // compute intersection distance along ray
        if(t0 > ray.t_max || t1 < ray.t_min) return false;
        
        float thit = t0;
        if(t0 < ray.t_min)
        {
            thit = t1;
            if(thit > ray.t_max) return false;
        }
        // test sphere intersection against clipping parameters
        if(p_hit.z < z_min || p_hit.z > z_max || phi > phi_max)
        {
            if(thit == t1) return false;
            if(t1 > ray.t_max) return false;
            
            thit = t1;

            // compute sphere hit position and phi
            p_hit = ray(thit);
            phi = atan2f(p_hit.y, p_hit.x);
            if (phi < 0.0f) phi += rt::TWOPI;

            if(p_hit.z < z_min || p_hit.z > z_max || phi > phi_max) return false;
        }

        // find parametric representation of sphere hit
        /*
        float u = phi / phi_max;
        float theta = acosf( p_hit.z / radius );
        float v = (theta - theta_min) / (theta_max - theta_min);
        */
        
        // these implimentations start at (pg. 102) in pbrt 2nd ed.
        // compute dpdu and dpdv
        // compute dndu and dndv
        // initialize DifferentialGeometry from parametric information

        // update t_hit for quadratic intersection
        *t_hit = thit;

        return true;
    }
    
    // this does not follow the pbrt implementation, but should still work 
    bool doesIntersect(const Ray& ray) const override
    {
        Ray r = world_to_object(ray);

        float A = dot(r.d, r.d);
        float B = 2.0f * (r.d.x*r.o.x + r.d.y*r.o.y + r.d.y*r.o.y);
        float C = (r.o.x*r.o.x + r.o.y*r.o.y + r.o.z*r.o.z) - radius * radius;
        
        float t0, t1;
        if( !rt::solveQuadratic(A, B, C, &t0, &t1) ) return false;
        
        if(t0 > ray.t_max || t1 < ray.t_min) return false;
        
        float thit = t0;
        if(thit < ray.t_min)
        {
            thit = t1;
            if(thit > ray.t_max) return false;
        }
        
        // TODO: either follow pbrt implementation or handle partial spheres correctly here
        
        return true;
    }
};

#endif // SPHERE_H