#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

bool is_running = false;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

SDL_Texture* color_buffer_texture = NULL;
uint32_t* color_buffer = NULL;

int window_width;
int window_height;

bool initialize_window(void){
	// initializes SDL
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
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

	if(!window){
		fprintf(stderr, "Error initializing SDL window.\n");
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if(!renderer){
		fprintf(stderr, "Error initializing SDL renderer.\n");
		return false;
	}

	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return true;
}

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

void clear_color_buffer(uint32_t color){
	for(int y = 0; y < window_height; y++){
		for(int x = 0; x < window_width; x++){
			color_buffer[(window_width * y) + x] = color;
		}
	}
}

void render_color_buffer(void){
	SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int) sizeof(uint32_t) * window_width);
	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void render(void){
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);

	render_color_buffer();
	clear_color_buffer(0xFFFFFF00);

	SDL_RenderPresent(renderer); 
}

void destroy_window(void){
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
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
