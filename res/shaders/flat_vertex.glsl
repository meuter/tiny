#version 120

attribute vec3 position;
attribute vec2 textureCoord;

uniform mat4 transform;

varying vec2 textureCoord0;

void main()
{
	textureCoord0 = textureCoord;
	gl_Position = transform * vec4(position, 1.0f);

}