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
#include "camera.h"
#include "clipping.h"

#define PI 3.14159265358979323846

#define MAX_TRIANGLES_TO_RENDER 10000

// define an array of triangles to be rendered on screen
triangle_t triangles_to_render[MAX_TRIANGLES_TO_RENDER];
int num_triangles_to_render = 0;

///////////////////////////////////////////////////////////////////////////////
//DECLARE THE BASIC MATRICES 
//////////////////////////////////////////////////////////////////////////////
mat4_t projection_matrix;
mat4_t view_matrix;
mat4_t world_matrix;

///////////////////////////////////////////////////////////////////////////////
//DECLARE VARIABLES RELATED TO THE GAME LOOP 
//////////////////////////////////////////////////////////////////////////////
bool is_running = false;
float delta_time = 0;

int previous_frame = 0;

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
	init_light(vec3_new(0, 0, 1));

	float aspect_y = (float)get_window_height() / (float)get_window_width();
	float aspect_x = (float)get_window_width() / (float)get_window_height();

	// initialize the perspective matrix to multiply by the vertices during projection.
	float fov_y = PI / 3.0; // the same as 180/3, or 60 degrees

	// calculates the fov of the x axis https://courses.pikuma.com/courses/take/learn-computer-graphics-programming/lessons/15961854-horizontal-vertical-fov-angles
	float fov_x = atan(tan(fov_y / 2) * aspect_x) * 2.0; 

	float znear = 1.0;
	float zfar = 20.0;
	projection_matrix = mat4_make_perspective(aspect_y, fov_y, zfar, znear);

	// initialize the frustrum planes with a field of view a near and a far
	initialize_frustrum_planes(fov_x, fov_y, znear, zfar);

	cull_method = CULL_BACKFACE;
	render_method = RENDER_FILL_TRIANGLE;

	// loads the hardcoded brick texture to a uint32_t pointer.
	// mesh_texture = (uint32_t*) REDBRICK_TEXTURE;

	// load_cube_mesh_data();
	load_obj_mesh_data("./assets/f22.obj");

	load_png_texture_data("./assets/f22.png");
}

void proccess_input(void)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
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
			if (event.key.keysym.sym == SDLK_1){
				render_method = RENDER_WIRE;
				break;
			}
			if (event.key.keysym.sym == SDLK_2){
				render_method = RENDER_WIRE_VERTEX;
				break;
			}
			if (event.key.keysym.sym == SDLK_3){
				render_method = RENDER_FILL_TRIANGLE;
				break;
			}
			if (event.key.keysym.sym == SDLK_4){
				render_method = RENDER_FILL_TRIANGLE_WIRE;
				break;
			}
			if (event.key.keysym.sym == SDLK_5){
				render_method = RENDER_TEXTURED;
				break;
			}
			if (event.key.keysym.sym == SDLK_6){
				render_method = RENDER_TEXTURED_WIRE;
				break;
			}
			if (event.key.keysym.sym == SDLK_c) {
				cull_method = CULL_BACKFACE;
				break;
			}
			if (event.key.keysym.sym == SDLK_b){
				cull_method = CULL_NONE;
				break;
			}
			if (event.key.keysym.sym == SDLK_q){
				update_camera_position_y(get_camera_position().y - 3.0 * delta_time);
				break;
			}
			if (event.key.keysym.sym == SDLK_e){
				update_camera_position_y(get_camera_position().y + 3.0 * delta_time);
				break;
			}
			if (event.key.keysym.sym == SDLK_a){
				update_camera_yaw(get_camera_yaw() + 1.0f * delta_time);
				break;
			}
			if (event.key.keysym.sym == SDLK_d){
				update_camera_yaw(get_camera_yaw() - 1.0f * delta_time);
				break;
			}
				if (event.key.keysym.sym == SDLK_UP){
				update_camera_pitch(get_camera_pitch() + 1.0f * delta_time);
				break;
			}
			if (event.key.keysym.sym == SDLK_DOWN){
				update_camera_pitch(get_camera_pitch() - 1.0f * delta_time);
				break;
			}

			if (event.key.keysym.sym == SDLK_w)
			{
				update_camera_forward_velocity(vec3_mul(get_camera_direction(), 5.0 * delta_time));
				update_camera_position(vec3_add(get_camera_position(), get_camera_forward_velocity())); 
				break;
			}

			if (event.key.keysym.sym == SDLK_s)
			{
				update_camera_forward_velocity(vec3_mul(get_camera_direction(), 5.0 * delta_time));
				update_camera_position(vec3_sub(get_camera_position(), get_camera_forward_velocity())); 
				break;
			}

			break;
		}
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

	// get the delta time factor converted to seconds to multiply the update the game objects.
	delta_time = (SDL_GetTicks() - previous_frame) / 1000.0f;

	previous_frame = SDL_GetTicks();

	// initializes the count of triangles to render for the current frame
	num_triangles_to_render = 0;

	// add rotation on every axis of the cube
	// mesh.rotation.x += 0.005 * delta_time;
	// mesh.rotation.y += 0.01 * delta_time;
	// mesh.rotation.z += 0.01 * delta_time;

	// increases the scale of the mesh in every frame
	// mesh.scale.x += 0.002 * delta_time;

	// Increases the translation every frame
	// mesh.translation.x += 0.01 * delta_time;

	// Sets the translation in z to represent the camera depth.
	mesh.translation.z = 5.0;

	// move the camera to the right every frame.
	// camera.position.y -= -0.008 * delta_time;

	//////////// creates the view (or camera) matrix. //////////////////
	
	// finds the target position based on the yaw and pitch rotation of the camera.
	vec3_t target = get_camera_lookat_target();	
	vec3_t up = {0, 1, 0};
	view_matrix = mat4_look_at(get_camera_position(), target, up);

	// creates the scale, translation and rotation matrices to mutiply the mesh vertices
	mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
	mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);

	// creates the rotation matrices for x, y and z axis.
	mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh.rotation.x);
	mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh.rotation.y);
	mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh.rotation.z);
	
	int num_faces = array_length(mesh.faces);

	// loop all the faces of the cube
	for (int i = 0; i < num_faces; i++)
	{

		// if (i != 4) continue;

		face_t face = mesh.faces[i];

		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[face.a];
		face_vertices[1] = mesh.vertices[face.b];
		face_vertices[2] = mesh.vertices[face.c];

		vec4_t transformed_vertices[3];

		// loops all the vertices in this face and transform them
		for (int j = 0; j < 3; j++)
		{
			///////////////////////////// IMPORTANT NOTE!! ///////////////////////////////////
			// the order of transformation operations needs to be scale -> rotate -> translate.
			/////////////////////////////////////////////////////////////////////////////////

			vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);

			// Create a world matrix to combine all transformations in a single matrix and multiply the vertex by it.
			world_matrix = mat4_identity();

			// multiply by the scale matrix.
			world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);

			// multiply by the rotation matrices.
			world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);

			// multiply by the translation matrix.
			world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

			// Use the translation matrix to move the vertex.
			transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

			// multiply the transformed vector by the view matrix to transform the scene to view(or camera) space.
			transformed_vertex = mat4_mul_vec4(view_matrix, transformed_vertex);

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
		vec3_t origin = {0, 0, 0};
		vec3_t camera_ray = vec3_sub(origin, vector_a);

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

		/* create a polygon of the transformed vertices to start the clipping process */
		polygon_t polygon = create_polygon_from_triangle(
			vec3_from_vec4(transformed_vertices[0]), 
			vec3_from_vec4(transformed_vertices[1]), 
			vec3_from_vec4(transformed_vertices[2]),
			face.a_uv,
			face.b_uv,
			face.c_uv
		);

		// clip the polygon and transform it to a new polygon with potentially new vertices.
		clip_polygon(&polygon);

		triangle_t triangles_after_clipping[MAX_NUM_TRIANGLES];
		int num_triangles_after_clipping = 0;

		create_triangle_from_polygon(&polygon, triangles_after_clipping, &num_triangles_after_clipping);

		for (int t = 0; t < num_triangles_after_clipping; t++)
		{
			triangle_t triangle_after_clipping = triangles_after_clipping[t];

			vec4_t projected_vertex[3];

			// apply the projection for every vertex in this face
			for (int j = 0; j < 3; j++)
			{
				// make the perspective projection
				projected_vertex[j] = mat4_mult_vec4_project(projection_matrix, triangle_after_clipping.points[j]);

				// invert the y value to account for the screen y coord.
				projected_vertex[j].y *= -1;
				// projected_vertex[j].x *= -1;

				// scales the vertices into the screen.
				projected_vertex[j].x *= (get_window_width() / 2.0);
				projected_vertex[j].y *= (get_window_height() / 2.0);

				// translates the vertices to the middle of the screen.
				projected_vertex[j].x += (get_window_width() / 2.0);
				projected_vertex[j].y += (get_window_height() / 2.0);
			}

			/* calculate the light intensity on the face, based on the angle between the light direction and the face normal. */

			float light_percentage = -vec3_dot(face_normal, get_light_direction());

			uint32_t face_color = light_apply_intensity(face.color, light_percentage);

			triangle_t triangle_to_render = {
				.points = {
					{projected_vertex[0].x, projected_vertex[0].y, projected_vertex[0].z, projected_vertex[0].w},
					{projected_vertex[1].x, projected_vertex[1].y, projected_vertex[1].z, projected_vertex[1].w},
					{projected_vertex[2].x, projected_vertex[2].y, projected_vertex[2].z, projected_vertex[2].w}
				},
				.texcoords = {
					{triangle_after_clipping.texcoords[0].u, triangle_after_clipping.texcoords[0].v}, 
					{triangle_after_clipping.texcoords[1].u, triangle_after_clipping.texcoords[1].v}, 
					{triangle_after_clipping.texcoords[2].u, triangle_after_clipping.texcoords[2].v}
				},
				.color = face_color,
				.light_percentage = 1.0
			};

			if (num_triangles_to_render < MAX_TRIANGLES_TO_RENDER) {
				triangles_to_render[num_triangles_to_render] = triangle_to_render;
				num_triangles_to_render++;
            }
		}
	}
}

void render(void)
{
	clear_color_buffer(0xFF000000);
	clear_z_buffer();

	draw_grid(10);

	for (int i = 0; i < num_triangles_to_render; i++)
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
				triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w, // vertex A
				triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w, // vertex B
				triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w, // vertex C
				triangle.color);
		}

		if(render_method == RENDER_TEXTURED || render_method == RENDER_TEXTURED_WIRE){
			draw_textured_triangle(
				triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w, triangle.texcoords[0].u, triangle.texcoords[0].v, // vertex A
				triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w, triangle.texcoords[1].u, triangle.texcoords[1].v, // vertex B
				triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w, triangle.texcoords[2].u, triangle.texcoords[2].v, // vertex C
				mesh_texture, triangle.light_percentage
			);
		}

		if (render_method == RENDER_WIRE_VERTEX || render_method == RENDER_WIRE || render_method == RENDER_FILL_TRIANGLE_WIRE || render_method == RENDER_TEXTURED_WIRE)
		{
			// draw the wireframe of the triangle
			// draw_triangle_wire(triangle.points[0], triangle.points[1], triangle.points[2], 0xFFFFFFFF);
			  draw_triangle(
                triangle.points[0].x, triangle.points[0].y, // vertex A
                triangle.points[1].x, triangle.points[1].y, // vertex B
                triangle.points[2].x, triangle.points[2].y, // vertex C
                0xFFFFFFFF
            );
		}

	}


	render_color_buffer();
}

/**
 * free the resources dynamically allocated by the program
 */
void free_resources(void)
{
	upng_free(png_texture);
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
