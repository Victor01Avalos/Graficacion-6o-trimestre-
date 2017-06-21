#version 430 core

out vec4 fColor;
in vec4 color;
uniform  sampler2D texture0;
uniform  sampler2D texture1;

in vec2 textureCoords;

void main()
{
    fColor = mix (texture(texture0, textureCoords),texture(texture1, textureCoords),0.6);
}
