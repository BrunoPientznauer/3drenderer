#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "display.h"
#include "vectors.h"
#include "mesh.h"
#include "array.h"
#include "matrix.h"
#include "light.h"
#include "texture.h"

#define PI 3.14159265358979323846

// define an array of triangles to be rendered on screen
triangle_t *triangles_to_render = NULL;

vec3_t camera_position = {0, 0, 0};

light_t directional_light = {.direction = {0, 0, 1}};

mat4_t projection_matrix;

bool is_running = false;

vec2_t projected_points[N_CUBE_FACES * 3];

enum cull_method
{
	CULL_NONE,
	CULL_BACKFACE
} cull_method;

enum render_method
{
	RENDER_WIRE,
	RENDER_WIRE_VERTEX,
	RENDER_FILL_TRIANGLE,
	RENDER_FILL_TRIANGLE_WIRE,
	RENDER_TEXTURED,
	RENDER_TEXTURED_WIRE
} render_method;

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

	// initialize the perspective matrix to multiply by the vertices during projection.
	float fov = PI / 3.0; // the same as 180/3, or 60 degrees
	float aspect = (float)window_height / (float)window_width;
	float znear = 0.1;
	float zfar = 100.0;
	projection_matrix = mat4_make_perspective(aspect, fov, znear, zfar);

	cull_method = CULL_BACKFACE;
	render_method = RENDER_FILL_TRIANGLE;

	// loads the hardcoded brick texture to a uint32_t pointer.
	mesh_texture = (uint32_t*) REDBRICK_TEXTURE;

	load_cube_mesh_data();
	// load_obj_mesh_data("./assets/f22.obj");
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
		{
			is_running = false;
		}

		if (event.key.keysym.sym == SDLK_1)
			render_method = RENDER_WIRE;
		if (event.key.keysym.sym == SDLK_2)
			render_method = RENDER_WIRE_VERTEX;
		if (event.key.keysym.sym == SDLK_3)
			render_method = RENDER_FILL_TRIANGLE;
		if (event.key.keysym.sym == SDLK_4)
			render_method = RENDER_FILL_TRIANGLE_WIRE;
		if (event.key.keysym.sym == SDLK_5)
			render_method = RENDER_TEXTURED;
		if (event.key.keysym.sym == SDLK_6)
			render_method = RENDER_TEXTURED_WIRE;
		if (event.key.keysym.sym == SDLK_c)
			cull_method = CULL_BACKFACE;
		if (event.key.keysym.sym == SDLK_d)
			cull_method = CULL_NONE;

		break;
	}
}

void update(void)
{
	// lock the program fps
	int time_to_wait = TARGET_FRAME_TIME - (SDL_GetTicks() - previous_frame);

	if (time_to_wait > 0 && time_to_wait <= TARGET_FRAME_TIME)
	{
		SDL_Delay(time_to_wait);
	}

	previous_frame = SDL_GetTicks();

	// initializes the array of triangles to render
	triangles_to_render = NULL;

	// add rotation on every axis of the cube
	mesh.rotation.x += 0.01;
	// mesh.rotation.y += 0.01;
	// mesh.rotation.z += 0.01;

	// increases the scale of the mesh in every frame
	// mesh.scale.x += 0.002;

	// Increases the translation every frame
	// mesh.translation.x += 0.01;

	// Sets the translation in z to represent the camera depth.
	mesh.translation.z = 5.0;

	// creates the scale, translation and rotation matrices to mutiply the mesh vertices
	mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
	mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);

	// creates the rotation matrices for x, y and z axis.
	mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh.rotation.z);
	mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh.rotation.x);
	mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh.rotation.y);

	int num_faces = array_length(mesh.faces);

	// loop all the faces of the cube
	for (int i = 0; i < num_faces; i++)
	{
		face_t face = mesh.faces[i];

		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[face.a - 1];
		face_vertices[1] = mesh.vertices[face.b - 1];
		face_vertices[2] = mesh.vertices[face.c - 1];

		vec4_t transformed_vertices[3];

		// loops all the vertices in this face and transform them
		for (int j = 0; j < 3; j++)
		{
			///////// IMPORTANT NOTE!! //////////
			// the order of transformation operations needs to be scale -> rotate -> translate.
			/////////////////////////////////////

			vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);

			// Create a world matrix to combine all transformations in a single matrix and multiply the vertex by it.
			mat4_t world_matrix = mat4_identity();

			// multiply by the scale matrix.
			world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);

			// multiply by the rotation matrices.
			world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);

			// multiply by the translation matrix.
			world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

			// Use the translation matrix to move the vertex.
			transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

			transformed_vertices[j] = transformed_vertex;
		}


		/* calculates the normal of the face */

		vec3_t vector_a = vec3_from_vec4(transformed_vertices[0]);
		vec3_t vector_b = vec3_from_vec4(transformed_vertices[1]);
		vec3_t vector_c = vec3_from_vec4(transformed_vertices[2]);

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

		// performs back face culling
		if (cull_method == CULL_BACKFACE)
		{
			if (camera_dot_normal < 0)
			{
				continue;
			}
		}

		vec4_t projected_vertex[3];

		// apply the projection for every vertex in this face
		for (int j = 0; j < 3; j++)
		{
			// make the perspective projection
			projected_vertex[j] = mat4_mult_vec4_project(projection_matrix, transformed_vertices[j]);

			// scales the vertices into the screen.
			projected_vertex[j].x *= (window_width / 2.0);
			projected_vertex[j].y *= (window_height / 2.0);

			// invert the y value to account for the screen y coord.
			projected_vertex[j].y *= -1;
			projected_vertex[j].x *= -1;

			// translates the vertices to the middle of the screen.
			projected_vertex[j].x += (window_width / 2.0);
			projected_vertex[j].y += (window_height / 2.0);
		}

		// calculate the average z position of the vertice to perform the painters algorithm.
		float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3.0;


		/* calculate the light intensity on the face, based on the angle between the light direction and the face normal. */

		float light_percentage = -vec3_dot(face_normal, directional_light.direction);

		uint32_t face_color = light_apply_intensity(face.color, light_percentage);

		triangle_t projected_triangle = {
			.points = {
				{projected_vertex[0].x, projected_vertex[0].y},
				{projected_vertex[1].x, projected_vertex[1].y},
				{projected_vertex[2].x, projected_vertex[2].y}},
			.texcoords = {
				{face.a_uv.u, face.a_uv.v},
				{face.b_uv.u, face.b_uv.v},
				{face.c_uv.u, face.c_uv.v}
			},
			.color = face_color,
			.avg_depht = avg_depth};

		array_push(triangles_to_render, projected_triangle);
	}

	// Sort the triangles to render by their avg_depth
	int num_triangles = array_length(triangles_to_render);
	for (int i = 0; i < num_triangles; i++)
	{
		for (int j = i; j < num_triangles; j++)
		{
			if (triangles_to_render[i].avg_depht < triangles_to_render[j].avg_depht)
			{
				// Swap the triangles positions in the array
				triangle_t temp = triangles_to_render[i];
				triangles_to_render[i] = triangles_to_render[j];
				triangles_to_render[j] = temp;
			}
		}
	}
}

void render(void)
{
	SDL_RenderClear(renderer);

	draw_grid(10);

	int num_triangles = array_length(triangles_to_render);
	for (int i = 0; i < num_triangles; i++)
	{
		triangle_t triangle = triangles_to_render[i];

		if (render_method == RENDER_WIRE_VERTEX)
		{
			draw_rect(triangle.points[0].x, triangle.points[0].y, 4, 4, 0xFFFF0000);
			draw_rect(triangle.points[1].x, triangle.points[1].y, 4, 4, 0xFFFF0000);
			draw_rect(triangle.points[2].x, triangle.points[2].y, 4, 4, 0xFFFF0000);
		}

		if (render_method == RENDER_FILL_TRIANGLE_WIRE || render_method == RENDER_FILL_TRIANGLE)
		{
			draw_filled_triangle(
				triangle.points[0].x, triangle.points[0].y, // vertex A
				triangle.points[1].x, triangle.points[1].y, // vertex B
				triangle.points[2].x, triangle.points[2].y, // vertex C
				triangle.color);
		}

		if(render_method == RENDER_TEXTURED || render_method == RENDER_TEXTURED_WIRE){
			draw_textured_triangle(
				triangle.points[0].x, triangle.points[0].y, triangle.texcoords[0].u, triangle.texcoords[0].v, // vertex A
				triangle.points[1].x, triangle.points[1].y, triangle.texcoords[1].u, triangle.texcoords[1].v, // vertex B
				triangle.points[2].x, triangle.points[2].y, triangle.texcoords[2].u, triangle.texcoords[2].v, // vertex C
				mesh_texture
			);
		}

		if (render_method == RENDER_WIRE_VERTEX || render_method == RENDER_WIRE || render_method == RENDER_FILL_TRIANGLE_WIRE || render_method == RENDER_TEXTURED_WIRE)
		{
			// draw the wireframe of the triangle
			draw_triangle(triangle.points[0], triangle.points[1], triangle.points[2], 0xFFFFFFFF);
		}
	}

	// clear the array of triangles to render
	array_free(triangles_to_render);

	render_color_buffer();

	clear_color_buffer(0xFF000000);

	SDL_RenderPresent(renderer);
}

/**
 * free the resources dynamically allocated by the program
 */
void free_resources(void)
{
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
