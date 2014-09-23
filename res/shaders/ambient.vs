#version 120

#include "lights.glsl"

attribute vec3 position;
attribute vec2 texcoord;

uniform mat4 MVP;
uniform BaseLight ambientLight;

varying vec2 fragTexcoord;
varying vec4 fragLight;

void main()
{
	fragTexcoord      = texcoord;
	fragLight         = computeAmbientLight(ambientLight);
	gl_Position       = (MVP * vec4(position, 1.0f));
}