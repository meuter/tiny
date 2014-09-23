#version 120

#include "lights.glsl"

struct SpotLight
{
	vec3 color;
	vec3 position;
	vec3 attenuation;
	vec3 direction;

	float intensity;
	float range;
	float cutoff;
};

uniform Material material;
uniform SpotLight spotLight;
uniform vec3 eyePos;

varying vec2 fragTexcoord;
varying vec3 fragNormal;
varying vec3 fragWorldPosition;

vec4 calcLight(vec3 color, vec3 direction, float intensity, vec3 normal)
{
	direction = normalize(direction);
	normal    = normalize(normal);

	float diffuseFactor = dot(normal, direction);

	vec4 diffuseLight  = vec4(0,0,0,0);
	vec4 specularLight = vec4(0,0,0,0);

	if (diffuseFactor > 0)
	{
		diffuseLight = vec4(color, 1.0f) * intensity * diffuseFactor;

		vec3 directionToEye = normalize(eyePos - fragWorldPosition);
		vec3 reflectDirection = normalize(reflect(direction, normal));	

		float specularFactor = pow(dot(directionToEye, reflectDirection), material.shininess);	

		if (specularFactor > 0)
			specularLight = vec4(color, 1.0f) * vec4(material.specular, 1.0f) * specularFactor;
	}

	return diffuseLight + specularLight;
}

vec4 calcPointLight(SpotLight pointLight, vec3 normal)
{
	vec3  lightDirection  = fragWorldPosition - pointLight.position;
	float distanceToPoint = length(lightDirection);
	vec4  light = vec4(0,0,0,0);

	if (distanceToPoint < pointLight.range)
	{
		float attenuation = dot(vec3(pow(distanceToPoint, 2), distanceToPoint, 1), pointLight.attenuation) + 0.00001f;
		light += calcLight(pointLight.color, -lightDirection, pointLight.intensity, normal) / attenuation;
	}

	return light;
}

vec4 calcSpotLight(SpotLight spotLight, vec3 normal)
{
	vec3 lightDirection = normalize(fragWorldPosition - spotLight.position);
	float spotFactor = dot(lightDirection, spotLight.direction);

	vec4 light = vec4(0,0,0,0);

	if (spotFactor > spotLight.cutoff)
	{
		float fadeFactor = 1.0f - ((1.0f - spotFactor) / (1.0f - spotLight.cutoff));
		light += fadeFactor * calcPointLight(spotLight, normal);
	}


	return light;
}


void main()
{
	vec4 color = computeMaterialColor(material, fragTexcoord);
	vec4 light = calcSpotLight(spotLight, fragNormal);

	gl_FragColor = color * light;
}