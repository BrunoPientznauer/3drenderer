#include "display.h"
#include "array.h"


static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *color_buffer_texture = NULL;
static uint32_t *color_buffer = NULL;
static float *z_buffer = NULL;

static int window_width = 800;
static int window_height = 600;

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

    int fullscreen_width = display_mode.w;
    int fullscreen_height = display_mode.h;

    window_width = fullscreen_width / 3.0;
    window_height = fullscreen_height / 3.0;

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        fullscreen_width,
        fullscreen_height,
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

    // initializes color and z buffers
    color_buffer = (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);
	z_buffer = (float*)malloc(sizeof(float) * window_width * window_height);

	if (!color_buffer)
	{
		fprintf(stderr, "an error occurred trying to allocate memory\n");
		return false;
	}

	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height);


    return true;
}

void clear_z_buffer(void)
{
    for(int i = 0; i < window_width * window_height; i++){
        z_buffer[i] = 1.0;
    }
}

float get_zbuffer_at(int x, int y)
{
    if(x >= window_width || y >= window_height || x < 0 || y < 0){
        return 1.0f;
    }

    return z_buffer[(window_width * y) + x];
}

void set_zbuffer_at(int x, int y, float value)
{
    if(x >= window_width || y >= window_height || x < 0 || y < 0){
        return;
    }

    z_buffer[(window_width * y) + x] = value;
}

void clear_color_buffer(uint32_t color)
{
    for (int i = 0; i < window_width * window_height; i++)
    {
        color_buffer[i] = color;
    }
}

void render_color_buffer(void)
{
    SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int)sizeof(uint32_t) * window_width);
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void draw_pixel(int x, int y, uint32_t color)
{
    if(x >= window_width || y >= window_height || x < 0 || y < 0){
        return;
    }

    color_buffer[window_width * y + x] = color;
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
    free(color_buffer);
	free(z_buffer);
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

int get_window_height(void){
    return window_height;
}

int get_window_width(void){
    return window_width;
}
