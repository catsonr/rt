/*
    a Sample is a 
    a Sampler generates samples at a given (x, y) coordinate in screen space
*/

#ifndef SAMPLE_H
#define SAMPLE_H

class Scene;
class SurfaceIntegrator;
class VolumeIntegrator;

// (pg. 299)
struct Sample
{
    /* PUBLIC MEMBERS */
    // camera sample data
    float image_x, image_y;
    float lens_u, lens_v;
    float time;

    // integrator sample data
    
    /* CONSTRUCTORS */
    Sample(float image_x, float image_y, float lens_u, float lens_v, float time):
        image_x(image_x),
        image_y(image_y),
        lens_u(lens_u),
        lens_v(lens_v),
        time(time)
    {}
    
    Sample(SurfaceIntegrator* surface, VolumeIntegrator* volume, const Scene* scene); // implementation @ (pg. 300) of pbrt 2nd ed.

    /* PUBLIC METHODS */
};

class Sampler
{
public:
    /* PUBLIC MEMBERS */
    int x_start, x_end;
    int y_start, y_end;
    int samplesPerPixel;
    
    /* CONSTRUCTORS */
    Sampler(int x_start, int x_end, int y_start, int y_end, int samplesPerPixel) :
        x_start(x_start),
        x_end(x_end),
        y_start(y_start),
        y_end(y_end),
        samplesPerPixel(samplesPerPixel)
    {}
    
    /* VIRTUAL FUNCTIONS */
    virtual bool getNextSample(Sample* sample) = 0;
    
    /* PUBLIC METHODS*/
    int totalSamples() const
    {
        return samplesPerPixel * (x_end - x_start) * (y_end - y_start);
    }
};

#endif // SAMPLE_H