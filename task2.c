#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define INITIAL_RADIUS 20
#define GROWTH_RATE 60.0f

 void DrawCircle(SDL_Renderer *renderer, int cx, int cy, int radius)
{
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius))
                SDL_RenderPoint(renderer, (float)(cx + dx), (float)(cy + dy));
        }
    }
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (!SDL_CreateWindowAndRenderer("SDL3 - Growing Circle",
                                     WINDOW_WIDTH, WINDOW_HEIGHT,
                                     0, &window, &renderer))
    {
        SDL_Log("SDL_CreateWindowAndRenderer failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    const int cx = WINDOW_WIDTH / 2;
    const int cy = WINDOW_HEIGHT / 2;

    const int maxRadius = (cx < cy ? cx : cy);

    float radius = (float)INITIAL_RADIUS;
    int collisions = 0;

    Uint64 prevTick = SDL_GetTicks();

    SDL_Log("Center (%d, %d) | max radius before collision: %d", cx, cy, maxRadius);

    int running = 1;
    SDL_Event event;

    while (running)
    {

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = 0;
            if (event.type == SDL_EVENT_KEY_DOWN &&
                event.key.key == SDLK_ESCAPE)
                running = 0;
        }

        Uint64 nowTick = SDL_GetTicks();
        float dt = (nowTick - prevTick) / 1000.0f;
        prevTick = nowTick;

        radius += GROWTH_RATE * dt;

        if ((int)radius >= maxRadius)
        {
            radius = (float)INITIAL_RADIUS;
            collisions++;
            SDL_Log("Collision #%d - radius reset to %d", collisions, INITIAL_RADIUS);
        }

        int iRadius = (int)radius;

        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 70, 130, 220, 255);
        DrawCircle(renderer, cx, cy, iRadius);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}