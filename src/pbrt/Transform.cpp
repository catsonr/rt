#include "Transform.h"

Transform::Transform()
{
    const std::shared_ptr<Mat4> _m = std::make_shared<Mat4>(); // default identity matrix
    
    m = _m;
    m_inv = _m; // inverse of identitiy is identity
}

Transform::Transform(const std::shared_ptr<Mat4> matrix) :
    m(matrix),
    m_inv(matrix->inverse())
{}

Transform::Transform(const std::shared_ptr<Mat4> matrix, const std::shared_ptr<Mat4> matrix_inverse) :
    m(matrix),
    m_inv(matrix_inverse)
{}

Transform::Transform(float matrix[4][4])
{
    m = std::make_shared<Mat4>(
        matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3],
        matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3],
        matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3],
        matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]
    );
    
    m_inv = m->inverse();
}

/* PUBLIC METHODS */
bool Transform::swapsHandedness() const
{
    float det = ((m->m[0][0] *
                  (m->m[1][1] * m->m[2][2] -
                   m->m[1][2] * m->m[2][1])) -
                 (m->m[0][1] *
                  (m->m[1][0] * m->m[2][2] -
                   m->m[1][2] * m->m[2][0])) +
                 (m->m[0][2] *
                  (m->m[1][0] * m->m[2][1] -
                   m->m[1][1] * m->m[2][0])));

    return det < 0.0f;
}

Transform Transform::translate(const Vector& delta)
{
    std::shared_ptr<Mat4> m_new = std::make_shared<Mat4>(
        1, 0, 0, delta.x,
        0, 1, 0, delta.y,
        0, 0, 1, delta.z,
        0, 0, 0, 1
    );

    std::shared_ptr<Mat4> m_new_inv = std::make_shared<Mat4>(
        1, 0, 0, -delta.x,
        0, 1, 0, -delta.y,
        0, 0, 0, -delta.z,
        0, 0, 0, 1
    );
    
    return Transform(m_new, m_new_inv);
}

Transform Transform::scale(float x, float y, float z)
{
    std::shared_ptr<Mat4> m_new = std::make_shared<Mat4>(
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    );
    std::shared_ptr<Mat4> m_new_inv = std::make_shared<Mat4>(
        1.0f / x, 0, 0, 0,
        0, 1.0f / y, 0, 0,
        0, 0, 1.0f / z, 0,
        0, 0, 0, 1
    );
    
    return Transform(m_new, m_new_inv);
}

Transform Transform::rotateX(float angle_radians)
{
    float sin = sinf(angle_radians);
    float cos = cosf(angle_radians);
    
    std::shared_ptr m_rotated = std::make_shared<Mat4>(
        1, 0, 0, 0,
        0, cos, -sin, 0,
        0, sin, cos, 0,
        0, 0, 0, 1
    );
    
    return Transform(m_rotated, m_rotated->transpose());
}
Transform Transform::rotateY(float angle_radians)
{
    float sin = sinf(angle_radians);
    float cos = cosf(angle_radians);
    
    std::shared_ptr m_rotated = std::make_shared<Mat4>(
        cos, 0, sin, 0,
        0, 1, 0, 0,
        -sin, 0, cos, 0,
        0, 0, 0, 1
    );
    
    return Transform(m_rotated, m_rotated->transpose());
}
Transform Transform::rotateZ(float angle_radians)
{
    float sin = sinf(angle_radians);
    float cos = cosf(angle_radians);
    
    std::shared_ptr m_rotated = std::make_shared<Mat4>(
        cos, -sin, 0, 0,
        sin, cos, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
    
    return Transform(m_rotated, m_rotated->transpose());
}

// implementation @ (pg. 75) of pbrt 2nd ed.
Transform Transform::lookAt(const Point& pos, const Point& lookingAt, const Vector& up)
{
    float mat[4][4];
    
    // initialize 4th column
    mat[0][3] = pos.x;
    mat[1][3] = pos.y;
    mat[2][3] = pos.z;
    mat[3][3] = 1;
    
    // initialize columns 1, 2, and 3
    Vector dir = normalize(lookingAt - pos);
    Vector right = cross(dir, normalize(up));
    Vector newUp = cross(right, dir);
    
    mat[0][0] = right.x;
    mat[1][0] = right.y;
    mat[2][0] = right.z;
    mat[3][0] = 0.0f;
    mat[0][1] = newUp.x;
    mat[1][1] = newUp.y;
    mat[2][1] = newUp.z;
    mat[3][1] = 0.0f;
    mat[0][2] = dir.x;
    mat[1][2] = dir.y;
    mat[2][2] = dir.z;
    mat[3][2] = 0.0f;

    Mat4* camera_to_world = new Mat4(mat);
    
    return Transform(camera_to_world->inverse(), std::make_shared<Mat4>(*camera_to_world));
}