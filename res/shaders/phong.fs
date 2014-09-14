#version 120

uniform sampler2D texture;
uniform vec3 ambient;

varying vec2 texcoord0;

void main()
{
	gl_FragColor = vec4(ambient, 1) * texture2D(texture, texcoord0);
}