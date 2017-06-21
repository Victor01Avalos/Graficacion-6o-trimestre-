//

//  main.c

//  HelloTriangle

//

//  Created by Pedro  Cervantes Pintor on 19/Aug/12.

//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.

//


//Copyright (C) 2010-2012 by Jason L. McKesson

//This file is licensed under the MIT License.




#include <algorithm>
#include <Windows.h>
#include <string>
#include <vector>
#include <stdio.h>


#include "GL\glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL/freeglut.h"
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "SOIL.h"

GLuint theProgram;

const std::string strVertexShader(
	"#version 430 core\n"
	"layout (location = 0) in vec4 vPosition;"
	"layout (location = 1) in vec2 vTexCoord;"
	"out vec4 color;"
	"out vec2 texCoord;"
	"out float ftime;"

	"uniform float time;"
	"uniform mat4 modelViewMatrix, projectionMatrix;"
	"uniform sampler2D map;"
	"void main()"
	"{"
	"	vec4 newPos = vPosition;"

	"   vec3 colorDeMapa = texture(map, vTexCoord).rgb;"
	"	newPos.y = 100.0 * (0.2126*colorDeMapa.r) + (0.7152*colorDeMapa.g) + (0.0722*colorDeMapa.b);"
	//"   newPos.y = 10 * (colorDeMapa.r + colorDeMapa.g+colorDeMapa.b);"
	"	texCoord = vTexCoord;"
	"   gl_Position = projectionMatrix * modelViewMatrix * newPos;"
	
	"}"
	);


const std::string strFragmentShader(
	"#version 430 core\n"
	"out vec4 fColor;"
	"in vec4 color;"
	"in vec2 texCoord;"
	"in float ftime;"

	"uniform sampler2D tex;"

	"void main()"
	"{"
	"    fColor = texture(tex, texCoord);"
	"}"
	);

GLuint positionBufferObject;
GLuint vao;
float rotY = 0.0f, rotX = 0.0f, rotZ = 0.0f;

glm::mat4 modelViewMatrix;
GLint uModelViewMatrix;

glm::mat4 projectionMatrix;
GLint uProjectionMatrix;

GLint   uTime;
GLfloat fTime = 0.0f;

//para texturas
GLuint texture_id[2];
GLint uTexture, uHeightMap;

float cameraDistToTarget = 10.0f;
glm::vec3 cameraPosition; //esto se usa para los angulos de rotacion de la camara

GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile)
{
	GLuint shader = glCreateShader(eShaderType);
	const char *strFileData = strShaderFile.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch (eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
			//case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

	return shader;
}

GLuint CreateProgram(const std::vector<GLuint> &shaderList)
{
	GLuint program = glCreateProgram();

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glAttachShader(program, shaderList[iLoop]);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glDetachShader(program, shaderList[iLoop]);

	return program;
}


void InitializeProgram()
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, strVertexShader));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, strFragmentShader));

	theProgram = CreateProgram(shaderList);

	std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
	glUseProgram(theProgram);
	uModelViewMatrix = glGetUniformLocation(theProgram, "modelViewMatrix");
	uProjectionMatrix = glGetUniformLocation(theProgram, "projectionMatrix");
	uTime = glGetUniformLocation(theProgram, "time");
}



//GLfloat *plane; // 4 coordenadas homogeneas
GLfloat *normals; //
#define TS  2*3*2
GLfloat texCoords[100 * 100 * TS];
void initPlane()
{
#define PX  100
#define PZ  100
	//2 triangulos de 3 vertices con 4 coords 
#define VS  2*3*4
	//2 triangulos de 3 vertices con 2 coords 

	static const int px = PX, pz = PZ;
	static const int planeSize = px * VS * pz;
	static const int textureSize = px * TS * pz;
	GLfloat plane[planeSize]; // 4 coordenadas homogeneas

	//GLfloat normals[px * pz * 2 * 3 * 3]; //
	//plane = (GLfloat*) malloc (planeSize * sizeof(GLfloat));
	//normals = (GLfloat*)malloc(px*pz * 2 * 3 * 3 * sizeof(GLfloat));

	//calculo de coordenadas segun pantalla
	GLfloat dx = px / 2.0, dz = pz / 2.0, len = 1.0;

	int colNorm = 0;
	int col = 0;
	int texCol = 0;
	for (int j = 0; j < pz; ++j)
		for (int i = 0; i < px; ++i, col += 24, texCol += TS)
		{
			GLfloat x = i * len - dx,
				z = j * len - dz;

			//int index = px*VS * j + i;

			//triangulo 1 V1
			plane[col] = x;	//x
			plane[col + 1] = 0.0;				//y
			plane[col + 2] = z;	//z
			plane[col + 3] = 1.0;				//w
			texCoords[texCol] = (float)i / (float)PX;
			texCoords[texCol + 1] = (float)j / (float)PZ;

			//triangulo 1 V2
			plane[col + 4] = x;	//x
			plane[col + 5] = 0.0;				//y
			plane[col + 6] = z - len;	//z
			plane[col + 7] = 1.0;				//w
			texCoords[texCol + 2] = (float)i / (float)PX;
			texCoords[texCol + 3] = ((float)j - len) / (float)PZ;

			//triangulo 1 V3
			plane[col + 8] = x + len;	//x
			plane[col + 9] = 0.0;				//y
			plane[col + 10] = z - len;	//z
			plane[col + 11] = 1.0;				//w
			texCoords[texCol + 4] = ((float)i + len) / (float)PX;
			texCoords[texCol + 5] = ((float)j - len) / (float)PZ;

			//calculo de normales
			//normals[colNorm + 0] = normals[colNorm + 3] = normals[colNorm + 6] = 0.0;
			//normals[colNorm + 1] = normals[colNorm + 4] = normals[colNorm + 7] = 1.0;
			//normals[colNorm + 2] = normals[colNorm + 5] = normals[colNorm + 8] = 0.0;

			//triangulo 2 V1
			plane[col + 12] = x;
			plane[col + 13] = 0.0;			//y
			plane[col + 14] = z;
			plane[col + 15] = 1.0;			//w
			texCoords[texCol + 6] = (float)i / (float)PX;
			texCoords[texCol + 7] = (float)j / (float)PZ;

			//triangulo 2 V2
			plane[col + 16] = x + len;//x
			plane[col + 17] = 0.0;	//y
			plane[col + 18] = z - len;//z
			plane[col + 19] = 1.0; //w
			texCoords[texCol + 8] = ((float)i + len) / (float)PX;
			texCoords[texCol + 9] = ((float)j - len) / (float)PZ;

			//triangulo 2 V3
			plane[col + 20] = x + len;	//x
			plane[col + 21] = 0.0;				//y
			plane[col + 22] = z;	//z
			plane[col + 23] = 1.0;				//w
			texCoords[texCol + 10] = ((float)i + len) / (float)PX;
			texCoords[texCol + 11] = (float)j / (float)PZ;

			//calculo de normales
			//normals[colNorm + 9] = normals[colNorm + 12] = normals[colNorm + 15] = 0.0;
			//normals[colNorm + 10] = normals[colNorm + 13] = normals[colNorm + 16] = 1.0;
			//normals[colNorm + 11] = normals[colNorm + 14] = normals[colNorm + 17] = 0.0;
			//normals[col +15] = normals[col +19] = normals[col +23] = 1.0;

			colNorm += 18;
		}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &positionBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plane) + sizeof(texCoords), NULL, GL_STATIC_DRAW);
	
	//coordenadas de vertices
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(plane), &plane);
	//coordenadas de textura
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(plane), sizeof(texCoords), texCoords);
	//inicio de vertices
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	//inicio de texturas
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(plane));
	//atributo vertices
	glEnableVertexAttribArray(0);
	//atributo texturas
	glEnableVertexAttribArray(1);

}


void initPlane2()
{
	static const GLfloat quad_data_pos[] =
	{
		-1.0f, -1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 1.0f,
	};
	static const GLfloat quad_data_tex[] =
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &positionBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_data_pos)+sizeof(quad_data_tex), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quad_data_pos), &quad_data_pos);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(quad_data_pos), sizeof(quad_data_tex), &quad_data_tex);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	//inicio de texturas
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(quad_data_pos));
	//atributo vertices
	glEnableVertexAttribArray(0);
	//atributo texturas
	glEnableVertexAttribArray(1);

}

void InitTexture()
{
	int img_width, img_height;
	unsigned char* img = SOIL_load_image("texture.jpg", &img_width, &img_height, NULL, 0);
	

	glGenTextures(2, texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	// Mipmap generation is now included in OpenGL 3 and above
	glGenerateMipmap(GL_TEXTURE_2D);
	// Trilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	SOIL_free_image_data(img);

	//mapa de altura
	int channels = 1;  //forzamos carga en blanco y negro
	//img = SOIL_load_image("terrain.jpg", &img_width, &img_height, &channels, SOIL_LOAD_L);
	img = SOIL_load_image("heightmap.jpg", &img_width, &img_height, NULL, 0);

	glBindTexture(GL_TEXTURE_2D, texture_id[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	SOIL_free_image_data(img);
	glBindTexture(GL_TEXTURE_2D, 0);

	uTexture = glGetUniformLocation(theProgram, "tex");
	uHeightMap = glGetUniformLocation(theProgram, "map");
}

//Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void init()
{
	InitializeProgram();
	initPlane();
	InitTexture();
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	projectionMatrix = glm::perspective(45.0f, (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT), 0.1f, 10000.f);
	cameraPosition = glm::vec3(0.0f, 100.0f, 10.0f);
}

//Called to update the display.
//You should call glutSwapBuffers after all of your rendering to display what you rendered.
//If you need continuous updates of the screen, call glutPostRedisplay() at the end of the function
void displayPlane()
{

	glm::mat4 view = glm::lookAt(cameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 ViewRotateX = glm::rotate(glm::mat4(1.0f), rotX, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 ViewRotateY = glm::rotate(glm::mat4(1.0f), rotY, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 ViewRotateZ = glm::rotate(glm::mat4(1.0f), rotZ, glm::vec3(0.0f, 0.0f, 1.0f));

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
	modelViewMatrix = view * trans * ViewRotateX * ViewRotateY * ViewRotateZ;


	glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
	glUniformMatrix4fv(uProjectionMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	//glUniformMatrix4fv(uTime, 1, GL_FALSE, &fTime);
	glUniform1f(uTime, fTime);
	//activamos array y dibjamos
	glBindVertexArray(vao);

	//pasamos parametros de texturas
	glUniform1i(uTexture, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id[0]);

	//pasamos parametros de mapa de altura
	glUniform1i(uHeightMap, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_id[1]);

	glDrawArrays(GL_TRIANGLES, 0, PX * PZ * 2 * 3 /* 4*/);
	//glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	//glDisableVertexAttribArray(0);
	//glUseProgram(0);

	glutSwapBuffers();
}

//Called whenever the window is resized. The new window size is given, in pixels.
//This is an opportunity to call glViewport or glScissor to keep up with the change in size.
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	projectionMatrix = glm::perspective(45.0f, (float)w / (float)h, 0.1f, 1000.f);
}

void idle()
{
	fTime = fTime + 1.0;
	glutPostRedisplay();
}

//Called whenever a key on the keyboard was pressed.
//The key is given by the ''key'' parameter, which is in ASCII.
//It's often a good idea to have the escape key (ASCII value 27) call glutLeaveMainLoop() to 
//exit the program.
void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(EXIT_SUCCESS);
		//glutLeaveMainLoop();
		return;

	case 'X': ++rotX; break;
	case 'x': --rotX; break;
	case 'Y': ++rotY; break;
	case 'y': --rotY; break;
	case 'Z': ++rotZ; break;
	case 'z': --rotZ; break;

	case 'a': ++cameraPosition.x; break;
	case 'A': --cameraPosition.x; break;
	case 'b': ++cameraPosition.y; break;
	case 'B': --cameraPosition.y; break;
	case 'c': ++cameraPosition.z; break;
	case 'C': --cameraPosition.z; break;

	case 'r': rotX = rotY = rotZ = 0.0;
		cameraPosition.x = cameraPosition.y = cameraPosition.z = 0; break;

	}
	printf("%f, %f, %f\n", cameraPosition.x, cameraPosition.y, cameraPosition.z);



	glutPostRedisplay();
}

void passiveMotionFunc(int x, int y)
{
	static int lastX = 0, lastY = 0;

	y += glutGet(GLUT_WINDOW_WIDTH) / 2.0; //Esto se mapea en X
	x += glutGet(GLUT_WINDOW_HEIGHT) / 2.0; //Esto en realidad se mapea en Z

	rotX = y;
	rotZ = x;

	glutPostRedisplay();


}

unsigned int defaults(unsigned int displayMode, int &width, int &height)
{
	return displayMode;
}

int main(int argc, char *argv[])
{


	glutInit(&argc, argv);
	//argc is the number of variables passed to our program, argv is a pointer to those arguments,
	//they are passed to glut here. There's no need to know more about this right now.

	//glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);


	/*As the name says, this line initializes the display mode.
	-GLUT_RGBA states we want color and alpha buffers for our window
	-GLUT_DOUBLE means we want a Double Buffer, this means that images are completely drawn before they are sent
	to the screen. This will prevent flickering images.
	-GLUT_DEPTH means that we want a Depth Buffer. This will allow for the images to be drawn correctly with respect
	to their distance: i.e. objects closeby will overlap objects further away.*/


	glutInitWindowSize(1024, 768);
	//Define the size we would like the window to be

	glutCreateWindow("Height Map");
	//This command creates the window with the specifications we've set before and gives it a name. 

	glewInit();
	init();


	glutDisplayFunc(displayPlane);
	//this function is called whenever GLUT decides it's time to redraw the contents of the current window, it's also
	//the function where we'll do our drawing.
	glutReshapeFunc(reshape);

	//this function is called whenever the user changes the size of the frame

	//this function is called whenever glut is not doing anything
	glutIdleFunc(idle);

	glutPassiveMotionFunc(passiveMotionFunc);

	glutKeyboardFunc(Keyboard);

	//this function is called when a key is pressed
	glutMainLoop();
	//glutMainLoop() goes on forever and ever until you quit the program.
}
