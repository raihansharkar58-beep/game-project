#include <SDL3/SDL.h>
#include <stdbool.h>
#include <math.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define RADIUS 30

typedef struct
{
    float x, y;
    float dx, dy;
    int radius;
    SDL_Color color;
} Circle;

void drawCircle(SDL_Renderer *renderer, int cx, int cy, int radius, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int w = -radius; w <= radius; w++)
    {
        for (int h = -radius; h <= radius; h++)
        {
            if (w * w + h * h <= radius * radius)
            {
                SDL_RenderPoint(renderer, cx + w, cy + h);
            }
        }
    }
}

bool checkCollision(Circle c1, Circle c2)
{
    float dx = c1.x - c2.x;
    float dy = c1.y - c2.y;
    float distance = sqrt(dx * dx + dy * dy);
    return distance < (c1.radius + c2.radius);
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Circle Collision Demo",SCREEN_WIDTH, SCREEN_HEIGHT,
                                                               SDL_WINDOW_RESIZABLE);
                                                               
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    Circle moving = {0, SCREEN_HEIGHT / 2, 2, 0, RADIUS, {0, 255, 0, 255}};
    Circle controlled = {SCREEN_WIDTH / 2, 0, 0, 0, RADIUS, {0, 0, 255, 255}};

    bool running = true;
    SDL_Event e;

    while (running)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
            if (e.type == SDL_EVENT_KEY_DOWN)
            {
                switch (e.key.key)
                { 
                case SDLK_UP:
                    controlled.y -= 10;
                    break;
                case SDLK_DOWN:
                    controlled.y += 10;
                    break;
                case SDLK_LEFT:
                    controlled.x -= 10;
                    break;
                case SDLK_RIGHT:
                    controlled.x += 10;
                    break;
                }
            }
        }

        moving.x += moving.dx;
        if (moving.x > SCREEN_WIDTH)
            moving.x = 0;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawCircle(renderer, (int)moving.x, (int)moving.y, moving.radius, moving.color);
        drawCircle(renderer, (int)controlled.x, (int)controlled.y, controlled.radius, controlled.color);

        if (checkCollision(moving, controlled))
        {
            SDL_Color red = {255, 0, 0, 255};
            drawCircle(renderer, (int)moving.x, (int)moving.y, moving.radius, red);
            drawCircle(renderer, (int)controlled.x, (int)controlled.y, controlled.radius, red);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}