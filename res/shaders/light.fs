#version 120

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


vec4 computeAmbientLight(LightSource light, Material material, Fragment fragment)
{
	vec4 texel = texture2D(material.texture, fragment.texcoord);
	vec3 diffuse  = material.diffuse;

	return light.intensity * vec4(diffuse * light.color, 1) * texel;
}

vec4 computeSpotLight(LightSource light, Material material, Fragment fragment, vec3 eyePosition)
{
	fragment.normal = normalize(fragment.normal);

	vec3 lightDirection  = normalize(light.position - fragment.position);
	vec3 directionToEye   = normalize(eyePosition - fragment.position);
	vec3 reflectDirection = reflect(lightDirection, fragment.normal);	

	float distanceToPoint = length(light.position - fragment.position);
	float attenuation = dot(vec3(pow(distanceToPoint, 2), distanceToPoint, 1), light.attenuation) + 0.00001f;
	float spotFactor = -dot(lightDirection, light.direction); 
 	float fadeFactor = step(light.cutoff, spotFactor) * pow(max(0.0f, spotFactor), light.cutoffExponent);
	float cosTheta = dot(fragment.normal, lightDirection);
	float diffuseFactor = max(0.0f, cosTheta);
	float specularFactor = step(0, cosTheta) * pow(dot(directionToEye, reflectDirection), material.shininess);	

	vec4 texel = texture2D(material.texture, fragment.texcoord);
	vec3 diffuse  = diffuseFactor * material.diffuse;
	vec3 specular = specularFactor * material.specular;

	return light.intensity * fadeFactor / attenuation * vec4((diffuse + specular) * light.color, 1) * texel;
}


uniform Material material;
uniform LightSource ambientLight;
uniform LightSource lightSources[10];
uniform vec3 eyePosition;

varying vec3 fragPosition;
varying vec2 fragTexcoord;
varying vec3 fragNormal;

void main()
{
	Fragment fragment = Fragment(fragPosition, fragTexcoord, fragNormal);
	vec4 color = computeAmbientLight(ambientLight, material, fragment);

	for (int i = 0; i < 10; i++)
	{
		if (lightSources[i].intensity > 0)
			color += computeSpotLight(lightSources[i], material, fragment, eyePosition);
	}

	gl_FragColor = color;
}