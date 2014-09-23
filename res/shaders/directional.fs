#version 120

#include "lights.glsl"

uniform Material material;
uniform DirectionalLight directionalLight;
uniform vec3 eyePos;

varying vec3 fragPosition;
varying vec2 fragTexcoord;
varying vec3 fragNormal;

void main()
{
	Fragment fragment = Fragment(fragPosition, fragTexcoord, fragNormal);

	vec4 color = computeMaterialColor(material, fragTexcoord);
	vec4 light = computeDirectionalLight(directionalLight, material, fragment, eyePos);

	gl_FragColor = color * light;
}