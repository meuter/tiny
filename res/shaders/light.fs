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
varying mat3 fragTBN;

void main()
{
	Fragment fragment = Fragment(fragPosition, fragTexcoord, fragTBN);
	vec4 color = computeAmbientLight(ambientLight, material, fragment);

#if MAX_LIGHT_SOURCES > 0
	for (int i = 0; i < MAX_LIGHT_SOURCES; i++)
		color += computeLightSource(lightSources[i], material, fragment, eyePosition);
#endif


	vec3 bump = (255.0f/128.0f * texture2D(material.normalMap, fragTexcoord).xyz - 1.0f);
	vec3 normal = normalize(fragTBN * bump);


	gl_FragData[0] = color;
	gl_FragData[1] = vec4(fragTexcoord, 0, 0);
	gl_FragData[2] = vec4(normal, 0);
	gl_FragData[3] = vec4(fragPosition, 0);
}