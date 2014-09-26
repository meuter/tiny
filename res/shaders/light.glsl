struct Material
{
	sampler2D texture;
	sampler2D normalMap;
	vec3 diffuse; 
	vec3 specular;
	float shininess;
};

struct Fragment 
{
	vec3 position;
	vec2 texcoord;
	mat3 TBN;
};

struct LightSource
{
	vec3 color;
	vec3 direction;
	vec3 position;
	vec3 attenuation;

	float cutoff, cutoffExponent;
};


vec4 computeAmbientLight(vec3 ambientLight, Material material, Fragment fragment)
{
	vec4 texel = texture2D(material.texture, fragment.texcoord);

	return vec4(material.diffuse * ambientLight, 1) * texel;
}

vec4 computeLightSource(LightSource light, Material material, Fragment fragment, vec3 eyePosition)
{
	vec3 normal = normalize(fragment.TBN * vec3(0,1,0));
	vec3 lightDirection  = normalize(light.position - fragment.position);
	vec3 directionToEye   = normalize(eyePosition - fragment.position);
	vec3 reflectDirection = reflect(lightDirection, normal);	

	float distanceToPoint = length(light.position - fragment.position);
	float attenuation = dot(vec3(pow(distanceToPoint, 2), distanceToPoint, 1), light.attenuation) + 0.00001f;
	float spotFactor = -dot(lightDirection, light.direction); 
 	float fadeFactor = step(light.cutoff, spotFactor) * pow(max(0.0f, spotFactor), light.cutoffExponent);
	float cosTheta = dot(normal, lightDirection);
	float diffuseFactor = max(0.0f, cosTheta);
	float specularFactor = step(0, cosTheta) * pow(dot(directionToEye, reflectDirection), material.shininess);	

	vec4 texel = texture2D(material.texture, fragment.texcoord);
	vec3 diffuse  = diffuseFactor * material.diffuse;
	vec3 specular = specularFactor * material.specular;	

	return (fadeFactor / attenuation) * vec4((diffuse + specular) * light.color, 1) * texel;
}