/*
    the Transform class represents a linear algebra tranformation
    it holds both the matrix of the given transformation as well as its inverse (to transform back)
    
    Transform can transform Points, Vectors, Normals, Rays, and Bboxes
    
    it supports translation, scaling, rotation (coming soon), and lookAt matrix generation
*/

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <memory>

#include "Mat4.h"
#include "Ray.h"
#include "Bbox.h"

class Transform
{
public:
    /* PUBLIC MEMBERS */
    std::shared_ptr<Mat4> m, m_inv;
    
    /* CONSTRUCTORS */
    Transform();
    Transform(const std::shared_ptr<Mat4> matrix);
    Transform(const std::shared_ptr<Mat4> matrix, const std::shared_ptr<Mat4> matrix_inverse);
    Transform(float matrix[4][4]);
    
    /* PUBLIC METHODS */
    // returns true if the transformation causes the resulting matrix to swap handedness
    bool swapsHandedness() const;
    
    static Transform translate(const Vector& delta);
    static Transform scale(float x, float y, float z);
    static Transform rotateX(float angle);
    static Transform rotateY(float angle);
    static Transform rotateZ(float angle);
    //Transform rotate(const Vector& axis, float angle); // implementation @ (pg. 74) of pbrt 2nd ed.
    static Transform lookAt(const Point& pos, const Point& lookingAt, const Vector& up);
    static Transform orthographic(float clip_near, float clip_far);
    
    /* INLINE OPERATOR OVERLOADS */
    inline Transform operator*(const Transform& t2) const // transform composition (multiplication)
    {
        std::shared_ptr<Mat4> m1 = Mat4::multiply(m, t2.m);
        std::shared_ptr<Mat4> m2 = Mat4::multiply(t2.m_inv, m_inv);
        
        return Transform(m1, m2);
    }

    inline Point operator()(const Point& p) const // transforming a point
    {
        float x = m->m[0][0]*p.x + m->m[0][1]*p.y + m->m[0][2]*p.z + m->m[0][3];
        float y = m->m[1][0]*p.x + m->m[1][1]*p.y + m->m[1][2]*p.z + m->m[1][3];
        float z = m->m[2][0]*p.x + m->m[2][1]*p.y + m->m[2][2]*p.z + m->m[2][3];
        float w = m->m[3][0]*p.x + m->m[3][1]*p.y + m->m[3][2]*p.z + m->m[3][3];
        
        assert(w != 0);
        if(w == 1.0f)
            return Point(x, y, z);
        else
            return Point(x/w, y/w, z/w);
    }
    inline void operator()(const Point& p, Point* p_transformed) const // transforming a point in place
    {
        p_transformed->x = m->m[0][0]*p.x + m->m[0][1]*p.y + m->m[0][2]*p.z + m->m[0][3];
        p_transformed->y = m->m[1][0]*p.x + m->m[1][1]*p.y + m->m[1][2]*p.z + m->m[1][3];
        p_transformed->z = m->m[2][0]*p.x + m->m[2][1]*p.y + m->m[2][2]*p.z + m->m[2][3];
        float w = m->m[3][0]*p.x + m->m[3][1]*p.y + m->m[3][2]*p.z + m->m[3][3];
        
        if(w != 1.0f)
        {
            p_transformed->x /= w;
            p_transformed->y /= w;
            p_transformed->z /= w;
        }
    }
    
    inline Vector operator()(const Vector& v) const // transforming a vector 
    {
        return Vector(
            m->m[0][0]*v.x + m->m[0][1]*v.y + m->m[0][2]*v.z + m->m[0][3],
            m->m[1][0]*v.x + m->m[1][1]*v.y + m->m[1][2]*v.z + m->m[1][3],
            m->m[2][0]*v.x + m->m[2][1]*v.y + m->m[2][2]*v.z + m->m[2][3]
        );
    }
    inline void operator()(const Vector& v, Vector* v_transformed) const // transforming a vector in place 
    {
        v_transformed->x = m->m[0][0]*v.x + m->m[0][1]*v.y + m->m[0][2]*v.z + m->m[0][3];
        v_transformed->y = m->m[1][0]*v.x + m->m[1][1]*v.y + m->m[1][2]*v.z + m->m[1][3];
        v_transformed->z = m->m[2][0]*v.x + m->m[2][1]*v.y + m->m[2][2]*v.z + m->m[2][3];
    }
    
    inline Normal operator()(const Normal& n) const // transforming a normal
    {
        return Normal(
            m_inv->m[0][0]*n.x + m_inv->m[1][0]*n.y + m_inv->m[2][0]*n.z,
            m_inv->m[0][1]*n.x + m_inv->m[1][1]*n.y + m_inv->m[2][1]*n.z,
            m_inv->m[0][2]*n.x + m_inv->m[1][2]*n.y + m_inv->m[2][2]*n.z
        );
    }
    inline void operator()(const Normal &n, Normal* n_transformed) const // transforming a normal in place
    {
        n_transformed->x = m_inv->m[0][0]*n.x + m_inv->m[0][1]*n.y + m_inv->m[0][2]*n.z + m_inv->m[0][3];
        n_transformed->y = m_inv->m[1][0]*n.x + m_inv->m[1][1]*n.y + m_inv->m[1][2]*n.z + m_inv->m[1][3];
        n_transformed->z = m_inv->m[2][0]*n.x + m_inv->m[2][1]*n.y + m_inv->m[2][2]*n.z + m_inv->m[2][3];
    }
    
    inline Ray operator()(const Ray& ray) const // transforming a ray
    {
        Ray ret;
        (*this)(ray.o, &ret.o);
        (*this)(ray.d, &ret.d);
        
        ret.t_min = ray.t_min;
        ret.t_max = ray.t_max;
        ret.t = ray.t;

        return ret;
    }
    inline void operator()(const Ray& ray, Ray* ray_transformed) const
    {
        ray_transformed->o = (*this)(ray.o);
        ray_transformed->d = (*this)(ray.d);
        
        ray_transformed->t_min = ray.t_min;
        ray_transformed->t_max = ray.t_max;
        ray_transformed->t = ray.t;
    }
    
    inline Bbox operator()(const Bbox& box) const // transforming a bbox
    {
        const Transform& M = *this;
        
        Bbox ret(            M(Point(box.p_min.x, box.p_min.y, box.p_min.z)));
        ret = ret.Union(ret, M(Point(box.p_max.x, box.p_min.y, box.p_min.z)));
        ret = ret.Union(ret, M(Point(box.p_min.x, box.p_max.y, box.p_min.z)));
        ret = ret.Union(ret, M(Point(box.p_min.x, box.p_min.y, box.p_max.z)));
        ret = ret.Union(ret, M(Point(box.p_min.x, box.p_max.y, box.p_max.z)));
        ret = ret.Union(ret, M(Point(box.p_max.x, box.p_max.y, box.p_min.z)));
        ret = ret.Union(ret, M(Point(box.p_max.x, box.p_min.y, box.p_max.z)));
        ret = ret.Union(ret, M(Point(box.p_max.x, box.p_max.y, box.p_max.z)));
        
        return ret;
    }
};

#endif // TRANSFORM_H