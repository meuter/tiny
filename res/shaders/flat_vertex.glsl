#version 120

attribute vec3 position;
attribute vec2 texcoord;

uniform mat4 transform;

varying vec2 texcoord0;
varying vec4 color0;

void main()
{
	texcoord0 = texcoord;
	color0    = vec4(clamp(position, 0.3f, 1.0f), 1.0f);
	gl_Position = transform * vec4(position, 1.0f);

}