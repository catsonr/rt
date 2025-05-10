/*
    !!! CHATGPT CODE, LIKE ALL OF IT
*/

#ifndef TEST_BBOX_H
#define TEST_BBOX_H

#include "Bbox.h"
#include "Point.h"
#include "Vector.h"
#include "Ray.h"
#include <cassert>
#include <cmath>
#include <limits>
#include <iostream>

namespace test_bbox {
    static constexpr float EPS = 1e-6f;

    inline bool feq(float a, float b) {
        if (std::isinf(a) && std::isinf(b) && (std::signbit(a) == std::signbit(b)))
            return true;
        return std::fabs(a - b) <= EPS;
    }

    inline void test_default_constructor() {
        Bbox b;
        // default is inverted: no point contained
        assert(!b.containsPoint(Point(0,0,0)));
        // overlaps with nothing
        Bbox c(Point(0,0,0));
        assert(!b.overlaps(c));
        // volume is negative infinite times zero => implementation‐defined; skip volume test here
    }

    inline void test_point_constructor() {
        Point p(1.f, 2.f, 3.f);
        Bbox b(p);
        // both min and max equal p
        assert(feq(b.p_min.x, p.x) && feq(b.p_min.y, p.y) && feq(b.p_min.z, p.z));
        assert(feq(b.p_max.x, p.x) && feq(b.p_max.y, p.y) && feq(b.p_max.z, p.z));
        assert(b.containsPoint(p));
        assert(b.volume() == 0.0f);
        // maximumExtent: all extents zero, should pick z (2)
        assert(b.maximumExtent() == 2);
    }

    inline void test_two_point_constructor_and_contains() {
        Point p1(1, 4, -2);
        Point p2(-3, 0, 5);
        Bbox b(p1, p2);
        // p_min = (-3,0,-2), p_max=(1,4,5)
        assert(feq(b.p_min.x, -3) && feq(b.p_min.y, 0) && feq(b.p_min.z, -2));
        assert(feq(b.p_max.x,  1) && feq(b.p_max.y, 4) && feq(b.p_max.z,  5));
        // contains corner and interior
        assert(b.containsPoint(p1));
        assert(b.containsPoint(p2));
        assert(b.containsPoint(Point(0,2,1)));
        assert(!b.containsPoint(Point(2,2,1)));
    }

    inline void test_overlaps() {
        Bbox a(Point(0,0,0), Point(2,2,2));
        Bbox b(Point(1,1,1), Point(3,3,3));
        Bbox c(Point(3,3,3), Point(4,4,4));
        assert(a.overlaps(b));
        assert(b.overlaps(a));
        assert(!a.overlaps(c));
        assert(!c.overlaps(a));
    }

    inline void test_intersectsP() {
        Bbox b(Point(0,0,0), Point(1,1,1));
        // ray through center
        Ray r1(Point(-1,0.5f,0.5f), Vector(1,0,0));
        float t0, t1;
        assert(b.intersectsP(r1, &t0, &t1));
        assert(feq(t0, 1.0f) && feq(t1, 2.0f));

        // ray misses
        Ray r2(Point(-1,2,0.5f), Vector(1,0,0));
        assert(!b.intersectsP(r2, &t0, &t1));

        // ray origin inside box
        Ray r3(Point(0.5f,0.5f,0.5f), Vector(0,1,0));
        assert(b.intersectsP(r3, &t0, &t1));
        assert(feq(t0, r3.t_min));  // entry is t_min
    }

    inline void test_volume_and_extent() {
        Bbox b(Point(0,0,0), Point(1,2,3));
        assert(feq(b.volume(), 6.0f));
        // extents: x=1,y=2,z=3 => max is z (2)
        assert(b.maximumExtent() == 2);
    }

    inline void test_union_and_expand() {
        Bbox a(Point(0,0,0), Point(1,1,1));
        // Union with point outside
        Bbox u1 = Bbox::Union(a, Point(-1,2,0.5f));
        assert(feq(u1.p_min.x, -1) && feq(u1.p_min.y, 0) && feq(u1.p_min.z, 0));
        assert(feq(u1.p_max.x,  1) && feq(u1.p_max.y, 2) && feq(u1.p_max.z, 1));

        // Union with another box
        Bbox b2(Point(0.5f,-1,0), Point(2,0,0.5f));
        Bbox u2 = Bbox::Union(a, b2);
        assert(feq(u2.p_min.x, 0) && feq(u2.p_min.y, -1) && feq(u2.p_min.z, 0));
        assert(feq(u2.p_max.x, 2) && feq(u2.p_max.y,  1) && feq(u2.p_max.z, 1));

        // expand
        Bbox e = a;
        e.expand(0.5f);
        assert(feq(e.p_min.x, -0.5f) && feq(e.p_min.y, -0.5f) && feq(e.p_min.z, -0.5f));
        assert(feq(e.p_max.x,  1.5f) && feq(e.p_max.y,  1.5f) && feq(e.p_max.z,  1.5f));
    }

    inline void run_all_bbox_tests() {
        test_default_constructor();
        test_point_constructor();
        test_two_point_constructor_and_contains();
        test_overlaps();
        test_intersectsP();
        test_volume_and_extent();
        test_union_and_expand();
        std::cout << "[test_bbox] all Bbox tests passed\n";
    }
}

#endif // TEST_BBOX_H