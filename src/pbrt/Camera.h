/*
    Camera is an abstract base class that all camera types inherit from
    ProjectiveCamera is an abstract base class that all projective cameras inherit from (namely, orthographic and perspective cameras)
*/

#include "Transform.h"
#include "Sample.h"
class Ray;
class Film;

#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
protected:
    /* PROTECTED MEMBERS */
    Transform world_to_camera, camera_to_world;
    float clip_near, clip_far;
    float shutter_open, shutter_close;
    Film* film;
    
public:
    /* CONSTRUCTORS */
    Camera(const Transform& world_to_camera, float clip_near, float clip_far, float shutter_open = 0.0f, float shutter_close = 0.0f, Film* film = nullptr) :
        world_to_camera(world_to_camera),
        camera_to_world(world_to_camera.getInverse()),
        clip_near(clip_near),
        clip_far(clip_far),
        shutter_open(shutter_open),
        shutter_close(shutter_close),
        film(film)
    {}

    /* VIRTUAL METHODS */
    virtual float generateRay(Sample& sample, Ray* ray) const = 0;
};

class ProjectiveCamera : public Camera
{
protected:
    /* PROTECTEd MEMBERS */    
    Transform camera_to_screen, world_to_screen, raster_to_camera;
    Transform screen_to_raster, raster_to_screen;
    float lensRadius, focalDistance;

public:
    /* CONSTRUCTORS */
    ProjectiveCamera(
        const Transform& world_to_camera,
        const Transform& projection,
        const float screen[4],
        float clip_near,
        float clip_far,
        float shutter_open,
        float shutter_close,
        float lensRadius,
        float focalDistance,
        Film* film = nullptr
    ) :
        Camera(world_to_camera, clip_near, clip_far, shutter_open, shutter_close, film),
        lensRadius(lensRadius),
        focalDistance(focalDistance)
    {
        // compute projective camera transformations
        camera_to_screen = projection;
        world_to_screen = camera_to_screen * world_to_camera;

        // compute projective camera screen transformations
        // screen_to_raster should actually be based on the Film's xResolution and yResolution per (pg. 260) in pbrt 2nd ed.
        screen_to_raster = Transform::scale( (float)rt::CANVAS_WIDTH, (float)rt::CANVAS_HEIGHT, 1.0f ) *
                        Transform::scale( 1.0f / (screen[1] - screen[0]), 1.0f / (screen[2] - screen[3]), 1.0f ) *
                        Transform::translate( Vector(-screen[0], -screen[3], 0.0f) );
        raster_to_screen = screen_to_raster.getInverse();
        
        raster_to_camera = camera_to_screen.getInverse() * raster_to_screen;
    }
};

class OrthographicCamera : public ProjectiveCamera
{
public:
    OrthographicCamera(const Transform& world_to_camera,
        const float screen[4], float clip_near, float clip_far,
        float shutter_open, float shutter_close, float lensRadius, float focalDistance,
        Film* film = nullptr
    ):
        ProjectiveCamera(world_to_camera, Transform::orthographic(clip_near, clip_far), screen, clip_near, clip_far, shutter_open, shutter_close, lensRadius, focalDistance, film)
    {}
    
    // implementation @ (pg. 264) of pbrt 2nd ed.
    float generateRay(Sample& sample, Ray* ray) const override
    {
        // generate raster and camera samples
        Point p_ras(sample.image_x, sample.image_y, 0);
        Point p_camera;
        raster_to_camera(p_ras, &p_camera);
        
        ray->o = p_camera;
        ray->d = Vector(0, 0, 1);

        // set ray time value
        ray->t = rt::lerp(sample.time, shutter_open, shutter_close);

        // modify ray for depth of field @ (pg. 269) of pbrt 2nd ed.
        if(lensRadius > 0.0f)
        {
            // sample point on lens
            // compute point on plane of focus
            // update ray for effect of lens
        }

        ray->t_min = 0.0f;
        ray->t_max = clip_far - clip_near;
        ray->d = normalize(ray->d);
        camera_to_world(*ray, ray);

        return 1.0f;
    }
};

#endif // CAMERA_H