#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_gpu.h>

#include "pbrt/pbrt.h"
#include "pbrt/MathExam.h"

#include "rtiow/rtiow.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

#define WINDOW_TITLE "rt!"

// fps stuff 
static Uint64 then = 0, now = 0;
static double timeElapsed = 0; // seconds
static Uint64 freq = 0;

const float debug_textScale = 1.0f;

/* HELPER FUNCTIONS */
void debug_drawDebugInfo(SDL_Renderer* renderer, double fps)
{
    SDL_SetRenderDrawColor(renderer, 100, 200, 255, 255);
    
    int debug_lineHeight = (int)SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * debug_textScale;

    // create and draw debug_FPS string
    char debug_FPS[32];
    snprintf(debug_FPS, sizeof(debug_FPS), "%.1f fps", fps);
    SDL_RenderDebugText(renderer, 0, debug_lineHeight * 0, debug_FPS);
    
    // create and draw debug_elapsedTime string
    char debug_timeElapsed[32];
    snprintf(debug_timeElapsed, sizeof(debug_timeElapsed), "%.1f seconds elapsed", timeElapsed);
    SDL_RenderDebugText(renderer, 0, debug_lineHeight * 1, debug_timeElapsed);
}

/* SDL FUNCTIONS */

/* SDL_AppInit -> executed at startup */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    printf("program initializing ...\n");
    
    // initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("failed to initialize SDL3: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // open window and create renderer
    if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, rt::CANVAS_WIDTH, rt::CANVAS_HEIGHT, SDL_WINDOW_HIGH_PIXEL_DENSITY, &window, &renderer)) {
        SDL_Log("failed to create window and/or renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    
    // scale renderer
    SDL_SetRenderScale(renderer, debug_textScale, debug_textScale);
    
    // fps stuff
    freq = SDL_GetPerformanceFrequency();
    then = SDL_GetPerformanceCounter();
    
    // RTIOW initialization
    RayTracingInOneWeekend r;

    return SDL_APP_CONTINUE;
}

/* SDL_AppEvent -> executed when event detected (mouse input, keypress, etc.) */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
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
    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
    SDL_RenderClear(renderer);
    
    // draw fps and elapsed time
    debug_drawDebugInfo(renderer, fps);

    // blip renderer to screen
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

/* SDL_AppQuit -> exectued at shutdown */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    printf("program shutting down ...\n");
}
