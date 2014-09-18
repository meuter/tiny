#version 120

attribute vec3 position;
attribute vec2 texcoord;
attribute vec3 normal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

varying vec2 texcoord0;
varying vec3 normal0;
varying vec3 worldPosition0;

void main()
{
	mat4 MV  = V * M;
	mat4 MVP = P * MV;

	texcoord0      = texcoord;
	normal0        = (M *   vec4(normal,   0.0f)).xyz;
	worldPosition0 = (M *   vec4(position, 1.0f)).xyz;
	gl_Position    = (MVP * vec4(position, 1.0f));
}