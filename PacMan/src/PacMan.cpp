
#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>

#include "GL/glew.h"
#include "Application.h"

Application app;

//Se tiene que llamar despues de inicializar la ventana y el OpenGL, pero antes del main loop
void setup(){
	app.setup();
}

//Se ejecuta siempre que se necesite actualizar la imagen
void display(){
	if (!app.editor) {
		app.display();
	}
	else {
		app.Display2();
	}
}

void update() {
	if (!app.editor) {
		app.update();
	}
}

//void reshape(int w, int h){
//	app.reshape(w, h);
//}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	app.keyboard(key, scancode, action, mods);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	app.mouseButtonX(button, action, mods);
}

void mouse_pos(GLFWwindow* window, double xpos, double ypos) {
	app.mousePos(xpos, ypos);
}

int main(int argc, char *argv[]){

	GLFWwindow* window;

	if (!glfwInit())
		return -1;
	
	window = glfwCreateWindow(app.screen.x, app.screen.y, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE; 
	glewInit();
	app.setup();
	
	// get version info 
	const GLubyte* renderer = glGetString (GL_RENDERER); 
	const GLubyte* version = glGetString (GL_VERSION); 
	printf (" Renderer: %s\ n", renderer); 
	printf (" OpenGL version supported %s\ n", version); 
	
	

	glViewport(0, 0, (GLsizei)app.screen.x, (GLsizei)app.screen.y);
	glEnable(GL_DEPTH_TEST); //Z-buffer

	while (!glfwWindowShouldClose(window))
	{
		
		glfwPollEvents();
		update();
		
		display(); 
		glfwSwapBuffers(window);
		glfwSetCursorPosCallback(window, mouse_pos);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetKeyCallback(window, key_callback);
	}
	
	
	glfwTerminate();
	return 0;

}
