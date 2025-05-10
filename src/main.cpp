/* SDL3 INCLUDES */
/* after installation of SDL3 to the root directory, there should be a folder 'vendorded', which contains the build of SDL3 that is used */
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>

/* this is only for the rt namespace, which includes global defintitions like canvas width, and rng */
#include "pbrt/pbrt.h"
#include "rtiow/rtiow.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

struct BBXState
{
    bool running = true;
    char window_title[32] = "rt!";
    int targetCanvasWidth = rt::CANVAS_WIDTH;
    int targetCanvasHeight = rt::CANVAS_HEIGHT;

    int canvasX, canvasY;
};

BBXState* bbxstate;

/* RTIOW stuff */
std::unique_ptr<RayTracingInOneWeekend> rtiow;

// fps stuff 
static Uint64 then = 0, now = 0;
static double timeElapsed = 0; // seconds
static Uint64 freq = 0;
const float debug_textScale = 1.0f;

/* HELPER FUNCTIONS */
void debug_drawDebugInfo(SDL_Renderer* renderer, double fps)
{
    SDL_SetRenderDrawColor(renderer, 10, 10, 60, 255);
    
    int debug_lineHeight = (int)SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * debug_textScale / debug_textScale;

    // create and draw debug_FPS string
    char debug_FPS[32];
    snprintf(debug_FPS, sizeof(debug_FPS), "%.1f fps", fps);
    SDL_RenderDebugText(renderer, 0, debug_lineHeight * 0, debug_FPS);
    
    // create and draw debug_elapsedTime string
    char debug_timeElapsed[32];
    snprintf(debug_timeElapsed, sizeof(debug_timeElapsed), "%.1f seconds elapsed", timeElapsed);
    SDL_RenderDebugText(renderer, 0, debug_lineHeight * 1, debug_timeElapsed);
}

void event_handleWindowResize(SDL_Renderer* renderer, BBXState* state)
{
    SDL_GetCurrentRenderOutputSize(renderer, &state->canvasX, &state->canvasY);
    
    printf("[bbx] updated canvas width and height to %i x %i\n", state->canvasX, state->canvasY);
}

/* SDL FUNCTIONS */

/* SDL_AppInit -> executed at startup */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    bbxstate = new BBXState();
    printf("[bbx] initializing ...\n");
    
    // initialize SDL
    if( !SDL_Init(SDL_INIT_VIDEO) ) {
        SDL_Log("[sdl] failed to initialize SDL3: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // open window and create renderer
    SDL_WindowFlags windowflags =
        SDL_WINDOW_HIGH_PIXEL_DENSITY |
        SDL_WINDOW_ALWAYS_ON_TOP | 
        SDL_WINDOW_RESIZABLE |
        SDL_WINDOW_INPUT_FOCUS | 
        SDL_WINDOW_MOUSE_FOCUS
    ;
    if( !SDL_CreateWindowAndRenderer(bbxstate->window_title, bbxstate->targetCanvasWidth, bbxstate->targetCanvasHeight, windowflags, &window, &renderer) ) {
        SDL_Log("[sdl] failed to create window and/or renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    event_handleWindowResize(renderer, bbxstate);
    printf("[bbx] created window of size (%i x %i)\n", bbxstate->canvasX, bbxstate->canvasY);
    
    if( SDL_SetRenderVSync(renderer, SDL_RENDERER_VSYNC_ADAPTIVE) )
    {
        SDL_Log("[sdl] failed to enable vsync: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    printf("[bbx] enabled vsync\n");
    
    // scale renderer
    SDL_SetRenderScale(renderer, debug_textScale, debug_textScale);
    
    rtiow = std::make_unique<RayTracingInOneWeekend>(renderer);
    rtiow->samplePixels();
    
    // fps stuff
    freq = SDL_GetPerformanceFrequency();
    then = SDL_GetPerformanceCounter();
    
    return SDL_APP_CONTINUE;
}

/* SDL_AppEvent -> executed when event detected (mouse input, keypress, etc.) */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    else if(event->type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) 
    {
        printf("[bbx-event] window resized!\n");
        event_handleWindowResize(renderer, bbxstate);
    }

    return SDL_APP_CONTINUE;
}

/* SDL_AppIterate -> runs every frame */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    // calculate fps
    now = SDL_GetPerformanceCounter();
    Uint64 dt = now - then;
    double fps;
    if(dt > 0)
        fps = (double)freq / dt;
    then = now;
    timeElapsed += (double)dt / freq;

    // clear screen
    SDL_SetRenderDrawColor(renderer, 233, 255, 211, 255);
    SDL_RenderClear(renderer);
    
    // draw current frame of RTIOW
    rtiow->draw();
    
    // draw fps and elapsed time
    debug_drawDebugInfo(renderer, fps);

    // blip renderer to screen
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

/* SDL_AppQuit -> exectued at shutdown */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    delete bbxstate;

    printf("[rt] shutting down ... (ran for %.1f seconds)\n", timeElapsed);
}
