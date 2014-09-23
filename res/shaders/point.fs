#version 120

#include "lights.glsl"

uniform Material material;
uniform PointLight pointLight;
uniform vec3 eyePos;

varying vec2 fragTexcoord;
varying vec3 fragNormal;
varying vec3 fragPosition;

void main()
{
	Fragment fragment = Fragment(fragPosition, fragTexcoord, fragNormal);

	gl_FragColor = computePointLight(pointLight, material, fragment, eyePos);
}