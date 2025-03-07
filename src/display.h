#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define FPS 30
#define TARGET_FRAME_TIME (1000 / FPS)

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* color_buffer_texture;
extern uint32_t* color_buffer;
extern int window_width;
extern int window_height;
extern int previous_frame;


bool initialize_window(void);
void clear_color_buffer(uint32_t color);
void render_color_buffer(void);
void draw_pixel(int x, int y, uint32_t color);
void draw_grid(int grid_size);
void draw_rect(int x, int y, int width, int height, uint32_t color);
void destroy_window(void);

#endif