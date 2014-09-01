#version 120

attribute vec3 position;

uniform float scale;

void main()
{
	gl_Position = vec4(scale * position, 1.0f);
}