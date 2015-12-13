#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>

#include "gl.hpp"

void checkShaderCompilation(int shader) {
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	char buffer[512];
	glGetShaderInfoLog(shader, 512, NULL, buffer);
	
	if (status == GL_TRUE) {
		std::cout << "Compilation of shader " << shader << " succesful\n";
	} else {
		std::cout << "Compilation unsuccesful. Error: " << buffer << "\n";
	}
}


