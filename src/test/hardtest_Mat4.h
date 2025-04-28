/*
    !!! CHATGPT CODE, LIKE ALL OF IT
*/

#ifndef HARDTEST_MAT4_H
#define HARDTEST_MAT4_H

#include "Mat4.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <random>

namespace hardtest_mat4 {

    static constexpr float EPS = 1e-4f;

    inline bool feq(float a, float b) {
        float diff = std::fabs(a - b);
        if (diff <= EPS) return true;
        return diff <= EPS * std::max(std::fabs(a), std::fabs(b));
    }

    // manual 4×4 multiply
    inline Mat4 manualMultiply(const Mat4 &A, const Mat4 &B) {
        Mat4 R;
        // zero out R
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                R.m[i][j] = 0.0f;
        for (int i = 0; i < 4; ++i)
            for (int k = 0; k < 4; ++k)
                for (int j = 0; j < 4; ++j)
                    R.m[i][j] += A.m[i][k] * B.m[k][j];
        return R;
    }

    // test a bunch of random multiplications
    inline void test_random_multiplication(int trials = 10) {
        std::mt19937_64 rng(12345);
        std::uniform_real_distribution<float> dist(-10.0f, 10.0f);
        for (int t = 0; t < trials; ++t) {
            float a[16], b[16];
            for (int i = 0; i < 16; ++i) {
                a[i] = dist(rng);
                b[i] = dist(rng);
            }
            Mat4 A(a), B(b);
            auto Ap = std::make_shared<Mat4>(a);
            auto Bp = std::make_shared<Mat4>(b);
            auto Capi = Mat4::multiply(Ap, Bp);
            Mat4 Cmanual = manualMultiply(A, B);
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    assert(feq(Capi->m[i][j], Cmanual.m[i][j]));
        }
    }

    // (A*B)*C == A*(B*C)
    inline void test_associativity(int trials = 10) {
        std::mt19937_64 rng(54321);
        std::uniform_real_distribution<float> dist(-5.0f, 5.0f);
        for (int t = 0; t < trials; ++t) {
            float a[16], b[16], c[16];
            for (int i = 0; i < 16; ++i) {
                a[i] = dist(rng);
                b[i] = dist(rng);
                c[i] = dist(rng);
            }
            auto Ap = std::make_shared<Mat4>(a);
            auto Bp = std::make_shared<Mat4>(b);
            auto Cp = std::make_shared<Mat4>(c);
            auto AB = Mat4::multiply(Ap, Bp);
            auto AB_C = Mat4::multiply(AB, Cp);
            auto BC = Mat4::multiply(Bp, Cp);
            auto A_BC = Mat4::multiply(Ap, BC);
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    assert(feq(AB_C->m[i][j], A_BC->m[i][j]));
        }
    }

    // transpose twice returns original
    inline void test_transpose_involution(int trials = 10) {
        std::mt19937_64 rng(98765);
        std::uniform_real_distribution<float> dist(-7.0f, 7.0f);
        for (int t = 0; t < trials; ++t) {
            float a[16];
            for (int i = 0; i < 16; ++i) a[i] = dist(rng);
            Mat4 A(a);
            auto At = A.transpose();
            auto Att = At->transpose();
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    assert(feq(Att->m[i][j], A.m[i][j]));
        }
    }

    // (A*B)^T == B^T * A^T
    inline void test_transpose_multiply_commute(int trials = 10) {
        std::mt19937_64 rng(19283);
        std::uniform_real_distribution<float> dist(-4.0f, 4.0f);
        for (int t = 0; t < trials; ++t) {
            float a[16], b[16];
            for (int i = 0; i < 16; ++i) {
                a[i] = dist(rng);
                b[i] = dist(rng);
            }
            auto Ap = std::make_shared<Mat4>(a);
            auto Bp = std::make_shared<Mat4>(b);
            auto C1 = Mat4::multiply(Ap, Bp)->transpose();
            auto Bt = Bp->transpose();
            auto At = Ap->transpose();
            auto C2 = Mat4::multiply(Bt, At);
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    assert(feq(C1->m[i][j], C2->m[i][j]));
        }
    }

    // test inverse on diagonal and random invertible matrices
    inline void test_random_inverse(int trials = 10) {
        std::mt19937_64 rng(55555);
        std::uniform_real_distribution<float> dist(-5.0f, 5.0f);
        for (int t = 0; t < trials; ++t) {
            float a[16];
            // random fill
            for (int i = 0; i < 16; ++i) a[i] = dist(rng);
            // enforce diagonal dominance -> invertible
            for (int i = 0; i < 4; ++i) {
                float rowsum = 0.0f;
                for (int j = 0; j < 4; ++j)
                    rowsum += std::fabs(a[i*4 + j]);
                a[i*4 + i] += rowsum + 1.0f;
            }
            auto Ap = std::make_shared<Mat4>(a);
            auto Ainv = Ap->inverse();
            auto I1 = Mat4::multiply(Ap, Ainv);
            auto I2 = Mat4::multiply(Ainv, Ap);
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    float expected = (i == j) ? 1.0f : 0.0f;
                    assert(feq(I1->m[i][j], expected));
                    assert(feq(I2->m[i][j], expected));
                }
            }
        }
    }

    // multiply by zero matrix gives zero
    inline void test_zero_multiplication() {
        float zeroArr[16] = {0};
        float a[16];
        for (int i = 0; i < 16; ++i) a[i] = static_cast<float>(i + 1);
        auto Ap = std::make_shared<Mat4>(a);
        auto Zp = std::make_shared<Mat4>(zeroArr);
        auto R1 = Mat4::multiply(Ap, Zp);
        auto R2 = Mat4::multiply(Zp, Ap);
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                assert(feq(R1->m[i][j], 0.0f)),
                assert(feq(R2->m[i][j], 0.0f));
    }

    // inverse(identity) == identity
    inline void test_inverse_of_identity() {
        auto Ip = std::make_shared<Mat4>();
        auto Iinv = Ip->inverse();
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j) {
                float expected = (i == j) ? 1.0f : 0.0f;
                assert(feq(Iinv->m[i][j], expected));
            }
    }

    // run ’em all
    inline void run_all_hard_Mat4_tests() {
        test_random_multiplication();
        test_associativity();
        test_transpose_involution();
        test_transpose_multiply_commute();
        test_random_inverse();
        test_zero_multiplication();
        test_inverse_of_identity();
        std::cout << "[hardtest_mat4] all rigorous tests passed\n";
    }
}

#endif // HARDTEST_MAT4_H
