#version 120

#include "lights.glsl"

uniform Material material;
uniform LightSource directionalLight;
uniform vec3 eyePos;

varying vec3 fragPosition;
varying vec2 fragTexcoord;
varying vec3 fragNormal;

void main()
{
	Fragment fragment = Fragment(fragPosition, fragTexcoord, fragNormal);
	
	gl_FragColor = computeDirectionalLight(directionalLight, material, fragment, eyePos);
}