#version 120

attribute vec3 position;

uniform mat4 transform;

varying vec4 position0;

void main()
{
	position0 = vec4(clamp(position,0.0f,1.0f), 1.0f);
	gl_Position = transform * vec4(position, 1.0f);

}