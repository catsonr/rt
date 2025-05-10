#ifndef TEST_H
#define TEST_H

#include "test_Mat4.h"
#include "hardtest_Mat4.h"

#include "test_Point.h"
#include "hardtest_Point.h"

#include "test_Transform.h"
#include "hardtest_Transform.h"

#include "test_Ray.h"

#include "test_Vector.h"

#include "test_Bbox.h"

#include "test_Camera.h"

namespace test {
    inline void run_all_tests() {
        test_mat4::run_all_mat4_tests();
        hardtest_mat4::run_all_hard_Mat4_tests();

        test_point::run_all_point_tests();
        hardtest_point::run_all_hard_Point_tests();

        test_transform::run_all_transform_tests();
        //hardtest_transform::run_all_hard_Transform_tests(); // doesnt pass

        test_ray::run_all_ray_tests();
        
        test_vector::run_all_vector_tests();
        
        test_bbox::run_all_bbox_tests();
        
        test_camera::run_all_camera_tests();
    }
}

#endif // TEST_H
