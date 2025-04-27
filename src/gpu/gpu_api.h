/*
    gpu_api.h contains helper functions for the use of SDL3's gpu api
*/

#ifndef GPU_API_H
#define GPU_API_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>

namespace gpu
{

    SDL_GPUDevice* init(
        SDL_Window* window,
        bool enable_validation
    );

    void shutdown(SDL_GPUDevice* device);
    
    SDL_GPUShader* loadShader(
        SDL_GPUDevice* device,
        const char* path,
        SDL_GPUShaderStage stage
    );
    
    SDL_GPUGraphicsPipeline* createPipeline(
            SDL_GPUDevice* device,
            SDL_GPUShader* vert,
            SDL_GPUShader* frag,
            SDL_GPUTexture* target_texture
    );
    
    SDL_GPUBuffer* createBuffer(
        SDL_GPUDevice* device,
        Uint32 usageFlags, // e.g., SDL_GPU_BUFFERUSAGE_VERTEX_BIT
        size_t sizeInBytes,
        const void* initialData // or nullptr
    );
    
    void updateBuffer(
        SDL_GPUDevice* device,
        SDL_GPUBuffer* buffer,
        const void* data,
        size_t sizeInBytes,
        size_t offset
    );
    
    void bindVertexBuffers(
        SDL_GPURenderPass* pass,
        const SDL_GPUBufferBinding* bindings,
        int bindingCount
    );
    
    void bindIndexBuffer(
        SDL_GPURenderPass* pass,
        SDL_GPUBuffer* indexBuffer,
        Uint32 offset
    );
    
    void drawTriangles(
        SDL_GPURenderPass* pass,
        int vertexCount,
        int instanceCount = 1
    );
    
    SDL_GPUCommandBuffer* beginFrame(
        SDL_GPUDevice* device,
        SDL_Window* window,
        SDL_Color clearColor
    );
    
    void endFrame(SDL_GPUCommandBuffer* cmd);
    
    SDL_GPUTexture* createTextureFromSurface(
        SDL_GPUDevice* device,
        SDL_Surface* surface
    );
    
    void checkErrors(const char* context);

} // gpu

#endif