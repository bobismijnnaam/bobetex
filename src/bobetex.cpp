#include <string>
#include <iostream>
#include <SDL.h>

#include "duktape.h"

int main() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Quit();

	duk_context *ctx = NULL;

	ctx = duk_create_heap_default();
	if (!ctx) {
		std::cout << "Failed to create Duktape heap\n";
		return 1;
	}
	
	if (duk_peval_file(ctx, "scripts/main.js") != 0) {
		std::cout << "Error interpreting main.js: " << duk_safe_to_string(ctx, -1) << "\n";
		return 2;
	}
	duk_pop(ctx); 
	
	if (duk_peval_file(ctx, "scripts/sub.js") != 0) {
		std::cout << "Error interpreting sub.js: " << duk_safe_to_string(ctx, -1) << "\n";
		return 2;
	}
	duk_pop(ctx); 

	// TODO: Read all other js file in scripts directory

	duk_push_global_object(ctx);
	duk_get_prop_string(ctx, -1, "main");
	duk_call(ctx, 0);
	for (int i = 0; i < 10; i++) {
		duk_push_string(ctx, "printAndInc");
		if (duk_pcall_prop(ctx, -2, 0) != 0) {
			std::cout << "Error executing function: " << duk_safe_to_string(ctx, -1) << "\n";
			return 3;
		}
		duk_pop(ctx); // Ignore return value "undefined"
	}

	duk_destroy_heap(ctx);

	return 0;
}
