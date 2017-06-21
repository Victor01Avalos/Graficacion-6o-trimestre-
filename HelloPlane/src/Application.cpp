
#include "Application.h"
#include <iostream>
#include <vector>

#include "GL\glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "glm\mat4x4.hpp"
#include "glm\common.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm\vec3.hpp"


#include "ShaderFuncs.h"


Application::Application() :time(0),
							eye(0.1f,0.1,2.0f),
							target(0.0f,0.0f,0.0f)
{}

Application::~Application() 
{}

void Application::setup()
{
	plane.createPlane(100);

	std::string strVertexShader = loadTextFile("shaders/vertexShader.vs");
	std::string strFragmentShader = loadTextFile("shaders/fragmentShader.fs");
	//InitializeProgram(triangle.shader, strVertexShader, strFragmentShader);
	InitializeProgram(plane.shader, strVertexShader, strFragmentShader);

	//zona de obtencion de Id's
	//timeId = glGetUniformLocation(triangle.shader, "time");
	timeId = glGetUniformLocation(plane.shader, "time");
	//transformationId = glGetUniformLocation(triangle.shader, "transformation");
	transformationId = glGetUniformLocation(plane.shader, "transformation");


	//triangle.numVertex = 3;
	//plane.numVertex =3 ;
	//glGenVertexArrays(1, &triangle.vao);
	glGenVertexArrays(1, &plane.vao);
	//glBindVertexArray(triangle.vao);
	glBindVertexArray(plane.vao);
	//glGenBuffers(1, &triangle.vbo);
	glGenBuffers(1, &plane.vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, triangle.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, plane.vbo);
	//glBufferData(GL_ARRAY_BUFFER, plane.getVertexSizeInBytes(), vertexPositions, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, plane.getVertexSizeInBytes(), plane.plane, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions)+sizeof(vertexColors), NULL, GL_STATIC_DRAW);
	//glBufferSubData(GL_ARRAY_BUFFER, 0/*offset*/, sizeof(vertexPositions), vertexPositions);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexPositions), sizeof(vertexColors), vertexColors);

	plane.cleanMemory();
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(vertexPositions));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void Application::update()
{
	camera = glm::lookAt(eye, target, glm::vec3(0.0f, 1.0, 0.0f));
	perspective = glm::perspective(45.0f, 1024.0f / 768.0f, 0.1f, 1000.0f);


	++time;
	//std::cout << time << std::endl;

	transformation = glm::rotate(glm::mat4(1.0f), glm::radians((float)time), glm::vec3(1.0f, 0.0f ,0.0f));
	transformation = perspective*camera*transformation;

	//pasamos valores de uniforms a los shaders
	glUniform1i(timeId, time);
	glUniformMatrix4fv(transformationId,1,GL_FALSE, glm::value_ptr(transformation));

}


void Application::display()
{
	//Borramos el buffer de color2qq1Q	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//Seleccionamos los shaders a usar
	//glUseProgram(triangle.shader);	
	glUseProgram(plane.shader);
	
	//activamos el vertex array object y dibujamos
	//glBindVertexArray(triangle.vao);
	glBindVertexArray(plane.vao);

	//glDrawArrays(GL_TRIANGLES, 0, triangle.numVertex);
	glDrawArrays(GL_TRIANGLES, 0, plane.numVertex);
}

void Application::reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}
