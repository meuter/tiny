#version 120

attribute vec3 position;
attribute vec2 texcoord;

uniform mat4 transform;

varying vec2 texcoord0;

void main()
{
	texcoord0 = texcoord;
	gl_Position = transform * vec4(position, 1.0f);
}