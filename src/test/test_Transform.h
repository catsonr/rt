/*
    !!! CHATGPT CODE, LIKE ALL OF IT
*/

#ifndef TEST_TRANSFORM_H
#define TEST_TRANSFORM_H

#include "Transform.h"    // your Transform implementation :contentReference[oaicite:0]{index=0}&#8203;:contentReference[oaicite:1]{index=1}
#include "Point.h"        // Point class :contentReference[oaicite:2]{index=2}&#8203;:contentReference[oaicite:3]{index=3}
#include "Vector.h"       // Vector class and Normal class
#include "Ray.h"          // Ray class
#include "Bbox.h"         // Bbox class
#include <cassert>
#include <cmath>
#include <iostream>

namespace test_transform {
    static constexpr float EPS = 1e-5f;

    inline bool feq(float a, float b) {
        return std::fabs(a - b) <= EPS;
    }

    inline void test_identity() {
        Transform T;  // identity :contentReference[oaicite:4]{index=4}&#8203;:contentReference[oaicite:5]{index=5}
        Point p(1,2,3);
        Vector v(4,5,6);
        // point unchanged
        Point p2 = T(p);
        assert(feq(p2.x, p.x) && feq(p2.y, p.y) && feq(p2.z, p.z));
        // vector unchanged
        Vector v2 = T(v);
        assert(feq(v2.x, v.x) && feq(v2.y, v.y) && feq(v2.z, v.z));
        // inverse of identity is identity
        Transform Ti = T.getInverse();
        Point p3 = Ti(p);
        assert(feq(p3.x, p.x) && feq(p3.y, p.y) && feq(p3.z, p.z));
    }

    inline void test_translate() {
        Vector d(7, -3, 2);
        Transform T = Transform::translate(d);  // translation :contentReference[oaicite:6]{index=6}&#8203;:contentReference[oaicite:7]{index=7}
        Point p(1,1,1);
        Point moved = T(p);
        assert(feq(moved.x, p.x + d.x));
        assert(feq(moved.y, p.y + d.y));
        assert(feq(moved.z, p.z + d.z));
        // vector should not be translated
        Vector v(1,2,3);
        Vector vt = T(v);
        assert(feq(vt.x, v.x) && feq(vt.y, v.y) && feq(vt.z, v.z));
        // composition with inverse yields identity
        Transform Ti = T.getInverse();
        Point back = Ti(moved);
        assert(feq(back.x, p.x) && feq(back.y, p.y) && feq(back.z, p.z));
    }

    inline void test_scale() {
        float sx=2, sy=3, sz=4;
        Transform S = Transform::scale(sx,sy,sz);  // scaling :contentReference[oaicite:8]{index=8}&#8203;:contentReference[oaicite:9]{index=9}
        Point p(1,2,3);
        Point ps = S(p);
        assert(feq(ps.x, p.x * sx));
        assert(feq(ps.y, p.y * sy));
        assert(feq(ps.z, p.z * sz));
        // vector scales likewise
        Vector v(1,1,1);
        Vector vs = S(v);
        assert(feq(vs.x, v.x * sx) && feq(vs.y, v.y * sy) && feq(vs.z, v.z * sz));
        // uniform scale leaves normals unchanged directionally
        Transform Su = Transform::scale(5,5,5);
        Normal n(0,0,1);
        Normal nu = Su(n);
        float invs = 1.0f/5.0f;
        // normal transform uses inverse matrix; direction should remain (0,0,1)
        assert(feq(nu.x, 0) && feq(nu.y, 0) && feq(nu.z, 1));
    }

    inline void test_composition_and_inverse() {
        Transform A = Transform::translate(Vector(1,2,3));
        Transform B = Transform::scale(2,2,2);
        Transform C = A * B;  // compose :contentReference[oaicite:10]{index=10}&#8203;:contentReference[oaicite:11]{index=11}
        // C(p) == A(B(p))
        Point p(1,1,1);
        Point c1 = C(p);
        Point c2 = A(B(p));
        assert(feq(c1.x, c2.x) && feq(c1.y, c2.y) && feq(c1.z, c2.z));
        // inverse of C is B⁻¹ * A⁻¹
        Transform Ci = C.getInverse();
        Point back = Ci(c1);
        assert(feq(back.x, p.x) && feq(back.y, p.y) && feq(back.z, p.z));
    }

    inline void test_swapsHandedness() {
        // negative scale flips handedness
        Transform T1 = Transform::scale(-1,1,1);
        assert(T1.swapsHandedness());
        // uniform positive scale does not
        Transform T2 = Transform::scale(2,2,2);
        assert(!T2.swapsHandedness());
    }

    inline void test_lookAt() {
        Point eye(1,2,3);
        Point target(4,6,3);
        Vector up(0,0,1);
        Transform L = Transform::lookAt(eye, target, up);  // lookAt :contentReference[oaicite:12]{index=12}&#8203;:contentReference[oaicite:13]{index=13}
        // eye should map to origin
        Point p_eye = L(eye);
        assert(feq(p_eye.x, 0) && feq(p_eye.y, 0) && feq(p_eye.z, 0));
        // target maps to (0,0,-d)
        Vector dir = target - eye;
        float d = std::sqrt(dir.x*dir.x + dir.y*dir.y + dir.z*dir.z);
        Point p_t = L(target);
        assert(feq(p_t.x, 0) && feq(p_t.y, 0) && feq(p_t.z, -d));
    }

    inline void test_orthographic() {
        float near=1, far=101;
        Transform O = Transform::orthographic(near, far);  // orthographic :contentReference[oaicite:14]{index=14}&#8203;:contentReference[oaicite:15]{index=15}
        // map near to -1, far to +1 in z
        Point pn(0,0,near), pf(0,0,far);
        Point on = O(pn), of = O(pf);
        assert(feq(on.x, 0) && feq(on.y, 0) && feq(on.z, -1));
        assert(feq(of.x, 0) && feq(of.y, 0) && feq(of.z,  1));
    }

    inline void test_ray_and_bbox() {
        // ray translation
        Ray r(Point(0,0,0), Vector(1,0,0));
        Transform T = Transform::translate(Vector(5,0,0));
        Ray rt = T(r);
        assert(feq(rt.o.x, 5) && feq(rt.o.y, 0) && feq(rt.o.z, 0));
        assert(feq(rt.d.x, 1) && feq(rt.d.y, 0) && feq(rt.d.z, 0));
        // bbox translation
        Bbox b(Point(0,0,0), Point(1,2,3));
        Bbox bt = T(b);
        assert(feq(bt.p_min.x, 5) && feq(bt.p_min.y, 0) && feq(bt.p_min.z, 0));
        assert(feq(bt.p_max.x, 6) && feq(bt.p_max.y, 2) && feq(bt.p_max.z, 3));
    }

    inline void run_all_transform_tests() {
        test_identity();
        test_translate();
        test_scale();
        test_composition_and_inverse();
        test_swapsHandedness();
        test_lookAt();
        test_orthographic();
        test_ray_and_bbox();
        std::cout << "[test_transform] all Transform tests passed\n";
    }
}

#endif // TEST_TRANSFORM_H