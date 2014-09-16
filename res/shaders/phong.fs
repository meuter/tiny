#version 120

struct BaseLight
{
	vec3 color;
	float intensity;
};

struct DirectionalLight
{
	BaseLight base;
	vec3 direction;
};

uniform sampler2D texture;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform DirectionalLight directionalLight;
uniform float specularIntensity;
uniform float specularExponent;
uniform vec3 eyePos;

varying vec2 texcoord0;
varying vec3 normal0;
varying vec3 worldPosition0;

vec4 calcLight(BaseLight base, vec3 direction, vec3 normal)
{
	float diffuseFactor = dot(normal, -direction);

	vec4 diffuseLight  = vec4(0,0,0,0);
	vec4 specularLight = vec4(0,0,0,0);

	if (diffuseFactor > 0)
	{
		diffuseLight = vec4(base.color, 1.0f) * base.intensity * diffuseFactor;

		vec3 directionToEye = normalize(eyePos - worldPosition0);
		vec3 reflectDirection = normalize(reflect(direction, normal));

		float specularFactor = pow(dot(directionToEye, reflectDirection), specularExponent);

		if (specularFactor > 0)
			specularLight = vec4(base.color, 1.0f) * specularIntensity * specularFactor;
	}

	return diffuseLight + specularLight;
}

vec4 calcDirectionalLight(DirectionalLight directionalLight, vec3 normal)
{
	return calcLight(directionalLight.base, directionalLight.direction, normal);
}

void main()
{
	vec4 light = vec4(ambient, 1);
	vec4 color = vec4(diffuse, 1);
	vec4 texel = texture2D(texture, texcoord0);

	light += calcDirectionalLight(directionalLight, normal0);

	gl_FragColor = color * texel * light;
}