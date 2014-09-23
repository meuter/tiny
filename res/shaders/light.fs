#version 120

#include "light.glsl"

uniform Material material;
uniform LightSource spotLight;
uniform vec3 eyePos;

varying vec3 fragPosition;
varying vec2 fragTexcoord;
varying vec3 fragNormal;

void main()
{
	Fragment fragment = Fragment(fragPosition, fragTexcoord, fragNormal);

	gl_FragColor = computeSpotLight(spotLight, material, fragment, eyePos);
}