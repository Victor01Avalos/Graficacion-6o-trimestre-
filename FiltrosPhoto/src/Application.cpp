
#include "Application.h"
#include <iostream>
#include <vector>
#include "GL\glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "ShaderFuncs.h"


Application::Application():oPlane(), nop(0){
	eye = glm::vec3(0.0f, 50.0f, 100.0f);
	target = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	time = 0;
}


Application::~Application() {}

void Application::update(){
	time += 0.000005f;
}

void Application::setup(){
	oPlane.createPlane(100);
	std::string strVertexShader = loadTextFile("Shaders/passThru.v");
	std::string strFragmentShader = loadTextFile("Shaders/passThru.f");


	//oPlane.texture_id[0] = initTexture("Lenna.png");
	oPlane.texture_id[0] = initTexture("FLECHA.png");


	InitializeProgram(oPlane.shader[0], strVertexShader, strFragmentShader);
	oPlane.uTexture[0] = glGetUniformLocation(oPlane.shader[0], "bTexture");
	oPlane.uTexture[1] = glGetUniformLocation(oPlane.shader[0], "cTexture");
	oPlane.uTransform[0] = glGetUniformLocation(oPlane.shader[0], "mTransform");
	oPlane.uTime[0] = glGetUniformLocation(oPlane.shader[0], "fTime");
	oPlane.uEye[0] = glGetUniformLocation(oPlane.shader[0], "vEye");

	glGenVertexArrays(1, &oPlane.vao);
	glBindVertexArray(oPlane.vao);
	glGenBuffers(1, &oPlane.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, oPlane.vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, oPlane.getVertexSizeInBytes() + oPlane.getTextureCoordsSizeInBytes(), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, oPlane.getVertexSizeInBytes(), oPlane.plane);
	glBufferSubData(GL_ARRAY_BUFFER, oPlane.getVertexSizeInBytes(), oPlane.getTextureCoordsSizeInBytes(), oPlane.textureCoords);
	oPlane.cleanMemory();

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)oPlane.getVertexSizeInBytes());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glEnable(GL_DEPTH_TEST);
}

void Application::display(){
	//Borramos el buffer de color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPolygonMode(GL_BACK, GL_LINE);
	//Seleccionamos los shaders a usar
	glUseProgram(oPlane.shader[0]);
	
	//activamos el vertex array object y dibujamos
	glBindVertexArray(oPlane.vao);
	lookAt = glm::lookAt(eye, target, up);
	perspective = glm::perspective(45.0f, 1920.0f / 1080.0f, 0.1f, 200.0f);
	transform = perspective*lookAt;
	glUniformMatrix4fv(oPlane.uTransform[0], 1, false, glm::value_ptr(transform));
	glUniform1f(oPlane.uTime[0], time);
	glUniform3fv(oPlane.uEye[0], 1, glm::value_ptr(eye));
	
	//texture0
	glBindTexture(GL_TEXTURE_2D, oPlane.texture_id[0]);
	glUniform1i(oPlane.uTexture[0], 0);
	glActiveTexture(GL_TEXTURE0);
	
	//texture1
	glBindTexture(GL_TEXTURE_2D, oPlane.texture_id[1]);
	glUniform1i(oPlane.uTexture[1], 1);
	glActiveTexture(GL_TEXTURE1);

	glDrawArrays(GL_TRIANGLES, 0, oPlane.getNumVertex());
}

void Application::reshape(int w, int h){
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void Application::keyboard(int key, int scancode, int action, int mods){
}

GLuint Application::initTexture(const std::string name){
	int width, height, channels;
	unsigned char* img = SOIL_load_image(name.c_str(), &width,&height,&channels,0);
	GLuint tex = 0;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	SOIL_free_image_data(img);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	
	glBindTexture(GL_TEXTURE_2D, 0);
	return tex;
}
