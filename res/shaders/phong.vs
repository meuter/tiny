#version 120

attribute vec3 position;
attribute vec2 texcoord;
attribute vec3 normal;

uniform mat4 MVP;

varying vec2 texcoord0;

void main()
{
	texcoord0 = texcoord;
	gl_Position = MVP * vec4(position, 1.0f);
}