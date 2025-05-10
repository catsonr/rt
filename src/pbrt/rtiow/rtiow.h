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
    static constexpr unsigned int width { rt::CANVAS_WIDTH };
    static constexpr unsigned int height { rt::CANVAS_HEIGHT };
    static constexpr float aspectRatio = float(width) / float(height);

    std::vector<std::shared_ptr<Shape>> shapes;

    SDL_Renderer* renderer { nullptr };
    SDL_Texture* texture { nullptr };
    void* pixels;
    int pitch;

    float t { 0.0f };

    // camera stuff
    const float screen[4]{
        -aspectRatio, // x_min
        aspectRatio, // x_max
        -1, // y_min
        1 // y_max
    };
    float clip_near { 0.0f };
    float clip_far { 10.0f };
    float shutter_open { 0.0f };
    float shutter_close { 0.0f };
    float lensRadius { 1.0f };
    float focalDistance { 1.0f };
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

    // sampler stuff
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
        
        // shape stuff
        printf("[RTIOW] creating shapes ...\n");
        
        float zmin = -1;
        float zmax = 1;
        float phi = rt::TWOPI * 0.8;

        Transform world_to_sphere = Transform::rotateX(rt::PI * 0.1) * Transform::translate( Vector(0.0, 0.0, -4.0) );
        std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(world_to_sphere.getInverse(), false, 1.0f, zmin, zmax, phi);
        
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

    // writes directly to SDL texture, as a test
    void textureTest()
    {
        void *pixels = nullptr;
        int pitch = 0;
        if( SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0)
        {
            printf("[RTIOW] failed to lock texture: %s\n", SDL_GetError());
        }

        // assume ARGB8888 (1 uint32_t per pixel), pitch is in bytes
        for (unsigned y = 0; y < height; ++y)
        {
            uint32_t *row = (uint32_t *)((uint8_t *)pixels + y * pitch);
            for (unsigned x = 0; x < width; ++x)
            {
                uint8_t r = uint8_t(255.0f * x / width);
                uint8_t g = uint8_t(255.0f * y / height);
                uint8_t b = 128;
                uint8_t a = 255;

                // SDL_PIXELFORMAT_RGBA8888 → R at highest bits
                row[x] = (r << 24) | (g << 16) | (b << 8) | a;
            }
        }

        SDL_UnlockTexture(texture);
    }

    void samplePixels()
    {
        printf("[RTIOW] sampling pixels ...\n");
        
        void* pixels = nullptr;
        int pitch = 0;
        if( SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0)
        {
            printf("[RTIOW] failed to lock texture: %s\n", SDL_GetError());
        }
        
        int samplerSampleCount = 0;
        // for each pixel 
        Ray ray; 
        while( sampler.getNextSample(&sample) )
        {
            //printf("sample %i : image_x=%f | image_y=%f\n", samplerSampleCount, sample.image_x, sample.image_y);
            samplerSampleCount++; 

            camera.generateRay(sample, &ray);
            //printf("\tray o = (%.2f %.2f %.2f)\n", ray.o.x, ray.o.y, ray.o.z);
            //printf("\tray d = (%.2f %.2f %.2f)\n\n", ray.d.x, ray.d.y, ray.d.z);

            // test ray intersection on all shapes
            bool hit = false;
            for(auto& shape : shapes)
            {
                float thit = 0; // this it not actually used btw, it's just here so the intersection function can be called (for now)
                if( shape->intersect(ray, &thit, nullptr) )
                {
                    hit = true;

                    break;
                }
            }
            
            // color purple if hits, otherwise do a sky gradient
            Vector color;
            if(hit)
            {
                color = Vector(0.9f, 0.2f, 0.9f); // purple 
            }
            else
            {
                Vector dir = normalize(ray.d);
                float tt = 0.5f * (dir.y + 1.0f);
                Vector white(1.0f, 1.0f, 1.0f);
                Vector blue(0.5f, 0.7f, 1.0f);
                color = white * (1.0f - tt) + blue * tt;
            }
            
            // put pixel color in SDL texture 
            int ix = static_cast<int>(sample.image_x);
            int iy = static_cast<int>(sample.image_y);
            if(ix >= 0 && ix < static_cast<int>(width) && iy >= 0 && iy <= static_cast<int>(height))
            {
                uint8_t r = (u_int8_t)(255.0f * std::clamp(color.x, 0.0f, 1.0f));
                uint8_t g = (u_int8_t)(255.0f * std::clamp(color.y, 0.0f, 1.0f));
                uint8_t b = (u_int8_t)(255.0f * std::clamp(color.z, 0.0f, 1.0f));
                uint8_t a = 255.0f;
                
                uint32_t* row = (uint32_t*)((uint8_t*)pixels + iy * pitch);
                row[ix] = (r << 24) | (g << 16) | (b << 8) | a;
            }
        }
        
        printf("\tsampler generated %i samples\n", samplerSampleCount);

        SDL_UnlockTexture(texture);
    }

    // draws canvas to SDL texture
    inline void draw()
    {
        //printf("[RTIOW] drawing frame ...\n");
        
        // texture should really only be updated if it's changed
        //SDL_UpdateTexture(texture, nullptr, canvas.data(), width * sizeof(uint32_t));

        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    }
};

#endif