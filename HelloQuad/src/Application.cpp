
#include "Application.h"
#include <iostream>
#include <vector>

#include "GL\glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "ShaderFuncs.h"

GLfloat vertexPositions[] = {
	0.75f, 0.75f, 0.0f, 1.0f,
	0.75f, -0.75f, 0.0f, 1.0f,
	-0.75f, -0.75f, 0.0f, 1.0f,

	-0.75f, 0.75f, 0.0f, 1.0f,
	0.75f, 0.75f, 0.0f, 1.0f,
	-0.75f, -0.75f, 0.0f, 1.0f,
};


GLfloat vertexColors[] = {
	1.0, 0.0, 0.0, 1.0,
	0.0, 1.0, 0.0, 1.0,
	0.0, 0.0, 1.0, 1.0,
};


Application::Application() 
{}

Application::~Application() 
{}

void Application::setup()
{
	std::string strVertexShader = loadTextFile("shaders/vertexShader.vs");
	std::string strFragmentShader = loadTextFile("shaders/fragmentShader.fs");
	InitializeProgram(triangle.shader, strVertexShader, strFragmentShader);

	timeId = glGetUniformLocation(triangle.shader, "time");

	triangle.numVertex = 6;
	glGenVertexArrays(1, &triangle.vao);
	glBindVertexArray(triangle.vao);
	glGenBuffers(1, &triangle.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, triangle.vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions)+sizeof(vertexColors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0/*offset*/, sizeof(vertexPositions), vertexPositions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexPositions), sizeof(vertexColors), vertexColors);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(vertexPositions));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void Application::update()
{
	++time;
	std::cout << time << std::endl;
	glUniform1i(timeId, time);
}


void Application::display()
{
	//Borramos el buffer de color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//Seleccionamos los shaders a usar
	glUseProgram(triangle.shader);	
	
	//activamos el vertex array object y dibujamos
	glBindVertexArray(triangle.vao);

	glDrawArrays(GL_TRIANGLES, 0, triangle.numVertex);
}

void Application::reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}
