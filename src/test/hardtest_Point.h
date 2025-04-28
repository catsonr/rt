/*
    !!! CHATGPT CODE, LIKE ALL OF IT
*/

#ifndef HARDTEST_POINT_H
#define HARDTEST_POINT_H

#include "Point.h"
#include "Vector.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <random>

namespace hardtest_point {
    static constexpr float EPS = 1e-5f;

    inline bool feq(float a, float b) {
        float diff = std::fabs(a - b);
        if (diff <= EPS) return true;
        return diff <= EPS * std::max(std::fabs(a), std::fabs(b));
    }

    // p + v - v == p
    inline void test_random_add_subtract(int trials = 100) {
        std::mt19937 rng(123);
        std::uniform_real_distribution<float> dist(-100.0f, 100.0f);
        for (int i = 0; i < trials; ++i) {
            Point p(dist(rng), dist(rng), dist(rng));
            Vector v(dist(rng), dist(rng), dist(rng));
            Point p1 = p + v;
            Point p2 = p1 - v;
            assert(feq(p2.x, p.x));
            assert(feq(p2.y, p.y));
            assert(feq(p2.z, p.z));
        }
    }

    // (p1 - p2) == -(p2 - p1)
    inline void test_subtract_symmetry(int trials = 100) {
        std::mt19937 rng(456);
        std::uniform_real_distribution<float> dist(-50.0f, 50.0f);
        for (int i = 0; i < trials; ++i) {
            Point p1(dist(rng), dist(rng), dist(rng));
            Point p2(dist(rng), dist(rng), dist(rng));
            Vector v1 = p1 - p2;
            Vector v2 = p2 - p1;
            assert(feq(v1.x, -v2.x));
            assert(feq(v1.y, -v2.y));
            assert(feq(v1.z, -v2.z));
        }
    }

    // compound ops match simple ops
    inline void test_compound_vs_simple(int trials = 100) {
        std::mt19937 rng(789);
        std::uniform_real_distribution<float> dist(-20.0f, 20.0f);
        for (int i = 0; i < trials; ++i) {
            Point p0(dist(rng), dist(rng), dist(rng));
            Vector v(dist(rng), dist(rng), dist(rng));

            Point p1 = p0;
            p1 += v;
            Point p2 = p0 + v;
            assert(feq(p1.x, p2.x));
            assert(feq(p1.y, p2.y));
            assert(feq(p1.z, p2.z));

            p2 -= v;
            assert(feq(p2.x, p0.x));
            assert(feq(p2.y, p0.y));
            assert(feq(p2.z, p0.z));
        }
    }

    // p2 == p1 + (p2 - p1)
    inline void test_point_vector_roundtrip(int trials = 100) {
        std::mt19937 rng(321);
        std::uniform_real_distribution<float> dist(-30.0f, 30.0f);
        for (int i = 0; i < trials; ++i) {
            Point p1(dist(rng), dist(rng), dist(rng));
            Point p2(dist(rng), dist(rng), dist(rng));
            Vector v = p2 - p1;
            Point p_recon = p1 + v;
            assert(feq(p_recon.x, p2.x));
            assert(feq(p_recon.y, p2.y));
            assert(feq(p_recon.z, p2.z));
        }
    }

    // valid indices 0,1,2
    inline void test_index_inbounds(int trials = 100) {
        std::mt19937 rng(654);
        std::uniform_real_distribution<float> dist(-10.0f, 10.0f);
        for (int i = 0; i < trials; ++i) {
            float x = dist(rng), y = dist(rng), z = dist(rng);
            Point p(x, y, z);
            assert(feq(p[0], x));
            assert(feq(p[1], y));
            assert(feq(p[2], z));
        }
    }

    // out‐of‐range returns NAN
    inline void test_index_oob() {
        Point p(1.0f, 2.0f, 3.0f);
        float val = p[42];
        assert(std::isnan(val));
    }

    inline void run_all_hard_Point_tests() {
        test_random_add_subtract();
        test_subtract_symmetry();
        test_compound_vs_simple();
        test_point_vector_roundtrip();
        test_index_inbounds();
        test_index_oob();
        std::cout << "[hardtest_point] all rigorous Point tests passed\n";
    }
}

#endif // HARDTEST_POINT_H
