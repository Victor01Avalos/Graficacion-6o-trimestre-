#pragma once

#include <iostream>
#include "Object3D.h"

#include "glm\mat4x4.hpp"
#include "glm\common.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm\vec3.hpp"

class Application {
public:
	Application();
	~Application();

	void setup();
	void update();
	void display();
	void reshape(int w, int h);

	//para tener el Id de la variable time de los shaders
	GLuint timeId,
		transformationId;

	int time;
	glm::mat4	transformation,
				camera,
				perspective;

	glm::vec3	eye,
				target;

private:	
	Object3D triangle;
};

