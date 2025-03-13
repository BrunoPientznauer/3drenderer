#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vectors.h"
#include "mesh.h"

triangle_t triangles_to_render[N_MESH_FACES];

vec3_t camera_position = {.x = 0, .y = 0, .z = -5};
float fov_factor = 640;

bool is_running = false;

vec3_t cube_rotation = {.x = 0, .y = 0, .z = 0};

vec2_t projected_points[N_MESH_FACES * 3];

void setup(void)
{
	color_buffer = (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);

	if (!color_buffer)
	{
		fprintf(stderr, "an error occurred trying to allocate memory");
		is_running = false;
	}

	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height);
}

void proccess_input(void)
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		is_running = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
			is_running = false;
		break;
	}
}

vec2_t project(vec3_t point)
{
	vec2_t projection = {
		.x = (fov_factor * point.x) / point.z,
		.y = (fov_factor * point.y) / point.z
	};

	return projection;
}

void update(void)
{
	//lock the program fps
	int time_to_wait = TARGET_FRAME_TIME - (SDL_GetTicks() - previous_frame);
	
	if(time_to_wait > 0 && time_to_wait <= TARGET_FRAME_TIME){
		SDL_Delay(time_to_wait);
	}

	previous_frame = SDL_GetTicks();
	
	cube_rotation.x += 0.01;
	cube_rotation.y += 0.01;
	cube_rotation.z += 0.01;

	for(int i = 0; i < N_MESH_FACES; i++){
		face_t face = mesh_faces[i];
		
		vec3_t face_vertices[3];
		face_vertices[0] = mesh_vertices[face.a - 1];
		face_vertices[1] = mesh_vertices[face.b - 1];
		face_vertices[2] = mesh_vertices[face.c - 1];

		triangle_t projected_triangle;
		for(int j = 0; j < 3; j++){
			vec3_t transformed_vertex = face_vertices[j];
			transformed_vertex = vector_rotate_x(transformed_vertex, cube_rotation.x);
			transformed_vertex = vector_rotate_y(transformed_vertex, cube_rotation.y);
			transformed_vertex = vector_rotate_z(transformed_vertex, cube_rotation.z);

			transformed_vertex.z -= camera_position.z;

			vec2_t projected_vertex = project(transformed_vertex);

			projected_vertex.x += (window_width / 2);
			projected_vertex.y += (window_height / 2);

			projected_triangle.points[j] = projected_vertex;
		}

		triangles_to_render[i] = projected_triangle;
	}

	// for (int i = 0; i < NUMBER_OF_POINTS; i++)
	// {
	// 	vec3_t point = cube_points[i];

	// 	vec3_t rotated_point = vector_rotate_x(point, cube_rotation.x);
	// 	rotated_point = vector_rotate_y(rotated_point, cube_rotation.y);
	// 	rotated_point = vector_rotate_z(rotated_point, cube_rotation.z);

	// 	rotated_point.z -= camera_position.z;

	// 	vec2_t projected_point = project(rotated_point);

	// 	projected_points[i] =  projected_point;
	// }
}

void render(void)
{
	draw_grid(10);

	for (int i = 0; i < N_MESH_FACES; i++)
	{
		triangle_t triangle = triangles_to_render[i];
		draw_rect(triangle.points[0].x, triangle.points[0].y, 4, 4, 0xFFFF0000);
		draw_rect(triangle.points[1].x, triangle.points[1].y, 4, 4, 0xFFFF0000);
		draw_rect(triangle.points[2].x, triangle.points[2].y, 4, 4, 0xFFFF0000);
	}

	render_color_buffer();
	clear_color_buffer(0x00000000);

	SDL_RenderPresent(renderer);
}

int main(void)
{

	is_running = initialize_window();

	setup();

	while (is_running)
	{
		proccess_input();
		update();
		render();
	}

	destroy_window();

	return 0;
}
