#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"

bool is_running = false;

void setup(void){
	color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);

	if(!color_buffer){
		fprintf(stderr, "an error occurred trying to allocate memory");
		is_running = false;
	}

	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);
}

void proccess_input(void){
	SDL_Event event;
	SDL_PollEvent(&event);

	switch(event.type){
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
			if(event.key.keysym.sym == SDLK_ESCAPE) is_running = false;
			break;
	}
}

void update(void){
	//TODO:
}

void render(void){
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);

	draw_grid(10);
	draw_rect(500, 500, 300, 200, 0xFFFF3333);

	render_color_buffer();
	clear_color_buffer(0x00000000);

	SDL_RenderPresent(renderer); 
}

int main(void){

	is_running = initialize_window();

	setup();

	while(is_running){
		proccess_input();
		update();
		render();
	}

	destroy_window();

	return 0;
}
