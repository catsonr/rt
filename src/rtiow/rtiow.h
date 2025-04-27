#ifndef RTIOW_H
#define RTIOW_H

#include <vector>

#include "pbrt.h"

class Shape;
#include "Camera.h"
#include "Sample.h"

struct RayTracingInOneWeekend
{
public:
    /* PUBLIC MEMBERS */
    unsigned int width { rt::CANVAS_WIDTH };
    unsigned int height { rt::CANVAS_HEIGHT };

    std::vector<Shape>* shapes { nullptr };
    std::vector<u_int32_t> texturepixels{ width * height };
    float t { 0.0f };
    
    //OrthographicCamera camera;
    StratifiedSampler sampler{ 0, static_cast<int>(width), 0, static_cast<int>(height) };
    
    /* CONSTRUCTORS */
    RayTracingInOneWeekend()
    {}
    
    /* PUBLIC FUNCTIONS */
    void tick(float dt)
    {
        t += dt;

        printf("RTIOW: ticked time by %f ms\n", dt);
    }
    
    // draws whatever frame is defined by rtiow
    inline void draw()
    {
        printf("RTIOW: drawing frame ...\n");
    }
};

#endif