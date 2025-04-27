/*
    The Mat4 class is a representation of a 4x4 matrix
    these matrices are members to the Transform class, which holds a mat4 m and it's inverse, m_inv
*/

#ifndef MAT4_H
#define MAT4_H

#include <stdio.h>
#include <memory>

class Mat4
{
private:
    /* PRIVATE MEMBERS */
    static constexpr int ROWS = 4;
    static constexpr int COLS = 4;
    static constexpr int ELEMS = ROWS * COLS;
    
public:
    /* PUBLIC MEMBERS */
    float m[ROWS][COLS];
    
    /* CONSTRUCTORS */
    Mat4();
    Mat4(const float (&matrix)[ELEMS]);
    Mat4(const float (&matrix)[ROWS][COLS]);
    Mat4(
        float t00, float t01, float t02, float t03,
        float t10, float t11, float t12, float t13,
        float t20, float t21, float t22, float t23,
        float t30, float t31, float t32, float t33
    );
    
    /* PUBLIC METHODS */
    std::shared_ptr<Mat4> transpose() const;
    std::shared_ptr<Mat4> multiply(const std::shared_ptr<Mat4> m1, const std::shared_ptr<Mat4> m2) const;
    std::shared_ptr<Mat4> inverse() const;
    void print() const;
};

#endif // MAT4_H