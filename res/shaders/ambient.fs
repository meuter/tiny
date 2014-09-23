#version 120

#include "lights.glsl"

uniform Material material;
uniform LightSource ambientLight;

varying vec2 fragTexcoord;

void main()
{
	gl_FragColor = computeAmbientLight(ambientLight, material, fragTexcoord);
}