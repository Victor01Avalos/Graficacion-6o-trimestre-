
#include "Application.h"
#include <iostream>
#include <vector>
#include "GL\glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "GLFW/glfw3.h"
#include "ShaderFuncs.h"

GLfloat vertexPositions[] = {
	-1.0f, 1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 1.0f,//t1

	-1.0f, 1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,//t2

	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, -1.0f, 1.0f,//t3

	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, -1.0f, 1.0f,//t4

	1.0f, 1.0f, -1.0f, 1.0f,
	1.0f,-1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f, 1.0f,//t5

	1.0f, 1.0f, -1.0f, 1.0f,
	-1.0f,-1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f, 1.0f,//t6

	-1.0f, 1.0f, -1.0f, 1.0f,
	-1.0f,-1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f, 1.0f,//t7

	-1.0f, 1.0f, -1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, 1.0f,//t8

	-1.0f, 1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,//t9

	-1.0f, 1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f, 1.0f,//t10


	-1.0f, -1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, -1.0f, 1.0f,//t11

	-1.0f, -1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 1.0f,//t12

};


GLfloat vertexColors[] = {
	1.0, 0.0, 0.0, 1.0,
	1.0, 0.0, 0.0, 1.0,
	1.0, 0.0, 0.0, 1.0,//t1 ColorRojo

	1.0, 0.0, 0.0, 1.0,
	1.0, 0.0, 0.0, 1.0,
	1.0, 0.0, 0.0, 1.0,//t2 ColorRojo

	0.0, 1.0, 0.0, 1.0,
	0.0, 1.0, 0.0, 1.0,
	0.0, 1.0, 0.0, 1.0,//t3 ColorVerde

	0.0, 1.0, 0.0, 1.0,
	0.0, 1.0, 0.0, 1.0,
	0.0, 1.0, 0.0, 1.0,//t4 ColorVerde

	1.0, 0.5, 0.0, 1.0,
	1.0, 0.5, 0.0, 1.0,
	1.0, 0.5, 0.0, 1.0,//t5 ColorNaranja

	1.0, 0.5, 0.0, 1.0,
	1.0, 0.5, 0.0, 1.0,
	1.0, 0.5, 0.0, 1.0,//t6 ColorNaranja

	0.0, 0.0, 1.0, 1.0,
	0.0, 0.0, 1.0, 1.0,
	0.0, 0.0, 1.0, 1.0,//t7 ColorAzul

	0.0, 0.0, 1.0, 1.0,
	0.0, 0.0, 1.0, 1.0,
	0.0, 0.0, 1.0, 1.0,//t8 ColorAzul

	1.0, 1.0, 1.0, 1.0,
	1.0, 1.0, 1.0, 1.0,
	1.0, 1.0, 1.0, 1.0,//t9 ColorBlanco

	1.0, 1.0, 1.0, 1.0,
	1.0, 1.0, 1.0, 1.0,
	1.0, 1.0, 1.0, 1.0,//t10 ColorBlanco

	1.0, 1.0, 0.0, 1.0,
	1.0, 1.0, 0.0, 1.0,
	1.0, 1.0, 0.0, 1.0,//t11 ColorAmarillo

	1.0, 1.0, 0.0, 1.0,
	1.0, 1.0, 0.0, 1.0,
	1.0, 1.0, 0.0, 1.0,//t11 ColorAmarillo

};

Application::Application() {
	eye = glm::vec3(2.0f, 0.0f, 2.5f);
	Eye2 = eye;
	movmnt = glm::vec3(1.0f, 0.0f, 0.0f);
	target= movmnt + eye;
	Target2 = target;
	walkW = false;
	walkS = false;
	Left = false;
	Right = false;
	editor = true;

	up = glm::vec3(0.0f, 1.0f, 0.0f);
	mouseX = 0;
	mouseY = 0;
	for (int i = 0; i < MapaX; i++) {
		for (int j = 0; j < MapaY; j++) {
			mapa[i][j] = 0;
		}
	}
	if (MapaX >= MapaY) {
		float sizex = MapaX;
		float sizey = MapaY;
		float resScale = sizey / sizex;
		screen = glm::vec2(1024, 1024 * resScale);
	}
	else {
		int resScale = MapaX / MapaY;
		screen = glm::vec2(1024*resScale, 1024);
	}
}

Application::~Application() 
{}

void Application::update(){
	Eye2 = eye;
	Target2 = target;
	if (movePlayer) {
		if (walkW) {
			eye += glm::vec3(0.9f, 0.9f, 0.9f) * movmnt;
			target += glm::vec3(0.9f, 0.9f, 0.9f)* movmnt;
		}
		else if (walkS) {
			eye -= glm::vec3(0.9f, 0.9f, 0.9f)* movmnt;
			target -= glm::vec3(0.9f, 0.9f, 0.9f)* movmnt;
		}
	}
	if (Left) {
		rotatePlayer(-0.9);
	}
	else if (Right) {
		rotatePlayer(0.9);
	}
	collisionWall();
}

void Application::setup()
{

	//int MapaX = 20; int MapaY = 15;

	/*int mapa[20][15] = {
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,0,0,0,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,1,0,0,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,1,0,0,1,1,1,0,1,1,1,1,1,1,1 },
		{ 1,1,1,0,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,1,0,0,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,1,0,0,1,1,1,0,1,1,1,1,1,1,1 },
		{ 1,1,1,0,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		};*/

	std::string strVertexShader = loadTextFile("Shaders/passThru.vs");
	std::string strFragmentShader = loadTextFile("Shaders/passThru.fs");
	InitializeProgram(triangle.shader, strVertexShader, strFragmentShader);
	

	//zona de obtencion de Id's
	timeId = glGetUniformLocation(triangle.shader, "time");
	transformationId = glGetUniformLocation(triangle.shader, "transformation");
	
	triangle.numVertex = 36;
	glGenVertexArrays(1, &triangle.vao);
	glBindVertexArray(triangle.vao);
	glGenBuffers(1, &triangle.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, triangle.vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions) + sizeof(vertexColors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0/*offset*/, sizeof(vertexPositions), vertexPositions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexPositions), sizeof(vertexColors), vertexColors);


	glEnable(GL_DEPTH_TEST);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(vertexPositions));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

}

void Application::Display2() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Seleccionamos los shaders a usar
	glUseProgram(triangle.shader);

	//activamos el vertex array object y dibujamos
	glBindVertexArray(triangle.vao);

	ortho = glm::ortho(-1.0f, MapaX*2.0f - 1.0f, MapaY*2.0f - 1.0f, -1.0f);
	for (int i = 0; i < MapaX; i++) {
		for (int j = 0; j < MapaY; j++) {
			if (mapa[i][j] == 1) {
				newPos = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f*i, 2.0f*j, 0.0));
				transform = ortho*newPos;

				glUniformMatrix4fv(triangle.uniform, 1, false, glm::value_ptr(transform));
				glDrawArrays(GL_TRIANGLES, 0, triangle.numVertex);
			}
		}
	}
}

void Application::display(){
	//Borramos el buffer de color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Seleccionamos los shaders a usar
	glUseProgram(triangle.shader);	
	
	//activamos el vertex array object y dibujamos
	glBindVertexArray(triangle.vao);


	lookAt = glm::lookAt(eye, target, up);
	perspective = glm::perspective(50.0f / 180.0f, screen.x/screen.y, 0.1f, 500.0f);
	for (int i = 0; i < MapaX; i++) {
		for (int j = 0; j < MapaY; j++) {
			if (mapa[i][j] == 1) {
				newPos = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f*i, 0.0f, 2.0f*j));
				transform = perspective*lookAt*newPos;

				glUniformMatrix4fv(triangle.uniform, 1, false, glm::value_ptr(transform));
				glDrawArrays(GL_TRIANGLES, 0, triangle.numVertex);
			}
		}
	}
}

void Application::keyboard(int key, int scancode, int action, int mods){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		exit(0);
	}
	
	if (key == GLFW_KEY_W	&& action == GLFW_PRESS) {
		walkW = true;
		walkS = false;
	}
	else if (key == GLFW_KEY_W	&& action == GLFW_RELEASE) {
		walkW = false;
	}
	if (key == GLFW_KEY_S	&& action == GLFW_PRESS) {
		walkW = false;
		walkS = true;
	}
	else if (key == GLFW_KEY_S	&& action == GLFW_RELEASE) {
		walkS = false;
	}
	if (key == GLFW_KEY_A	&& action == GLFW_PRESS) {
		Left = true;
		Right = false;
	}
	else if (key == GLFW_KEY_A	&& action == GLFW_RELEASE) {
		Left = false;
	}
	if (key == GLFW_KEY_D	&& action == GLFW_PRESS) {
		Left = false;
		Right = true;
	}
	else if (key == GLFW_KEY_D	&& action == GLFW_RELEASE) {
		Right = false;
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		editor = !editor;
	}
	if (action == GLFW_RELEASE) {
		return;
	}
}

void Application::mousePos(int x, int y){
	mouseX = x;
	mouseY = y;
}


void Application::collisionWall(){
	glm::vec2 playerPos = glm::vec2(eye.x, eye.z);
	float playerHit = 0.5f;
	float wallHit = 0.9f;
	glm::vec2 wallCoor;
	for (int i = 0; i < MapaX; i++) {
		for (int j = 0; j < MapaY; j++) {
			if (mapa[i][j] == 1) {
				wallCoor = glm::vec2(2.0f*i, 2.0f*j);
				if (glm::distance(playerPos, wallCoor) <= (playerHit + wallHit)) {
					eye = Eye2;
					target = Target2;
					movePlayer = false;
					return;
				}
			}
		}
	}
	movePlayer = true;
}

void Application::mouseButtonX(int button, int action, int mods){
	if (editor) {
		int i = mouseX / (screen.x / MapaX);
		int j = mouseY / (screen.y / MapaY);
		if (button == GLFW_MOUSE_BUTTON_LEFT&&action == GLFW_PRESS) {
			if (mapa[i][j] == 1) {
				mapa[i][j] = 0;
			}
			else {
				mapa[i][j] = 1;
			}
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT&&action == GLFW_PRESS) {
			if (mapa[i][j] == 2) {
				mapa[i][j] = 0;
			}
			else {
				mapa[i][j] = 2;
			}
		}
		if (action == GLFW_RELEASE) {
			return;
		}
	}
}

void Application::rotatePlayer(float angle) {
	this->angle += angle;
	float x = 0.2*cos(this->angle/180);
	float z = 0.2*sin(this->angle/180);
	movmnt.x = x;
	movmnt.z = z;
	target = eye + movmnt;
}
