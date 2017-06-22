#version 430 core

uniform sampler2D bTexture;
uniform sampler2D cTexture;
out vec4 fColor;
in vec2 textCoor;

// Desaturation

/*

vec4 Desaturate(vec3 color, float Desaturation)
{
	vec3 grayXfer = vec3(0.3, 0.59, 0.11);
	vec3 gray = vec3(dot(grayXfer, grayXfer);  // color));
	return vec4(mix(color, gray, Desaturation), 1.0);
}


*/
// Negativo

vec4 Negativo(vec3 color, float Negativo)
{
vec3 negativo = vec3(0.3, 0.2, 0.5);
vec3 gray = vec3(dot(negativo, negativo));
return vec4(mix(color, gray, Negativo), 1.0);
}

void main()
{
	vec4 texel = texture(bTexture,textCoor);
	vec4 texel2= texture(cTexture,textCoor);
    fColor = mix(texel,texel2,1.00f);

	//fColor=Desaturate(fColor.rgb, 2.0f);

	fColor=Negativo(fColor.rgb, 2.0f);
}


