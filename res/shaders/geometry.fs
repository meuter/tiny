#version 120

struct Material
{
	sampler2D texture;
	sampler2D normalMap;
	vec3 diffuse; 
	vec3 specular;
	float shininess;
};

uniform Material material;

varying vec3 fragPosition;
varying vec2 fragTexcoord;
varying mat3 fragTBN;

void main()
{
	vec3 texel  = material.diffuse * texture2D(material.texture, fragTexcoord).xyz;
	vec3 bump   = (255.0f/128.0f * texture2D(material.normalMap, fragTexcoord).xyz - 1.0f);
	vec3 normal = normalize(fragTBN * bump);

	gl_FragData[0] = vec4(texel, 0);
	gl_FragData[1] = vec4(fragTexcoord, 0, 0);
	gl_FragData[2] = vec4(normal, 0);
	gl_FragData[3] = vec4(fragPosition, 0);
}