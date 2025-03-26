#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vectors.h"
#include "mesh.h"
#include "array.h"

// define an array of triangles to be rendered on screen
triangle_t* triangles_to_render = NULL;

vec3_t camera_position = {0, 0, 0};
float fov_factor = 640;

bool is_running = false;

vec2_t projected_points[N_CUBE_FACES * 3];


void setup(void)
{
	color_buffer = (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);
 
	if (!color_buffer)
	{
		fprintf(stderr, "an error occurred trying to allocate memory\n");
		is_running = false;
	}

	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height);

	// load_cube_mesh_data();
	load_obj_mesh_data("./assets/f22.obj");
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

/**
 * performs a perspective projection in the point passed as argument 
 * \return the projected value of the point
 */
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
	
	// initializes the array of triangles to render
	triangles_to_render = NULL;

	// add rotation on every axis of the cube
	mesh.rotation.x += 0.01;
	mesh.rotation.y += 0.01;
	mesh.rotation.z += 0.0;

	int num_faces = array_length(mesh.faces);

	// loop all the faces of the cube
	for(int i = 0; i < num_faces; i++){
		face_t face = mesh.faces[i];
		
		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[face.a - 1];
		face_vertices[1] = mesh.vertices[face.b - 1];
		face_vertices[2] = mesh.vertices[face.c - 1];


		vec3_t transformed_vertices[3];
		
		//loops all the vertices in this face and transform them
		for(int j = 0; j < 3; j++){
			vec3_t transformed_vertex = face_vertices[j];
			transformed_vertex = vector_rotate_x(transformed_vertex, mesh.rotation.x);
			transformed_vertex = vector_rotate_y(transformed_vertex, mesh.rotation.y);
			transformed_vertex = vector_rotate_z(transformed_vertex, mesh.rotation.z);
			
			transformed_vertex.z += 5;
			
			transformed_vertices[j] = transformed_vertex;
		}
		
		
		// performs back face culling
		vec3_t vector_a = transformed_vertices[0];
		vec3_t vector_b = transformed_vertices[1];
		vec3_t vector_c = transformed_vertices[2];

		// gets the vectors of the subtractions B-A and C-A
		vec3_t vector_ab = vec3_sub(vector_b, vector_a);
		vec3_t vector_ac = vec3_sub(vector_c, vector_a);
		vec3_normalize(&vector_ab);
		vec3_normalize(&vector_ac);

		// gets the normal vector of the face by performing the cross product of the vectors between the vertices B and C to A.
		vec3_t face_normal = vec3_cross(vector_ab, vector_ac);

		// normalize the face normal vector
		vec3_normalize(&face_normal);

		// gets the vector of the distance between the camera position and the vertice a of the face
		vec3_t camera_ray = vec3_sub(camera_position, vector_a);

		// gets the dot product between the vector of the distance between camera position and the a vertice and the face normal vector 
		float camera_dot_normal = vec3_dot(camera_ray, face_normal);

		if(camera_dot_normal < 0) {
			continue;
		}


		triangle_t projected_triangle;

		//apply the projection for every vertex in this face
		for(int j = 0; j < 3; j++){

			vec2_t projected_vertex = project(transformed_vertices[j]);

			projected_vertex.x += (window_width / 2);
			projected_vertex.y += (window_height / 2);

			projected_triangle.points[j] = projected_vertex;
		}

		array_push(triangles_to_render, projected_triangle);
	}
}

void render(void)
{
	draw_grid(10);

	// int num_triangles = array_length(triangles_to_render);
	// for (int i = 0; i < num_triangles; i++)
	// {
	// 	triangle_t triangle = triangles_to_render[i];
	// 	draw_rect(triangle.points[0].x, triangle.points[0].y, 4, 4, 0xFFFFFF00);
	// 	draw_rect(triangle.points[1].x, triangle.points[1].y, 4, 4, 0xFFFFFF00);
	// 	draw_rect(triangle.points[2].x, triangle.points[2].y, 4, 4, 0xFFFFFF00);

	// 	draw_triangle(triangle.points[0], triangle.points[1], triangle.points[2], 0xFF00FF00);
	// }

	vec2_t v1 = {300, 100};
	vec2_t v2 = {50, 400};
	vec2_t v3 = {500, 700};
	draw_filled_triangle(300, 100, 50, 400, 500, 700, 0xFF00FF00);

	// clear the array of triangles to render
	array_free(triangles_to_render);

	render_color_buffer();
	clear_color_buffer(0x00000000);

	SDL_RenderPresent(renderer);
}

/**
 * free the resources dynamically allocated by the program
 */
void free_resources(void){
	free(color_buffer);
	array_free(mesh.faces);
	array_free(mesh.vertices);
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
	free_resources();

	return 0;
}
