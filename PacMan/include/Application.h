#pragma once

#include <iostream>
#include "Object3D.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "GLFW\glfw3.h"
#define MapaX 20
#define MapaY 15

class Application {
public:
	Application();
	~Application();
	void update();
	void setup();
	void Display2();
	
	void display();
	
	void collisionWall();
	
	void keyboard(int key, int scancode, int action, int mods);
	void rotatePlayer(float angle);
	void mouseButtonX(int button, int action, int mods);
	void mousePos(int x, int y);
	bool editor;
	glm::vec2 screen;
private:	
	Object3D triangle;
	
	float angle;
	bool walkW, walkS, Left, Right, movePlayer;
	int mouseX, mouseY;
	

	//para tener el Id de la variable time de los shaders
	GLuint timeId,
		transformationId;


	glm::mat4 rotationX;
	glm::mat4 rotationY;
	glm::mat4 rotationZ;
	glm::mat4 transform;
	glm::vec3 eye, Eye2;
	glm::vec3 target, Target2;
	glm::vec3 up;
	glm::vec3 movmnt;
	glm::mat4 lookAt;
	glm::mat4 perspective;
	glm::mat4 ortho;
	glm::mat4 newPos;


	int mapa[MapaX][MapaY] = {
		

	};
};