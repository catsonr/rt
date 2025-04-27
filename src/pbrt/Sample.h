#ifndef SAMPLE_H
#define SAMPLE_H

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

    /* PUBLIC METHODS */
};

#endif // SAMPLE_H