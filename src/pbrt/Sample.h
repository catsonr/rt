/*
    a Sample is a 
    a Sampler generates samples at a given (x, y) coordinate in screen space
*/

#ifndef SAMPLE_H
#define SAMPLE_H

#include <vector>

#include "pbrt.h"

class Scene;
class SurfaceIntegrator;
class VolumeIntegrator;

// (pg. 299)
struct Sample
{
    /* PUBLIC MEMBERS */
    // camera sample data
    float image_x, image_y; // (x, y) coordinates in raster space
    float lens_u, lens_v; // (u, v) coordinates of the lens
    float time;

    // integrator sample data @ (pg. 301) of pbrt 2nd ed.
    std::vector<u_int> n1D, n2D;
    float **oneD, **twoD;
    
    /* CONSTRUCTORS */
    Sample(float image_x, float image_y, float lens_u, float lens_v, float time):
        image_x(image_x),
        image_y(image_y),
        lens_u(lens_u),
        lens_v(lens_v),
        time(time)
    {}
    
    // not implemented yet
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
    virtual int roundSize(int size) const = 0;
    
    /* PUBLIC METHODS*/
    int totalSamples() const
    {
        return samplesPerPixel * (x_end - x_start) * (y_end - y_start);
    }
};

class StratifiedSampler : public Sampler
{
private:
    /* PRIVATE MEMBERS */
    int x_pixelSamples, y_pixelSamples;
    bool jitter;
    int x_pos, y_pos;
    int sample_pos;
    
    float *imageSamples, *lensSamples, *timeSamples;
    
    /* PRIVATE METHODS */
    // generate stratified camera samples for (x_pos, y_pos) @ (pg. 308) of pbrt 2nd ed.
    void generateStratifiedCameraSamples()
    {
        rt::stratifiedSample2D(imageSamples, x_pixelSamples, y_pixelSamples, jitter);
        rt::stratifiedSample2D(lensSamples, x_pixelSamples, y_pixelSamples, jitter);
        rt::stratifiedSample1D(timeSamples, x_pixelSamples * y_pixelSamples, jitter);
    }

public:
    /* CONSTRUCTORS */
    StratifiedSampler(int x_start, int x_end, int y_start, int y_end, int x_pixelSamples = 1, int y_pixelSamples = 1, bool jitter = false) :
        Sampler(x_start, y_end, y_start, y_end, x_pixelSamples * y_pixelSamples),
        jitter(jitter),
        x_pos(x_start),
        y_pos(y_start),
        x_pixelSamples(x_pixelSamples),
        y_pixelSamples(y_pixelSamples)
    {
        // allocate storage for a pixel's worth of stratified samples @ (pg. 306) of pbrt 2nd ed.
        imageSamples = (float*)rt::allocAligned(5 * x_pixelSamples * y_pixelSamples * sizeof(float));
        lensSamples = imageSamples + 2 * x_pixelSamples * y_pixelSamples;
        timeSamples = lensSamples + 2 * x_pixelSamples * y_pixelSamples;

        // generate stratified camera samples for (x_pos, y_pos)
        generateStratifiedCameraSamples();
        
        // shift stratified image samples to pixel coordinates
        for(int o = 0; o < 2 * x_pixelSamples * y_pixelSamples; o += 2)
        {
            imageSamples[o]     += x_pos;
            imageSamples[o + 1] += y_pos;
        }

        // decorrelate sample dimensions
        rt::shuffle(lensSamples, x_pixelSamples * y_pixelSamples, 2);
        rt::shuffle(timeSamples, x_pixelSamples * y_pixelSamples, 1);

        sample_pos = 0;
    }
    
    /* PUBLIC METHODS */
    int roundSize(int size) const override
    {
        return size;
    }
    
    // implementation @ (pg. 310) of pbrt 2nd 3d.
    bool getNextSample(Sample* sample) override
    {
        // compute new set of samples if needed for next pixel
        if(sample_pos == x_pixelSamples * y_pixelSamples)
        {
            // advance to next pixel for stratified sampling
            if(++x_pos == x_end)
            {
                x_pos = x_start;
                ++y_pos;
            }
            if(y_pos == y_end)
                return false;

            // generate stratified camera samples for (x_pos, y_pos)
            generateStratifiedCameraSamples();
        }

        // return next StratifiedSampler sample point
        sample->image_x = imageSamples[2 * sample_pos];
        sample->image_y = imageSamples[2 * sample_pos + 1];
        sample->lens_u = lensSamples[2 * sample_pos];
        sample->lens_v = lensSamples[2 * sample_pos + 1];
        sample->time = timeSamples[sample_pos];
        
        // generate stratifed samples for integrators @ (pg. 313) of pbrt 2nd ed.
        // not implemented yet, starts @ (pg. 311)

        sample_pos++;

        return true;
    }
};

#endif // SAMPLE_H