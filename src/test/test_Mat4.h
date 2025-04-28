/*
    !!! CHATGPT CODE, LIKE ALL OF IT
*/

#ifndef TEST_MAT4_H
#define TEST_MAT4_H

#include "Mat4.h"
#include <cassert>
#include <cmath>
#include <iostream>

namespace test_mat4 {
    static constexpr float EPS = 1e-6f;

    inline bool feq(float a, float b) {
        return std::fabs(a - b) <= EPS;
    }

    inline void test_default_constructor() {
        Mat4 m;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                assert(feq(m.m[i][j], (i == j) ? 1.0f : 0.0f));
    }

    inline void test_array_constructor() {
        float elems[16] = {
            1,  2,  3,  4,
            5,  6,  7,  8,
            9, 10, 11, 12,
           13, 14, 15, 16
        };
        Mat4 m(elems);
        for (int i = 0; i < 16; ++i) {
            int r = i / 4, c = i % 4;
            assert(feq(m.m[r][c], elems[i]));
        }
    }

    inline void test_transpose() {
        Mat4 m(
            1, 2, 3, 4,
            5, 6, 7, 8,
            9,10,11,12,
           13,14,15,16
        );
        auto mt = m.transpose();
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                assert(feq(mt->m[i][j], m.m[j][i]));
    }

    inline void test_multiply_identity() {
        auto I = std::make_shared<Mat4>();
        auto A = std::make_shared<Mat4>(
            2,3,5,7,
            11,13,17,19,
            23,29,31,37,
            41,43,47,53
        );
        auto left = Mat4::multiply(I, A);
        auto right = Mat4::multiply(A, I);
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j) {
                assert(feq(left->m[i][j], A->m[i][j]));
                assert(feq(right->m[i][j], A->m[i][j]));
            }
    }

    inline void test_inverse_diagonal() {
        auto B = std::make_shared<Mat4>(
            2,0,0,0,
            0,3,0,0,
            0,0,4,0,
            0,0,0,1
        );
        auto Binv = B->inverse();
        // expect 1/diag
        assert(feq(Binv->m[0][0], 0.5f));
        assert(feq(Binv->m[1][1], 1.0f/3.0f));
        assert(feq(Binv->m[2][2], 0.25f));
        assert(feq(Binv->m[3][3], 1.0f));
        // multiplication yields identity
        auto prod = Mat4::multiply(B, Binv);
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                assert(feq(prod->m[i][j], (i==j)?1.0f:0.0f));
    }

    inline void run_all_mat4_tests() {
        test_default_constructor();
        test_array_constructor();
        test_transpose();
        test_multiply_identity();
        test_inverse_diagonal();
        std::cout << "[mat4] all tests passed\n";
    }
}

#endif // TEST_MAT4_H