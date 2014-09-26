#version 120

#include "light.glsl"

uniform Material material;
uniform vec3 ambientLight;

#if MAX_LIGHT_SOURCES > 0
	uniform LightSource lightSources[MAX_LIGHT_SOURCES];
	uniform vec3 eyePosition;
#endif

varying vec3 fragPosition;
varying vec2 fragTexcoord;
varying vec3 fragNormal;
varying mat3 fragTBN;

void main()
{
	Fragment fragment = Fragment(fragPosition, fragTexcoord, fragNormal, fragTBN);
	vec4 color = computeAmbientLight(ambientLight, material, fragment);

#if MAX_LIGHT_SOURCES > 0
	for (int i = 0; i < MAX_LIGHT_SOURCES; i++)
		color += computeLightSource(lightSources[i], material, fragment, eyePosition);
#endif

	gl_FragColor = color;
}