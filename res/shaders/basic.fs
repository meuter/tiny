#version 120

uniform sampler2D texture;
uniform vec3      diffuse;

varying vec2 texcoord0;

void main()
{
	vec4 color = vec4(diffuse, 1.0f);
	vec4 texel = texture2D(texture, texcoord0);

	gl_FragColor = texel * color; 
}