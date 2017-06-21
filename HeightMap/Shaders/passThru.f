#version 430 core

out vec4 fColor;
in vec4 color;

//color
uniform  sampler2D texture0;

in vec2 textureCoords;

void main()
{
    fColor = texture(texture0, textureCoords);
}
