#include "gpu_api.h"

SDL_GPUDevice* gpu::init(SDL_Window *window, bool enable_validation)
{
    SDL_GPUDevice* device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, enable_validation, nullptr);

    if(!device)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "gpu::init() : failed to create GPU device!\n%s\n", SDL_GetError());
        
        return nullptr;
    }
    
    SDL_ClaimWindowForGPUDevice(device, window);

    return device;
}

void gpu::shutdown(SDL_GPUDevice* device)
{
    if(device)
        SDL_DestroyGPUDevice(device);
}

// gpt output for implimenting rest of functions
/*

// ------------------------------------------------------------------------------------------------
// shader helpers
// ------------------------------------------------------------------------------------------------
SDL_GPUShader* gpu_load_shader(SDL_GPUDevice* device,
                              const char*     path,
                              SDL_GPUShaderStage stage)
{
    size_t code_size = 0;
    void*  code      = SDL_LoadFile(path, &code_size);
    if (!code) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                     "gpu_load_shader: failed to read '%s': %s",
                     path, SDL_GetError());
        return nullptr;
    }

    SDL_GPUShaderCreateInfo info = {
        .code                 = code,
        .code_size            = code_size,
        .entrypoint           = "main",
        .format               = SDL_GPU_SHADERFORMAT_SPIRV,
        .stage                = stage,
        .num_uniform_buffers  = 0,
        .num_samplers         = 0,
        .num_storage_buffers  = 0,
        .num_storage_textures = 0
    };

    SDL_GPUShader* shader = SDL_CreateGPUShader(device, &info);
    SDL_free(code);
    if (!shader) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                     "gpu_load_shader: SDL_CreateGPUShader failed: %s",
                     SDL_GetError());
    }
    return shader;
}

// ------------------------------------------------------------------------------------------------
// pipeline creation (swapchain target only)
// ------------------------------------------------------------------------------------------------
SDL_GPUGraphicsPipeline*
gpu_create_pipeline(SDL_GPUDevice*    device,
                    SDL_GPUShader*    vert,
                    SDL_GPUShader*    frag,
                    SDL_Window*       window)
{
    // pick the window’s swapchain format
    SDL_GPUTextureFormat fmt = SDL_GetGPUSwapchainTextureFormat(device, window);

    // single color attachment, no blending by default
    SDL_GPUColorTargetDescription color_desc = {
        .format      = fmt,
        .blend_state = {  // default: no blending, write all RGBA
            .src_color_blendfactor  = SDL_GPU_BLENDFACTOR_ONE,
            .dst_color_blendfactor  = SDL_GPU_BLENDFACTOR_ZERO,
            .color_blend_op         = SDL_GPU_BLENDOP_ADD,
            .src_alpha_blendfactor  = SDL_GPU_BLENDFACTOR_ONE,
            .dst_alpha_blendfactor  = SDL_GPU_BLENDFACTOR_ZERO,
            .alpha_blend_op         = SDL_GPU_BLENDOP_ADD,
            .color_write_mask       = SDL_GPU_COLORCOMPONENT_ALL,
            .enable_blend           = false,
            .enable_color_write_mask= false
        }
    };

    SDL_GPUGraphicsPipelineTargetInfo tinfo = {
        .num_color_targets           = 1,
        .color_target_descriptions   = &color_desc,
        .depth_stencil_target_format = SDL_GPU_TEXTUREFORMAT_INVALID
    };

    SDL_GPUGraphicsPipelineCreateInfo info = {
        .target_info       = tinfo,
        .primitive_type    = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
        .vertex_shader     = vert,
        .fragment_shader   = frag,
        .num_vertex_elements = 0,
        .vertex_element_formats = nullptr,
        .vertex_buf_layouts     = nullptr,
        .index_element_size     = SDL_GPU_INDEXELEMENTSIZE_16BIT
    };
    info.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;

    SDL_GPUGraphicsPipeline* pipe =
        SDL_CreateGPUGraphicsPipeline(device, &info);
    if (!pipe) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                     "gpu_create_pipeline: %s",
                     SDL_GetError());
    }
    return pipe;
}

// ------------------------------------------------------------------------------------------------
// buffer creation & update
// ------------------------------------------------------------------------------------------------
SDL_GPUBuffer*
gpu_create_buffer(SDL_GPUDevice* device,
                  Uint32         usageFlags,
                  size_t         sizeInBytes,
                  const void*    initialData)
{
    SDL_GPUBuffer* buf =
        SDL_CreateGPUBuffer(device, usageFlags, sizeInBytes, initialData);
    if (!buf) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                     "gpu_create_buffer: %s",
                     SDL_GetError());
    }
    return buf;
}

void
gpu_update_buffer(SDL_GPUDevice* device,
                  SDL_GPUBuffer* buffer,
                  const void*    data,
                  size_t         sizeInBytes,
                  size_t         offset)
{
    if (SDL_UploadToGPUBuffer(device, buffer, data, sizeInBytes, offset) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                     "gpu_update_buffer: %s",
                     SDL_GetError());
    }
}

// ------------------------------------------------------------------------------------------------
// binding & drawing
// ------------------------------------------------------------------------------------------------
void
gpu_bind_vertex_buffers(SDL_GPURenderPass*        pass,
                        const SDL_GPUBufferBinding* bindings,
                        int                         bindingCount)
{
    // binding.index is implied by array order
    for (int i = 0; i < bindingCount; ++i) {
        SDL_BindGPUVertexBuffer(pass,
                                bindings[i].buffer,
                                i,
                                bindings[i].offset);
    }
}

void
gpu_bind_index_buffer(SDL_GPURenderPass* pass,
                      SDL_GPUBuffer*      indexBuffer,
                      Uint32              offset)
{
    SDL_BindGPUIndexBuffer(pass, indexBuffer, offset);
}

void
gpu_draw_triangles(SDL_GPURenderPass* pass,
                   int                vertexCount,
                   int                instanceCount)
{
    SDL_DrawGPUPrimitives(pass,
                          0,
                          SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
                          vertexCount,
                          instanceCount);
}

// ------------------------------------------------------------------------------------------------
// frame begin / end
// ------------------------------------------------------------------------------------------------
SDL_GPUCommandBuffer*
gpu_begin_frame(SDL_GPUDevice* device,
                SDL_Window*    window,
                SDL_Color      clearColor)
{
    // fetch a command buffer & the swapchain texture
    SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUTexture*       tx   = nullptr;
    SDL_WaitAndAcquireGPUSwapchainTexture(cmd,
                                           window,
                                           &tx,
                                           nullptr,
                                           nullptr);

    // start one render pass clearing to clearColor
    SDL_GPUColorTargetInfo ct = {
        .texture     = tx,
        .load_op     = SDL_GPU_LOADOP_CLEAR,
        .store_op    = SDL_GPU_STOREOP_STORE,
        .clear_color= clearColor
    };

    SDL_BeginGPURenderPass(cmd, &ct, 1, nullptr);
    return cmd;
}

void
gpu_end_frame(SDL_GPUCommandBuffer* cmd)
{
    SDL_EndGPURenderPass(cmd);
    SDL_SubmitGPUCommandBuffer(cmd);
}

// ------------------------------------------------------------------------------------------------
// texture helper
// ------------------------------------------------------------------------------------------------
SDL_GPUTexture*
gpu_create_texture_from_surface(SDL_GPUDevice* device,
                                SDL_Surface*   surface)
{
    // note: make sure surface is SDL_PIXELFORMAT_RGBA8888 or convert before
    SDL_GPUTexture* tex = SDL_CreateGPUTexture(device,
                                               SDL_GPU_TEXTURETYPE_2D,
                                               SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
                                               SDL_GPU_TEXTUREUSAGE_SAMPLER,
                                               surface->w,
                                               surface->h,
                                               1);
    if (tex) {
        // copy pixels over
        SDL_UploadToGPUTexture(device,
                               tex,
                               surface->pixels,
                               surface->format->format,
                               surface->format->BytesPerPixel*8,
                               surface->w,
                               surface->h,
                               0, 0,
                               0,
                               0);
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                     "gpu_create_texture_from_surface: %s",
                     SDL_GetError());
    }
    return tex;
}

// ------------------------------------------------------------------------------------------------
// simple error check
// ------------------------------------------------------------------------------------------------
void
gpu_check_error(const char* context)
{
    const char* err = SDL_GetError();
    if (err && *err) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                     "%s: %s",
                     context, err);
        SDL_ClearError();
    }
}

*/