#ifndef RTIOW_H
#define RTIOW_H

#include <vector>

#include "pbrt.h"

class Shape;
#include "Camera.h"

struct RayTracingInOneWeekend
{
public:
    /* PUBLIC MEMBERS */
    unsigned int width { rt::CANVAS_WIDTH };
    unsigned int height { rt::CANVAS_HEIGHT };

    std::vector<Shape>* shapes { nullptr };
    std::vector<u_int32_t> texturepixels{ width * height };
    float t { 0.0f };
    
    //ProjectiveCamera camera;
    
    /* CONSTRUCTORS */
    RayTracingInOneWeekend()
    {}
    RayTracingInOneWeekend(std::vector<Shape>* shapes) :
        shapes(shapes)
    {}
    
    /* PUBLIC FUNCTIONS */
    void tick(float dt)
    {
        t += dt;
    }
};

// draws whatever frame is defined by rtiow
inline void draw(RayTracingInOneWeekend& rtiow)
{

}

#endif