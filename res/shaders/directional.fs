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

struct Material
{
	sampler2D texture;
	vec3 ambient;
	vec3 diffuse; 
	vec3 specular;
	float shininess;
};

uniform Material material;
uniform DirectionalLight directionalLight;
uniform vec3 eyePos;

varying vec2 fragTexcoord;
varying vec3 fragNormal;
varying vec3 fragWorldPosition;

vec4 calcLight(BaseLight base, vec3 direction, vec3 normal)
{
	direction = normalize(direction);
	normal    = normalize(normal);

	float diffuseFactor = clamp(dot(normal, direction), 0,1);

	vec4 diffuseLight  = vec4(0,0,0,0);
	vec4 specularLight = vec4(0,0,0,0);

	if (diffuseFactor > 0)
	{
		diffuseLight = vec4(base.color, 1.0f) * base.intensity * diffuseFactor;

		vec3 directionToEye = normalize(eyePos - fragWorldPosition);
		vec3 reflectDirection = normalize(reflect(direction, normal));	

		float specularFactor = pow(dot(directionToEye, reflectDirection), material.shininess);	

		if (specularFactor > 0)
			specularLight = vec4(base.color, 1.0f) * vec4(material.specular, 1.0f) * specularFactor;
	}

	return diffuseLight + specularLight;
}

vec4 calcDirectionalLight(DirectionalLight directionalLight, vec3 normal)
{
	return calcLight(directionalLight.base, -directionalLight.direction, normal);
}

void main()
{
	vec4 light = vec4(0,0,0,1);
	vec4 color = vec4(material.diffuse, 1);
	vec4 texel = texture2D(material.texture, fragTexcoord);

	light += calcDirectionalLight(directionalLight, fragNormal);

	gl_FragColor = color * texel * light;
}