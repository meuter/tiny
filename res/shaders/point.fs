#version 120

struct PointLight
{
	vec3 color;
	float intensity;
	vec3 position;
	vec3 attenuation;
	float range;
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
uniform PointLight pointLight;
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

vec4 calcPointLight(PointLight pointLight, vec3 normal)
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


void main()
{
	vec4 color = vec4(material.diffuse, 1);
	vec4 light = calcPointLight(pointLight, fragNormal);
	vec4 texel = texture2D(material.texture, fragTexcoord);

	gl_FragColor = color * light;
}