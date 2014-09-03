#version 120

uniform sampler2D texture;

varying vec2 textureCoord0;

void main()
{

	gl_FragColor = texture2D(texture, textureCoord0);
}