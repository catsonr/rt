/*
    !!! CHATGPT CODE, LIKE ALL OF IT
*/

#ifndef TEST_RAY_H
#define TEST_RAY_H

#include "Ray.h"
#include "Point.h"
#include "Vector.h"
#include <cassert>
#include <cmath>
#include <limits>
#include <iostream>

namespace test_ray {
    static constexpr float EPS = 1e-6f;

    inline bool feq(float a, float b) {
        // handle infinities
        if (std::isinf(a) && std::isinf(b) && (std::signbit(a) == std::signbit(b)))
            return true;
        return std::fabs(a - b) <= EPS;
    }

    inline void test_default_constructor() {
        Ray r;
        // origin
        assert(feq(r.o.x, 0.0f) && feq(r.o.y, 0.0f) && feq(r.o.z, 0.0f));
        // direction
        assert(feq(r.d.x, 0.0f) && feq(r.d.y, 0.0f) && feq(r.d.z, 0.0f));
        // t_min should be positive epsilon
        assert(r.t_min > 0.0f);
        // t_max should be infinite
        assert(std::isinf(r.t_max));
        // time
        assert(feq(r.t, 0.0f));
    }

    inline void test_param_constructor() {
        Point p(1.0f, 2.0f, 3.0f);
        Vector v(4.0f, 5.0f, 6.0f);
        float t0 = 0.1f;
        float t1 = 100.0f;
        float t  = 2.5f;
        Ray r(p, v, t0, t1, t);
        assert(feq(r.o.x, p.x) && feq(r.o.y, p.y) && feq(r.o.z, p.z));
        assert(feq(r.d.x, v.x) && feq(r.d.y, v.y) && feq(r.d.z, v.z));
        assert(feq(r.t_min, t0));
        assert(feq(r.t_max, t1));
        assert(feq(r.t, t));
    }

    inline void test_eval_operator() {
        Point p(1.0f, -1.0f, 2.0f);
        Vector v(2.0f, 0.5f, -1.5f);
        Ray r(p, v);
        // r(t) = p + v * t
        float tvals[] = {0.0f, 1.0f, 2.3f, -0.5f};
        for (float tv : tvals) {
            Point expected = Point(
                p.x + v.x * tv,
                p.y + v.y * tv,
                p.z + v.z * tv
            );
            Point got = r(tv);
            assert(feq(got.x, expected.x) && feq(got.y, expected.y) && feq(got.z, expected.z));
        }
    }

    inline void run_all_ray_tests() {
        test_default_constructor();
        test_param_constructor();
        test_eval_operator();
        std::cout << "[test_ray] all Ray tests passed\n";
    }
}

#endif // TEST_RAY_H