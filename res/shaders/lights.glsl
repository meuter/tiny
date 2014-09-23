struct Material
{
	sampler2D texture;
	vec3 diffuse; 
	vec3 specular;
	float shininess;
};

struct Fragment 
{
	vec3 position;
	vec2 texcoord;
	vec3 normal;
};

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

struct PointLight
{
	BaseLight base;
	vec3 position;
	vec3 attenuation;
	float range;
};

struct SpotLight
{
	PointLight point;
	vec3 direction;
	float cutoff;
};

vec4 computeMaterialColor(Material material, vec2 texcoord)
{
	return vec4(material.diffuse, 1) * texture2D(material.texture, texcoord);
}

vec4 computeBaseLight(BaseLight light)
{
	return vec4(light.color,1) * light.intensity;
}

vec4 computeAmbientLight(BaseLight light, Material material, vec2 texcoord)
{
	return computeBaseLight(light) * computeMaterialColor(material, texcoord);
}

vec4 computeDiffuseAndSpecularLight(BaseLight light, Material material, vec3 lightDirection, Fragment fragment, vec3 eyePosition)
{
	lightDirection  = normalize(lightDirection);
	fragment.normal = normalize(fragment.normal);

	float diffuseFactor = dot(fragment.normal, lightDirection);

	vec4 totalLight  = vec4(0,0,0,0);

	if (diffuseFactor > 0)
	{		
		totalLight += computeBaseLight(light) * diffuseFactor;

		vec3 directionToEye = normalize(eyePosition - fragment.position);
		vec3 reflectDirection = normalize(reflect(lightDirection, fragment.normal));	

		float specularFactor = pow(dot(directionToEye, reflectDirection), material.shininess);	

		if (specularFactor > 0)
			totalLight += vec4(light.color, 1.0f) * vec4(material.specular, 1.0f) * specularFactor;
	}

	return totalLight * computeMaterialColor(material, fragment.texcoord);
}

vec4 computeDirectionalLight(DirectionalLight light, Material material, Fragment fragment, vec3 eyePosition)
{
	return computeDiffuseAndSpecularLight(light.base, material, -light.direction, fragment, eyePosition);
}

vec4 computePointLight(PointLight light, Material material, Fragment fragment, vec3 eyePosition)
{
	vec3  lightDirection  = fragment.position - light.position;
	float distanceToPoint = length(lightDirection);
	vec4  totalLight = vec4(0,0,0,0);

	if (distanceToPoint < light.range)
	{
		float attenuation = dot(vec3(pow(distanceToPoint, 2), distanceToPoint, 1), light.attenuation) + 0.00001f;

		totalLight = computeDiffuseAndSpecularLight(light.base, material, -lightDirection, fragment, eyePosition) / attenuation;
	}

	return totalLight;
}


vec4 computeSpotLight(SpotLight spotLight, Material material, Fragment fragment, vec3 eyePos)
{
	vec3 lightDirection = normalize(fragment.position - spotLight.point.position);
	float spotFactor = dot(lightDirection, spotLight.direction);

	vec4 light = vec4(0,0,0,0);

	if (spotFactor > spotLight.cutoff)
	{
		float fadeFactor = 1.0f - ((1.0f - spotFactor) / (1.0f - spotLight.cutoff));
		light += fadeFactor * computePointLight(spotLight.point, material, fragment, eyePos);
	}

	return light;
}
