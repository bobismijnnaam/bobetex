#include <GL/glew.h>

#include <string>
#include <algorithm>
#include <iostream>
#include <SDL.h>
#include <SDL_opengl.h>

#include "duktape.h"
#include "json.hpp"
#include "jsapi.hpp"
#include "utils.hpp"
#include "globals.hpp"

using json = nlohmann::json;

int main() {
	// Set up SDL & OpenGL
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Initialize Duktape
	duk_context *ctx = NULL;

	ctx = duk_create_heap_default();
	if (!ctx) {
		std::cout << "Failed to create Duktape heap\n";
		return 1;
	}

	// Build global object Btx
	duk_push_global_object(ctx);

	duk_idx_t id = duk_push_object(ctx);

	duk_function_list_entry const btx_generic_funcs[] = {
		{"flip", btx_flip, 0},
		{"triangle", btx_triangle, 6},
		{"triangleColor", btx_triangle_color, 3},
		{"tricolorTriangle", btx_tricolor_triangle, 15},
		{"rect", btx_rect, 4},
		{"quad", btx_quad, 8},
		{"setClearColor", btx_set_clear_color, 3},
		{"createWindow", btx_create_window, 3},
		{"pollEvent", btx_poll_event, 0},
		{"sanity", btx_sanity, 0},
		{"getTicks", btx_get_ticks, 0},
		{"delay", btx_delay, 1},
		{"include", btx_include, 1},
		{"randrange", btx_randrange, 2},
		{"uniform", btx_uniform, 0},
		{NULL, NULL, 0}
	};
	duk_put_function_list(ctx, id, btx_generic_funcs);

	duk_put_prop_string(ctx, id - 1, "btx");
	
	// Run program
	if (duk_peval_file(ctx, "scripts/main.js") != 0) {
		std::cout << "Error interpreting main.js: " << duk_safe_to_string(ctx, -1) << "\n";
		return 2;
	}
	duk_pop(ctx);

	// Clean up
	duk_destroy_heap(ctx);

	SDL_GL_DeleteContext(context);

	SDL_Quit();

	return 0;
}
