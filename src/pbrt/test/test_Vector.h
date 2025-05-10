/*
    !!! CHATGPT CODE, LIKE ALL OF IT
*/

#ifndef TEST_VECTOR_H
#define TEST_VECTOR_H

#include "Vector.h"
#include <cassert>
#include <cmath>
#include <limits>
#include <iostream>

namespace test_vector {
    static constexpr float EPS = 1e-6f;

    inline bool feq(float a, float b) {
        // handle NaN equality
        if (std::isnan(a) && std::isnan(b)) return true;
        // handle infinities
        if (std::isinf(a) && std::isinf(b) && (std::signbit(a) == std::signbit(b)))
            return true;
        return std::fabs(a - b) <= EPS;
    }

    inline void test_default_constructor() {
        Vector v;
        assert(feq(v.x, 0.0f) && feq(v.y, 0.0f) && feq(v.z, 0.0f));
        Normal n;
        assert(feq(n.x, 0.0f) && feq(n.y, 0.0f) && feq(n.z, 0.0f));
    }

    inline void test_parameter_constructor() {
        Vector v(1.0f, -2.0f, 3.5f);
        assert(feq(v.x, 1.0f) && feq(v.y, -2.0f) && feq(v.z, 3.5f));
        Normal n(-4.0f, 5.0f, -6.5f);
        assert(feq(n.x, -4.0f) && feq(n.y, 5.0f) && feq(n.z, -6.5f));
    }

    inline void test_length() {
        Vector v(1.0f, 2.0f, 2.0f);
        assert(feq(v.lengthSquared(), 9.0f));
        assert(feq(v.length(), 3.0f));

        Normal n(0.0f, 3.0f, 4.0f);
        assert(feq(n.lengthSquared(), 25.0f));
        assert(feq(n.length(), 5.0f));
    }

    inline void test_add_subtract() {
        Vector a(1, 2, 3), b(4, 5, 6);
        Vector sum = a + b;
        assert(feq(sum.x, 5) && feq(sum.y, 7) && feq(sum.z, 9));
        Vector diff = b - a;
        assert(feq(diff.x, 3) && feq(diff.y, 3) && feq(diff.z, 3));
        Vector c(1, 1, 1);
        c += Vector(2, 3, 4);
        assert(feq(c.x, 3) && feq(c.y, 4) && feq(c.z, 5));
        c -= Vector(1, 1, 1);
        assert(feq(c.x, 2) && feq(c.y, 3) && feq(c.z, 4));

        Normal na(1, 2, 3), nb(4, 5, 6);
        Normal nsum = na + nb;
        assert(feq(nsum.x, 5) && feq(nsum.y, 7) && feq(nsum.z, 9));
        Normal ndiff = nb - na;
        assert(feq(ndiff.x, 3) && feq(ndiff.y, 3) && feq(ndiff.z, 3));
        Normal nc(1, 1, 1);
        nc += Normal(2, 3, 4);
        assert(feq(nc.x, 3) && feq(nc.y, 4) && feq(nc.z, 5));
        nc -= Normal(1, 1, 1);
        assert(feq(nc.x, 2) && feq(nc.y, 3) && feq(nc.z, 4));
    }

    inline void test_scalar_mul_div() {
        Vector v(1, -2, 3);
        Vector vm = v * 2.0f;
        assert(feq(vm.x, 2) && feq(vm.y, -4) && feq(vm.z, 6));
        vm *= 0.5f;
        assert(feq(vm.x, 1) && feq(vm.y, -2) && feq(vm.z, 3));
        Vector vd = v / 2.0f;
        assert(feq(vd.x, 0.5f) && feq(vd.y, -1.0f) && feq(vd.z, 1.5f));
        v /= 1.0f;
        assert(feq(v.x, 1) && feq(v.y, -2) && feq(v.z, 3));

        Normal n(2, -4, 6);
        Normal nm = n * 0.5f;
        assert(feq(nm.x, 1) && feq(nm.y, -2) && feq(nm.z, 3));
        nm *= 2.0f;
        assert(feq(nm.x, 2) && feq(nm.y, -4) && feq(nm.z, 6));
        Normal nd = nm / 2.0f;
        assert(feq(nd.x, 1) && feq(nd.y, -2) && feq(nd.z, 3));
        nd /= 1.0f;
        assert(feq(nd.x, 1) && feq(nd.y, -2) && feq(nd.z, 3));
    }

    inline void test_unary_neg() {
        Vector v(1, -2, 3);
        Vector vn = -v;
        assert(feq(vn.x, -1) && feq(vn.y, 2) && feq(vn.z, -3));
        Normal n(-1, 2, -3);
        Normal nn = -n;
        assert(feq(nn.x, 1) && feq(nn.y, -2) && feq(nn.z, 3));
    }

    inline void test_index_operator() {
        Vector v(7.7f, 8.8f, 9.9f);
        assert(feq(v[0], 7.7f));
        assert(feq(v[1], 8.8f));
        assert(feq(v[2], 9.9f));
        float oob = v[42];
        assert(std::isnan(oob));
    }

    inline void test_dot_cross_normalize() {
        Vector a(1, 0, 0), b(0, 1, 0);
        assert(feq(dot(a, b), 0.0f));
        assert(feq(dot(b, a), 0.0f));
        assert(feq(dotAbs(a, b), 0.0f));
        Vector c(1, 2, 3);
        Vector cr = cross(a, b);
        assert(feq(cr.x, 0) && feq(cr.y, 0) && feq(cr.z, 1));
        Vector cn = normalize(c);
        assert(feq(cn.length(), 1.0f));
        // direction preserved
        float cosTheta = dot(cn, c / c.length());
        assert(std::fabs(cosTheta) >= 1.0f - 1e-6f);
    }

    inline void test_conversion() {
        Normal n(1, 2, 3);
        Vector vn(n);
        assert(feq(vn.x, 1) && feq(vn.y, 2) && feq(vn.z, 3));
        Vector v(4, 5, 6);
        Normal nv(v);
        assert(feq(nv.x, 4) && feq(nv.y, 5) && feq(nv.z, 6));
    }

    inline void run_all_vector_tests() {
        test_default_constructor();
        test_parameter_constructor();
        test_length();
        test_add_subtract();
        test_scalar_mul_div();
        test_unary_neg();
        test_index_operator();
        test_dot_cross_normalize();
        test_conversion();
        std::cout << "[test_vector] all Vector/Normal tests passed\n";
    }
}

#endif // TEST_VECTOR_H