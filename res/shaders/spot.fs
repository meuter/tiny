#version 120

#include "lights.glsl"

uniform SpotLight spotLight;
uniform Material material;
uniform vec3 eyePos;

varying vec2 fragTexcoord;
varying vec3 fragNormal;
varying vec3 fragPosition;

void main()
{
	Fragment fragment = Fragment(fragPosition, fragTexcoord, fragNormal);

	gl_FragColor = computeSpotLight(spotLight, material, fragment, eyePos);
}