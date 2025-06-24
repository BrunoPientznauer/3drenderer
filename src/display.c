#include "display.h"
#include "array.h"


SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *color_buffer_texture = NULL;
uint32_t *color_buffer = NULL;
float *z_buffer = NULL;
int window_width = 800;
int window_height = 600;

int previous_frame = 0;

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

void clear_z_buffer(void)
{
    for(int y = 0; y < window_height; y++){
        for(int x = 0; x < window_width; x++){
            z_buffer[window_width * y + x] = 1.0;
        }
    }
}

void clear_color_buffer(uint32_t color)
{
    for (int y = 0; y < window_height; y++)
    {
        for (int x = 0; x < window_width; x++)
        {
            draw_pixel(x, y, 0xFF000000);
        }
    }
}

void render_color_buffer(void)
{
    SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int)sizeof(uint32_t) * window_width);
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void draw_pixel(int x, int y, uint32_t color)
{
    if(x < window_width && y < window_height && x >= 0 && y >= 0){
        color_buffer[window_width * y + x] = color;
    }
}

void draw_grid(int grid_size)
{
    for (int y = 0; y < window_height; y += grid_size)
    {
        for (int x = 0; x < window_width; x += grid_size)
        {
            draw_pixel(x, y, 0xFF808080);
        }
    }
}

void draw_rect(int x, int y, int width, int height, uint32_t color)
{
    for (int i = x; i < x + height; i++)
    {
        for (int j = y; j < y + width; j++)
        {
            draw_pixel(i, j, color);
        }
    }
}

void destroy_window(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void draw_line(int x0, int y0, int x1, int y1, uint32_t color)
{
    int delta_x = (x1 - x0);
    int delta_y = (y1 - y0);

    int side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);

    float x_increment = delta_x / (float) side_length;
    float y_increment = delta_y / (float) side_length;

    float actual_x = x0;
    float actual_y = y0;

    for(int i = 0; i <= side_length; i++){
        draw_pixel(round(actual_x), round(actual_y), color);
        actual_x += x_increment;
        actual_y += y_increment;
    }
}

void draw_line_between_vectors(vec2_t p1, vec2_t p2, uint32_t color)
{
    draw_line(p1.x, p1.y, p2.x, p2.y, color);
}

void draw_triangle(vec2_t v1, vec2_t v2, vec2_t v3, uint32_t color)
{
    draw_line_between_vectors(v1, v2, color);
    draw_line_between_vectors(v2, v3, color);
    draw_line_between_vectors(v3, v1, color);
}