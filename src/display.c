#include "display.h"


SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *color_buffer_texture = NULL;
uint32_t *color_buffer = NULL;
int window_width;
int window_height;

bool initialize_window(void)
{
    // initializes SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    // get the max resolution of the current display(monitor)
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);

    window_width = display_mode.w;
    window_height = display_mode.h;

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        SDL_WINDOW_BORDERLESS);

    if (!window)
    {
        fprintf(stderr, "Error initializing SDL window.\n");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        fprintf(stderr, "Error initializing SDL renderer.\n");
        return false;
    }

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    return true;
}

void clear_color_buffer(uint32_t color)
{
    for (int y = 0; y < window_height; y++)
    {
        for (int x = 0; x < window_width; x++)
        {
            color_buffer[(window_width * y) + x] = color;
        }
    }
}

void render_color_buffer(void)
{
    SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int)sizeof(uint32_t) * window_width);
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void draw_grid(int grid_size)
{
    for (int y = 0; y < window_height; y += grid_size)
    {
        for (int x = 0; x < window_width; x += grid_size)
        {
            color_buffer[(window_width * y) + x] = 0xFF808080;
        }
    }
}

void draw_rect(int x, int y, int width, int height, uint32_t color)
{
    for (int i = x; i < x + height; i++)
    {
        for (int z = y; z < y + width; z++)
        {
            color_buffer[(window_width * i) + z] = color;
        }
    }
}

void destroy_window(void)
{
    free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}