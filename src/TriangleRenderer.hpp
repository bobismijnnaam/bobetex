#ifndef TRIANGLERENDERER_HPP
#define TRIANGLERENDERER_HPP

#include <vector>

class TriangleRenderer {
public:
	void addTricolorTriangle(float x1, float y1, float r1, float g1, float b1,
		float x2, float y2, float r2, float g2, float b2,
		float x3, float y3, float r3, float g3, float b3);
	void addTriangle(float x1, float y1, float x2, float y2, float x3, float y3);

	void setColor(float r, float g, float b);
	void render();

	void setup();

private:
	GLuint vao;
	GLuint triangleBuffer;	
	GLuint shaderProgram;
	GLuint vertexShader, fragmentShader;
	GLint posAttrib, colorAttrib;
	GLint uniScrW, uniScrH, uniColor;
	
	float currR = 1;
	float currG = 1;
	float currB = 1;

	bool setupDone = false;

	std::vector<float> floats;
} ;

#endif
