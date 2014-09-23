#version 120

#include "lights.glsl"

uniform Material material;
uniform PointLight pointLight;
uniform vec3 eyePos;

varying vec2 fragTexcoord;
varying vec3 fragNormal;
varying vec3 fragPosition;

void main()
{
	Fragment fragment = Fragment(fragPosition, fragTexcoord, fragNormal);

	vec4 color = computeMaterialColor(material, fragTexcoord);
	vec4 light = computePointLight(pointLight, material, fragment, eyePos);

	gl_FragColor = color * light;
}