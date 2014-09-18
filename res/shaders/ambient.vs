#version 120

attribute vec3 position;
attribute vec2 texcoord;

uniform mat4 MVP;

varying vec2 fragTexcoord;

void main()
{
	fragTexcoord      = texcoord;
	gl_Position       = (MVP * vec4(position, 1.0f));
}