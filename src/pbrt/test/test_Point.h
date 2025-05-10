/*
    !!! CHATGPT CODE, LIKE ALL OF IT
*/

#ifndef TEST_POINT_H
#define TEST_POINT_H

#include "Point.h"
#include "Vector.h"
#include <cassert>
#include <cmath>
#include <iostream>

namespace test_point {
    static constexpr float EPS = 1e-6f;

    inline bool feq(float a, float b) {
        return std::fabs(a - b) <= EPS;
    }

    inline void test_default_constructor() {
        Point p;
        assert(feq(p.x, 0.0f));
        assert(feq(p.y, 0.0f));
        assert(feq(p.z, 0.0f));
    }

    inline void test_parameter_constructor() {
        Point p(1.1f, -2.2f, 3.3f);
        assert(feq(p.x, 1.1f));
        assert(feq(p.y, -2.2f));
        assert(feq(p.z, 3.3f));
    }

    inline void test_add_vector() {
        Point p(1,2,3);
        Vector v(4,5,6);
        Point sum = p + v;
        assert(feq(sum.x, 5.0f));
        assert(feq(sum.y, 7.0f));
        assert(feq(sum.z, 9.0f));

        // algebra: (p + v) - v == p
        Point back = sum - v;
        assert(feq(back.x, p.x));
        assert(feq(back.y, p.y));
        assert(feq(back.z, p.z));
    }

    inline void test_subtract_vector() {
        Point p(7,8,9);
        Vector v(1,2,3);
        Point diff = p - v;
        assert(feq(diff.x, 6.0f));
        assert(feq(diff.y, 6.0f));
        assert(feq(diff.z, 6.0f));

        // compound -=
        diff -= v;
        assert(feq(diff.x, 5.0f));
        assert(feq(diff.y, 4.0f));
        assert(feq(diff.z, 3.0f));
    }

    inline void test_compound_add() {
        Point p(0,0,0);
        Vector v(2,4,6);
        p += v;
        assert(feq(p.x, 2.0f));
        assert(feq(p.y, 4.0f));
        assert(feq(p.z, 6.0f));
    }

    inline void test_point_minus_point() {
        Point p1(1,3,5);
        Point p2(4,2,0);
        Vector v = p2 - p1;
        assert(feq(v.x,  3.0f)); // 4-1
        assert(feq(v.y, -1.0f)); // 2-3
        assert(feq(v.z, -5.0f)); // 0-5

        // check that adding that vector to p1 yields p2
        Point p1moved = p1 + v;
        assert(feq(p1moved.x, p2.x));
        assert(feq(p1moved.y, p2.y));
        assert(feq(p1moved.z, p2.z));
    }

    inline void test_index_operator() {
        Point p(9.9f, 8.8f, 7.7f);
        assert(feq(p[0], 9.9f));
        assert(feq(p[1], 8.8f));
        assert(feq(p[2], 7.7f));

        float oob = p[99];
        assert(std::isnan(oob)); // out‐of‐range returns NAN
    }

    inline void run_all_point_tests() {
        test_default_constructor();
        test_parameter_constructor();
        test_add_vector();
        test_subtract_vector();
        test_compound_add();
        test_point_minus_point();
        test_index_operator();
        std::cout << "[test_point] all Point tests passed\n";
    }
}

#endif // TEST_POINT_H
