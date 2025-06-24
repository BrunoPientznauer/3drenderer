#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "vectors.h"

#define FPS 60
#define TARGET_FRAME_TIME (1000 / FPS)

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* color_buffer_texture;
extern uint32_t* color_buffer;
extern float* z_buffer;
extern int window_width;
extern int window_height;
extern int previous_frame;


bool initialize_window(void);
void clear_z_buffer(void);
void clear_color_buffer(uint32_t color);
void render_color_buffer(void);
void draw_pixel(int x, int y, uint32_t color);
void draw_grid(int grid_size);
void draw_rect(int x, int y, int width, int height, uint32_t color);
void destroy_window(void);
void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
void draw_line_between_vectors(vec2_t p1, vec2_t p2, uint32_t color);
void draw_triangle(vec2_t v1, vec2_t v2, vec2_t v3, uint32_t color);

#endif