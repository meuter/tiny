#version 120

#include "light.glsl"

uniform Material material;
uniform LightSource ambientLight;
uniform LightSource lightSources[16];
uniform vec3 eyePosition;

varying vec3 fragPosition;
varying vec2 fragTexcoord;
varying vec3 fragNormal;

void main()
{
	Fragment fragment = Fragment(fragPosition, fragTexcoord, fragNormal);
	vec4 color = computeAmbientLight(ambientLight, material, fragment);

	for (int i = 0; i < 16; i++)
	{
		if (lightSources[i].intensity > 0)
			color += computeSpotLight(lightSources[i], material, fragment, eyePosition);
	}

	gl_FragColor = color;
}