#include <iostream>
#include <algorithm>

#include "globals.hpp"
#include "jsapi.hpp"
#include "json.hpp"
#include "TriangleRenderer.hpp"
#include "utils.hpp"
#include "randutil.h"

using json = nlohmann::json;

TriangleRenderer triangles;

duk_ret_t btx_flip(duk_context *ctx) {
	glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	triangles.render();

	SDL_GL_SwapWindow(window);

	return 0;
}

// Expects from javascript: x1, y1, x2, y2, x3, y3
duk_ret_t btx_triangle(duk_context *ctx) {
	float x1 = duk_require_number(ctx, -6);
	float y1 = duk_require_number(ctx, -5);
	float x2 = duk_require_number(ctx, -4);
	float y2 = duk_require_number(ctx, -3);
	float x3 = duk_require_number(ctx, -2);
	float y3 = duk_require_number(ctx, -1);

	triangles.addTriangle(x1, y1, x2, y2, x3, y3);

	return 0;
}

// Expects from javascript: r, g, b
duk_ret_t btx_triangle_color(duk_context *ctx) {
	float r = duk_require_number(ctx, -3);
	float g = duk_require_number(ctx, -2);
	float b = duk_require_number(ctx, -1);

	triangles.setColor(r, g, b);

	return 0;
}

// Expects from javascript:
// x1, y1, r1, g1, b1
// x2, y2, r2, g2, b2
// x3, y3, r3, g3, b3
duk_ret_t btx_tricolor_triangle(duk_context *ctx) {
	float x1 = duk_require_number(ctx, -15);
	float y1 = duk_require_number(ctx, -14);
	float r1 = duk_require_number(ctx, -13);
	float g1 = duk_require_number(ctx, -12);
	float b1 = duk_require_number(ctx, -11);

	float x2 = duk_require_number(ctx, -10);
	float y2 = duk_require_number(ctx, -9);
	float r2 = duk_require_number(ctx, -8);
	float g2 = duk_require_number(ctx, -7);
	float b2 = duk_require_number(ctx, -6);

	float x3 = duk_require_number(ctx, -5);
	float y3 = duk_require_number(ctx, -4);
	float r3 = duk_require_number(ctx, -3);
	float g3 = duk_require_number(ctx, -2);
	float b3 = duk_require_number(ctx, -1);

	triangles.addTricolorTriangle(x1, y1, r1, g1, b1,
			x2, y2, r2, g2, b2,
			x3, y3, r3, g3, b3);

	return 0;
}

// Expects from javascript: top, right, bottom, left
duk_ret_t btx_rect(duk_context *ctx) {
	float top = duk_require_number(ctx, -4);
	float right = duk_require_number(ctx, -3);
	float bottom = duk_require_number(ctx, -2);
	float left = duk_require_number(ctx, -1);

	triangles.addTriangle(left, top, right, top, left, bottom);
	triangles.addTriangle(right, bottom, left, bottom, right, top);

	return 0;
}

// Expects from javascript: x1, y1, x2, y2, x3, y3, x4, y4
duk_ret_t btx_quad(duk_context *ctx) {
	float x1 = duk_require_number(ctx, -8);
	float y1 = duk_require_number(ctx, -7);
	float x2 = duk_require_number(ctx, -6);
	float y2 = duk_require_number(ctx, -5);
	float x3 = duk_require_number(ctx, -4);
	float y3 = duk_require_number(ctx, -3);
	float x4 = duk_require_number(ctx, -2);
	float y4 = duk_require_number(ctx, -1);

	triangles.addTriangle(x1, y1, x2, y2, x3, y3);
	triangles.addTriangle(x3, y3, x4, y4, x1, y1);

	return 0;
}

duk_ret_t btx_set_clear_color(duk_context *ctx) {
	float r = duk_require_number(ctx, -3);
	float g = duk_require_number(ctx, -2);
	float b = duk_require_number(ctx, -1);

	clearColor[0] = r / 255.0;
	clearColor[1] = g / 255.0;
	clearColor[2] = b / 255.0;

	return 0;
}

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

	SCR_W = scrW;
	SCR_H = scrH;

	window = SDL_CreateWindow(buf, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCR_W, SCR_H, SDL_WINDOW_OPENGL);

	// Init OpenGL stuff after creation of window
	context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	glewInit();

	glGetError();

	triangles.setup();

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
					{"x", e.motion.x},
					{"y", SCR_H - e.motion.y},
					{"xrel", e.motion.xrel},
					{"yrel", e.motion.yrel}
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

duk_ret_t btx_get_ticks(duk_context *ctx) {
	unsigned int ticks = SDL_GetTicks();

	duk_push_int(ctx, ticks);

	return 1;
}

duk_ret_t btx_delay(duk_context *ctx) {
	int duration = duk_require_number(ctx, -1);
	SDL_Delay(duration);

	return 0;
}

// Expects from javascript: filename string
duk_ret_t btx_include(duk_context *ctx) {
	std::string filename(duk_require_string(ctx, -1));
	filename = "scripts/" + filename;

	if (duk_peval_file(ctx, filename.c_str()) != 0) {
		std::cout << "Error interpreting " << filename << ": " << duk_safe_to_string(ctx, -1) << "\n";
		duk_push_string(ctx, "Error interpreting include");
		duk_throw(ctx);
		// This call doesn't return
	}

	duk_pop(ctx);

	return 0;
}

// Expects from javascript: start, end (inclusive)
randutils::mt19937_rng rng;
duk_ret_t btx_randrange(duk_context *ctx) {
	int left = duk_require_number(ctx, -2);
	int right = duk_require_number(ctx, -1);

	int result = rng.uniform(left, right);

	duk_push_int(ctx, result);

	return 1;
}

// Expects from javascript: nothing
duk_ret_t btx_uniform(duk_context *ctx) {
	float result = rng.uniform(0.0, 1.0);

	duk_push_number(ctx, result);

	return 1;
}

