#pragma once

#include <iostream>
#include "Object3D.h"

class Application {
public:
	Application();
	~Application();

	void setup();
	void update();
	void display();
	void reshape(int w, int h);

	//para tener el Id de la variable time de los shaders
	GLuint timeId;
	int time;

private:	
	Object3D triangle;
};

