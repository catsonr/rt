#ifndef RTIOW_H
#define RTIOW_H

#include <SDL3/SDL.h>

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
    std::vector<uint32_t> canvas = std::vector<uint32_t>(width * height);

    SDL_Renderer* renderer { nullptr };
    SDL_Texture* texture = { nullptr };

    float t { 0.0f };
    
    //OrthographicCamera camera;
    StratifiedSampler sampler{ 0, static_cast<int>(width), 0, static_cast<int>(height) };
    Sample sample { 0, 0, 0, 0, 0 };
    
    /* CONSTRUCTORS */
    RayTracingInOneWeekend(SDL_Renderer* renderer) :
        renderer(renderer)
    {
        printf("RTIOW: creating texture ...\n");
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
        //SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_NONE); // turns off blend so alpha is ignored
    }
    
    /* DECONSTRUCTORS */
    ~RayTracingInOneWeekend()
    {
        printf("RTIOW: cleaning up ...\n");
        if(texture) SDL_DestroyTexture(texture);
    }
    
    /* PUBLIC FUNCTIONS */
    void tick(float dt)
    {
        t += dt;

        printf("RTIOW: ticked time by %f\n", dt);
    }
    
    // fills canvas with colors from sampler
    void samplePixels()
    {
        //printf("RTIOW: sampling pixels ...\n");
        
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
    
    // draws canvas to SDL texture
    inline void draw()
    {
        //printf("RTIOW: drawing frame ...\n");
        
        SDL_UpdateTexture(texture, nullptr, canvas.data(), width * sizeof(uint32_t));
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    }
};

#endif