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

struct LightSource
{
	vec3 color;
	vec3 direction;
	vec3 position;
	vec3 attenuation;

	float intensity;
	float cutoff, cutoffExponent;
};

vec4 computeMaterialColor(Material material, vec2 texcoord)
{
	return vec4(material.diffuse, 1) * texture2D(material.texture, texcoord);
}

vec4 computeBaseLight(LightSource light)
{
	return vec4(light.color,1) * light.intensity;
}

vec4 computeAmbientLight(LightSource light, Material material, vec2 texcoord)
{
	return computeBaseLight(light) * computeMaterialColor(material, texcoord);
}

vec4 computeDiffuseAndSpecularLight(LightSource light, Material material, vec3 lightDirection, Fragment fragment, vec3 eyePosition)
{
	lightDirection  = normalize(lightDirection);
	fragment.normal = normalize(fragment.normal);

	float diffuseFactor = max(0.0f, dot(fragment.normal, lightDirection));

	vec3 directionToEye   = normalize(eyePosition - fragment.position);
	vec3 reflectDirection = normalize(reflect(lightDirection, fragment.normal));	
	float specularFactor  = step(0, diffuseFactor) * max(0.0, pow(dot(directionToEye, reflectDirection), material.shininess));	
		
	vec4 totalLight = diffuseFactor + vec4(material.specular, 1.0f) * specularFactor;

	return totalLight * computeBaseLight(light) * computeMaterialColor(material, fragment.texcoord);
}

vec4 computeDirectionalLight(LightSource light, Material material, Fragment fragment, vec3 eyePosition)
{
	return computeDiffuseAndSpecularLight(light, material, -light.direction, fragment, eyePosition);
}

vec4 computePointLight(LightSource light, Material material, Fragment fragment, vec3 eyePosition)
{
	vec3  lightDirection  = fragment.position - light.position;
	float distanceToPoint = length(lightDirection);
	float attenuation = dot(vec3(pow(distanceToPoint, 2), distanceToPoint, 1), light.attenuation) + 0.00001f;

	return computeDiffuseAndSpecularLight(light, material, -lightDirection, fragment, eyePosition) / attenuation;
}

vec4 computeSpotLight(LightSource light, Material material, Fragment fragment, vec3 eyePos)
{
	vec3 lightDirection = normalize(fragment.position - light.position);
	float spotFactor = dot(lightDirection, light.direction); 
 	float fadeFactor = step(light.cutoff, spotFactor) * pow(max(0.0f, spotFactor), light.cutoffExponent);

	return fadeFactor * computePointLight(light, material, fragment, eyePos);
}



