#version 120

#include "lights.glsl"

uniform Material material;

varying vec2 fragTexcoord;
varying vec4 fragLight;

void main()
{
	gl_FragColor = fragLight * computeMaterialColor(material, fragTexcoord);
}