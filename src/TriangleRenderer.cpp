#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>

#include "TriangleRenderer.hpp"
#include "utils.hpp"
#include "gl.hpp"
#include "globals.hpp"

// void TriangleRenderer::addSolidTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b) {
// 	floats.push_back(x1);
// 	floats.push_back(y1);
// 
// 	floats.push_back(r/255.0);
// 	floats.push_back(g/255.0);
// 	floats.push_back(b/255.0);
// 
// 	floats.push_back(x2);
// 	floats.push_back(y2);
// 
// 	floats.push_back(r/255.0);
// 	floats.push_back(g/255.0);
// 	floats.push_back(b/255.0);
// 
// 	floats.push_back(x3);
// 	floats.push_back(y3);
// 
// 	floats.push_back(r/255.0);
// 	floats.push_back(g/255.0);
// 	floats.push_back(b/255.0);
// }

void TriangleRenderer::addTricolorTriangle(float x1, float y1, float r1, float g1, float b1,
		float x2, float y2, float r2, float g2, float b2,
		float x3, float y3, float r3, float g3, float b3) {

	floats.push_back(x1);
	floats.push_back(y1);
	floats.push_back(r1/255.0);
	floats.push_back(g1/255.0);
	floats.push_back(b1/255.0);

	floats.push_back(x2);
	floats.push_back(y2);
	floats.push_back(r2/255.0);
	floats.push_back(g2/255.0);
	floats.push_back(b2/255.0);

	floats.push_back(x3);
	floats.push_back(y3);
	floats.push_back(r3/255.0);
	floats.push_back(g3/255.0);
	floats.push_back(b3/255.0);
}

void TriangleRenderer::addTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
	floats.push_back(x1);
	floats.push_back(y1);
	floats.push_back(currR);
	floats.push_back(currG);
	floats.push_back(currB);

	floats.push_back(x2);
	floats.push_back(y2);
	floats.push_back(currR);
	floats.push_back(currG);
	floats.push_back(currB);

	floats.push_back(x3);
	floats.push_back(y3);
	floats.push_back(currR);
	floats.push_back(currG);
	floats.push_back(currB);
}

void TriangleRenderer::setColor(float r, float g, float b) {
	currR = r / 255;
	currG = g / 255;
	currB = b / 255;
}

void TriangleRenderer::render() {
	glBindBuffer(GL_ARRAY_BUFFER, triangleBuffer);
	glBufferData(GL_ARRAY_BUFFER, floats.size() * sizeof(float), floats.data(), GL_STREAM_DRAW);

	glBindVertexArray(vao);
	glUseProgram(shaderProgram);
	glDrawArrays(GL_TRIANGLES, 0, floats.size() / 5);

	floats.clear();
}

void TriangleRenderer::setup() {
	if (setupDone) return;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glGenBuffers(1, &triangleBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, triangleBuffer);

	auto vertexSource = readFile("media/triangles.vertex");
	auto vertexSourcePtr = vertexSource.c_str();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSourcePtr, NULL);
	glCompileShader(vertexShader);
	checkShaderCompilation(vertexShader);

	auto fragmentSource = readFile("media/triangles.fragment");
	auto fragmentSourcePtr = fragmentSource.c_str();
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSourcePtr, NULL);
	glCompileShader(fragmentShader);
	checkShaderCompilation(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	GLint linked;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);
	if (!linked) { 
		std::cout << "Linking unsuccesful\n";
	}

	glUseProgram(shaderProgram);

	posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(posAttrib);

	colorAttrib = glGetAttribLocation(shaderProgram, "color");
	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(colorAttrib);

	uniScrW = glGetUniformLocation(shaderProgram, "scrW");
	glUniform1i(uniScrW, SCR_W);

	uniScrH = glGetUniformLocation(shaderProgram, "scrH");
	glUniform1i(uniScrH, SCR_H);

	GLuint code = glGetError();
	if (code != 0) {
		std::cout << "TriangleRenderer: Something went wrong. Code: " << code << "\n";
	} else {
		std::cout << "TriangleRenderer: No OpenGL problems" << "\n";
	}

	setupDone = true;
}
