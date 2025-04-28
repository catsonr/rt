#ifndef RTIOW_H
#define RTIOW_H

#include <SDL3/SDL.h>

#include <vector>

#include "pbrt.h"

#include "Sphere.h"
#include "Camera.h"
#include "Sample.h"

struct RayTracingInOneWeekend
{
public:
    /* PUBLIC MEMBERS */
    static const unsigned int width { rt::CANVAS_WIDTH };
    static const unsigned int height { rt::CANVAS_HEIGHT };
    static constexpr float aspectRatio = width / height;

    std::vector<std::shared_ptr<Shape>> shapes;
    std::vector<uint32_t> canvas = std::vector<uint32_t>(width * height);

    SDL_Renderer* renderer { nullptr };
    SDL_Texture* texture = { nullptr };

    float t { 0.0f };
    
    const float screen[4] { -1, 1, -1/aspectRatio, 1/aspectRatio };
    float clip_near { 1e-3f };
    float clip_far { 10.0f };
    float shutter_open { 0.0f };
    float shutter_close { 0.0f };
    float lensRadius { 1.0f };
    float focalDistance { 1.0f };
    //Transform world_to_camera = Transform::translate( Vector(0, 0, 1) ); 
    //OrthographicCamera camera { world_to_camera, screen, clip_near, clip_far, shutter_open, shutter_close, lensRadius, focalDistance };
    Transform camera_to_world = Transform::translate( Vector(0, 0, 1) );
    OrthographicCamera camera {
        camera_to_world,
        screen,
        clip_near,
        clip_far,
        shutter_open,
        shutter_close,
        lensRadius,
        focalDistance
    };

    const int x_start = 0, x_end = width;
    const int y_start = 0, y_end = height;
    const int x_samples = 1, y_samples = 1;
    const bool jitter = false;
    StratifiedSampler sampler{  x_start, x_end, y_start, y_end, x_samples, y_samples, jitter };
    Sample sample { 0, 0, 0, 0, 0 };
    
    /* CONSTRUCTORS */
    RayTracingInOneWeekend(SDL_Renderer* renderer) :
        renderer(renderer)
    {
        printf("[RTIOW] creating texture ...\n");
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
        
        printf("[RTIOW] creating shapes ...\n");
        Transform world_to_sphere = Transform::translate( Vector(0, 0, -4.0) );
        std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(world_to_sphere.getInverse(), false, 1.0f);

        shapes.push_back(sphere);
    }
    
    /* DECONSTRUCTORS */
    ~RayTracingInOneWeekend()
    {
        printf("[RTIOW] cleaning up ...\n");
        if(texture) SDL_DestroyTexture(texture);
    }
    
    /* PUBLIC FUNCTIONS */
    void tick(float dt)
    {
        t += dt;

        printf("[RTIOW] ticked time by %f\n", dt);
    }
    
    // fills canvas with colors to test if it's working
    void canvasTest()
    {
        printf("[RTIOW] testing canvas ...\n");
        
        for(unsigned y = 0; y < height; y++)
        {
            for(unsigned x = 0; x < width; x++)
            {
                uint8_t r = uint8_t(255.0f * x / width);
                uint8_t g = uint8_t(255.0f * y / height);
                uint8_t b = 128;
                uint8_t a = 255;
                
                canvas[y*width + x] = (r << 24) | (g << 16) | (b << 8) | a;
            }
        }
    }
    
    void samplePixels()
    {
        printf("[RTIOW] sampling pixels ...\n");
        
        // fill the canvas array with all 0s (black, transparent)
        std::fill(canvas.begin(), canvas.end(), 0u);
        
        int samplerSampleCount = 0;
        // for each pixel 
        while( sampler.getNextSample(&sample) )
        {
            //printf("sample %i : image_x=%f | image_y=%f\n", samplerSampleCount, sample.image_x, sample.image_y);
            samplerSampleCount++; 

            Ray ray; 
            camera.generateRay(sample, &ray);
            //printf("\tray o = (%.2f %.2f %.2f)\n", ray.o.x, ray.o.y, ray.o.z);
            //printf("\tray d = (%.2f %.2f %.2f)\n", ray.d.x, ray.d.y, ray.d.z);
            
            if(samplerSampleCount < 10)
            {
                if (samplerSampleCount < 10) {
                    printf("ray %d: img=(%.2f,%.2f) o=(%.2f,%.2f,%.2f) d=(%.2f,%.2f,%.2f)\n",
                        samplerSampleCount, sample.image_x, sample.image_y,
                        ray.o.x, ray.o.y, ray.o.z,
                        ray.d.x, ray.d.y, ray.d.z);
                }
            }

            // test ray intersection on all shapes
            bool hit = false;
            for(auto& shape : shapes)
            {
                if( shape->doesIntersect(ray) )
                {
                    hit = true;
                    //printf("hit something!\n");
                    break;
                }
            }
            
            // color purple if hits, otherwise do a sky gradient
            Vector color;
            if(hit)
            {
                color = Vector(1.0f, 0.0f, 1.0f); // purple 
            }
            else
            {
                Vector dir = normalize(ray.d);
                float tt = 0.5f * (dir.y + 1.0f);
                Vector white(1.0f, 1.0f, 1.0f);
                Vector blue(0.5f, 0.7f, 1.0f);
                color = white * (1.0f - tt) + blue * tt;
            }
            
            // put pixel color in canvas array
            int ix = (int)sample.image_x;
            int iy = (int)sample.image_y;
            if (ix >= 0 && ix < (int)width && iy >= 0 && iy < (int)height) {
                uint8_t r = (uint8_t)(255.0f * color.x);
                uint8_t g = (uint8_t)(255.0f * color.y);
                uint8_t b = (uint8_t)(255.0f * color.z);
                uint8_t a = 255;
                canvas[iy * width + ix] = (r << 24) | (g << 16) | (b << 8) | a;
            } 
        }
        
        printf("\tsampler generated %i samples\n", samplerSampleCount);
    }

    // draws canvas to SDL texture
    inline void draw()
    {
        //printf("[RTIOW] drawing frame ...\n");
        
        SDL_UpdateTexture(texture, nullptr, canvas.data(), width * sizeof(uint32_t));
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    }
};

#endif