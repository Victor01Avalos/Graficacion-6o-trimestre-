#version 430
layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vColor;

out vec4 color;
uniform int time;
uniform mat4 transformation;


void main()
{	
color = vColor;	

vec4 newPos=vPosition;
//newPos.x=newPos.x*cos(time*3.141592/180.0);

newPos= transformation *newPos;

gl_Position = newPos;
}
	