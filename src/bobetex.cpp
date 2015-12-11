#include <GL/glew.h>

#include <string>
#include <algorithm>
#include <iostream>
#include <SDL.h>
#include <SDL_opengl.h>

#include "duktape.h"
#include "json.hpp"

using json = nlohmann::json;

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
			json quitEvent = {
				{"quit", json({})}
			};

			duk_push_string(ctx, quitEvent.dump().c_str());
			duk_json_decode(ctx, -1);

			return 1;
		} else if (e.type == SDL_MOUSEMOTION) {
			json motionEvent = {
				{"motion", {
					{"x", std::to_string(e.motion.x)},
					{"y", std::to_string(e.motion.y)},
					{"xrel", std::to_string(e.motion.xrel)},
					{"yrel", std::to_string(e.motion.yrel)}
				}}
			};

			duk_push_string(ctx, motionEvent.dump().c_str());
			duk_json_decode(ctx, -1);

			return 1;
		} else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
			std::string button = "";
			std::string state = "";
			std::string direction = "";

			if (e.button.state == SDL_PRESSED) {
				state = "pressed";
			} else {
				state = "released";
			}

			if (e.button.button == SDL_BUTTON_LEFT) {
				button = "left";
			} else if (e.button.button == SDL_BUTTON_RIGHT) {
				button = "right";
			} else {
				button = "middle";
			}

			json buttonEvent = {
				{"button", {
					{"button", button},
					{"state", state},
					{"x", e.button.x},
					{"y", e.button.y}		
				}}
			};

			duk_push_string(ctx, buttonEvent.dump().c_str());
			duk_json_decode(ctx, -1);
			
			return 1;
		} else if (e.type == SDL_KEYDOWN  || e.type == SDL_KEYUP) {
			std::string state = "";
			if (e.key.state == SDL_PRESSED) {
				state = "pressed";
			} else {
				state = "released";
			}

			std::string keyName(SDL_GetKeyName(e.key.keysym.sym));
			std::transform(keyName.begin(), keyName.end(), keyName.begin(), ::tolower);

			json keyEvent = {
				{"key", {
					{"state", state},
					{"name", keyName}
				}}
			};

			duk_push_string(ctx, keyEvent.dump().c_str());
			duk_json_decode(ctx, -1);
			
			return 1;
		}
	}

	return 0;
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

	// Clean up
	duk_destroy_heap(ctx);

	SDL_GL_DeleteContext(context);

	SDL_Quit();

	return 0;
}
