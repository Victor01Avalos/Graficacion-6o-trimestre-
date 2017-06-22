#version 430 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec2 texture;

uniform mat4 mTransform;
out vec2 textCoor;

void main()
{
	textCoor = texture;
	gl_Position = mTransform * vPosition;
}
