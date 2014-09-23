#version 120

/*****************************************************************************/

struct AmbientLight
{
	vec3 color;
	float intensity;
};


vec4 calcAmbientLight(AmbientLight ambientLight)
{
	return vec4(ambientLight.color,1) * ambientLight.intensity;
}

/*****************************************************************************/

struct Material
{
	sampler2D texture;
	vec3 ambient;
	vec3 diffuse; 
	vec3 specular;
	float shininess;
};

vec4 calcMaterialColor(Material material, vec2 texcoord)
{
	return vec4(material.diffuse, 1) * texture2D(material.texture, texcoord);
}

/*****************************************************************************/

uniform Material material;
uniform AmbientLight ambientLight;

varying vec2 fragTexcoord;

void main()
{
	vec4 light = calcAmbientLight(ambientLight);
	vec4 color = calcMaterialColor(material, fragTexcoord);

	gl_FragColor = color * light;
}