/*
    !!! CHATGPT CODE, LIKE ALL OF IT
*/

#ifndef TEST_CAMERA_H
#define TEST_CAMERA_H

#include "Camera.h"
#include "Transform.h"
#include "Ray.h"
#include "Sample.h"
#include "Vector.h"
#include <cassert>
#include <cmath>
#include <iostream>

namespace test_camera {
    static constexpr float EPS = 1e-6f;

    inline bool feq(float a, float b) {
        if (std::isnan(a) && std::isnan(b)) return true;
        if (std::isinf(a) && std::isinf(b) && (std::signbit(a) == std::signbit(b))) return true;
        return std::fabs(a - b) <= EPS;
    }

    inline void test_orthographic_generateRay() {
        // Identity world_to_camera
        Transform id;
        float screen[4] = {-1.0f, 1.0f, 1.0f, -1.0f}; // left, right, top, bottom
        float clip_near = 0.1f, clip_far = 2.0f;
        float shutter_open = 0.0f, shutter_close = 0.0f;
        float lensRadius = 0.0f, focalDistance = 1.0f;
        OrthographicCamera cam(id, screen, clip_near, clip_far,
                               shutter_open, shutter_close,
                               lensRadius, focalDistance, nullptr);

        Sample s1;
        s1.image_x = 10.0f;
        s1.image_y = 20.0f;
        s1.time = 0.5f;
        Ray ray1;
        float w1 = cam.generateRay(s1, &ray1);
        // weight
        assert(feq(w1, 1.0f));
        // direction should be +Z in world space
        assert(feq(ray1.d.x, 0.0f) && feq(ray1.d.y, 0.0f) && feq(ray1.d.z, 1.0f));
        // t_min and t_max
        assert(feq(ray1.t_min, 0.0f));
        assert(feq(ray1.t_max, clip_far - clip_near));
        // origin depends on sample; run again with different sample
        Sample s2 = s1;
        s2.image_x = 30.0f;
        s2.image_y = 40.0f;
        Ray ray2;
        float w2 = cam.generateRay(s2, &ray2);
        // weight and direction unchanged
        assert(feq(w2, w1));
        assert(feq(ray2.d.x, ray1.d.x) && feq(ray2.d.y, ray1.d.y) && feq(ray2.d.z, ray1.d.z));
        // origins should differ
        bool origin_diff = !(
            feq(ray2.o.x, ray1.o.x) &&
            feq(ray2.o.y, ray1.o.y) &&
            feq(ray2.o.z, ray1.o.z)
        );
        assert(origin_diff);
    }

    inline void run_all_camera_tests() {
        test_orthographic_generateRay();
        std::cout << "[test_camera] all Camera tests passed\n";
    }
}

#endif // TEST_CAMERA_H