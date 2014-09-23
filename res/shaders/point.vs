#version 120

attribute vec3 position;
attribute vec2 texcoord;
attribute vec3 normal;

uniform mat4 M;
uniform mat4 MVP;

varying vec3 fragPosition;
varying vec2 fragTexcoord;
varying vec3 fragNormal;

void main()
{
	fragTexcoord = texcoord;
	fragNormal   = (M *   vec4(normal,   0.0f)).xyz;
	fragPosition = (M *   vec4(position, 1.0f)).xyz;
	gl_Position  = (MVP * vec4(position, 1.0f));
}