#include <GL/glew.h>

#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_opengl.h>

#include "duktape.h"

SDL_Window *window = nullptr;
SDL_GLContext context;

// Expects from javascript: windowtitle, width, height
duk_ret_t btx_create_window(duk_context *ctx) {
	if (window != nullptr) {
		duk_push_string(ctx, "window has already been created");
		duk_throw(ctx);
		// This call never returns
	}

	char const *buf;
	buf = duk_require_string(ctx, 0);

	int scrW, scrH;
	scrW = duk_require_int(ctx, 1);
	scrH = duk_require_int(ctx, 2);

	window = SDL_CreateWindow(buf, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, scrW, scrH, SDL_WINDOW_OPENGL);

	return 0;
}

// Expects from javascript: nothing
duk_ret_t btx_poll_event(duk_context *ctx) {
	SDL_Event e;
	if (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			duk_push_string(ctx, "BTX_QUIT");
		} else {
			duk_push_string(ctx, "BTX_EVENT");
		}

		return 1;
	} else {
		return 0;
	}
}

duk_ret_t btx_sanity(duk_context *ctx) {
	std::cout << "Donkey poo.\n";

	return 0;
}

int main() {
	// Set up SDL & OpenGL
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	context = SDL_GL_CreateContext(window);

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
		{"createWindow", btx_create_window, 3},
		{"pollEvent", btx_poll_event, 0},
		{"sanity", btx_sanity, 0},
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

	// duk_push_global_object(ctx);
	// duk_get_prop_string(ctx, -1, "main");
	// duk_call(ctx, 0);
	// for (int i = 0; i < 10; i++) {
	// 	duk_push_string(ctx, "printAndInc");
	// 	if (duk_pcall_prop(ctx, -2, 0) != 0) {
	// 		std::cout << "Error executing function: " << duk_safe_to_string(ctx, -1) << "\n";
	// 		return 3;
	// 	}
	// 	duk_pop(ctx); // Ignore return value "undefined"
	// }

	// Clean up
	duk_destroy_heap(ctx);

//	SDL_Event windowEvent;
//	while (true)
//	{
//		if (SDL_PollEvent(&windowEvent))
//		{
//			if (windowEvent.type == SDL_QUIT) break;
//		}
//
//		SDL_GL_SwapWindow(window);
//	}

	SDL_GL_DeleteContext(context);

	SDL_Quit();

	return 0;
}
