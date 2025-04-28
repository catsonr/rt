/*
    MathExam.h contains a single function test(), that tests to see if the pbrt classes produce results that are expected of their real world counterparts
    e.g., Vector(1, 2, 3) - Vector(1, 1, 1) = (0, 1, 2)
*/

#ifndef MATHEXAM_H
#define MATHEXAM_H

#include "Point.h"
#include "Vector.h"
#include "Mat4.h"

namespace me
{
    void test()
    {
        printf("math exam!\n");

        Point p1(1, 2, 3);
        Vector v1(1, 1, 1);
        
        printf("p1: (%f %f %f)\n", p1.x, p1.y, p1.z);
        printf("v1: (%f %f %f)\n", v1.x, v1.y, v1.z);
        
        Point p2 = p1 + v1;
        
        printf("p1 + v1 = (%f %f %f)\n", p2.x, p2.y, p2.z);
        printf("EXPECTED VALUE: (2, 3, 4)\n\n");
        
        v1 *= 4;
        
        printf("4 * v1 = (%f %f %f)\n", v1.x, v1.y, v1.z);
        printf("EXPECTED VALUE: (4, 4, 4)\n\n");
        
        Normal n1(v1);
        n1 = normalize(n1);

        printf("v1 => normal, then normalized = (%f %f %f)\n", n1.x, n1.y, n1.z);
        printf("EXPECTED VALUE: (0.577350, 0.577350, 0.577350)\n\n");
        
        Normal n2 = n1 * 6.5;
        
        printf("n2 = 6.5 * n1 = (%f %f %f)\n", n2.x, n2.y, n2.z);
        printf("EXPECTED VALUE: (3.75278, 3.75278, 3.75278)\n\n");
        
        Mat4 m1;
        
        m1.print();
        printf("matrix m1 initialized, EXPECTED VALUE: identity matrix\n\n");
        
        Mat4 m2 = *m1.inverse();
        
        m2.print();
        printf("m2 = inverse of m1, EXPECTED VALUE: still identity matrix\n\n");
        
    }
} // me

#endif // MATHEXAM_H