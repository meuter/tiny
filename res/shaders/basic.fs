#version 120

uniform sampler2D texture;

varying vec2 texcoord0;

void main()
{
	gl_FragColor = texture2D(texture, texcoord0);
}