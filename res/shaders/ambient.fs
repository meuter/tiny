#version 120

struct Material
{
	sampler2D texture;
	vec3 ambient;
	vec3 diffuse; 
	vec3 specular;
	float shininess;
};

uniform Material material;

varying vec2 fragTexcoord;

void main()
{
	vec4 light = vec4(material.ambient, 1);
	vec4 color = vec4(material.diffuse, 1);
	vec4 texel = texture2D(material.texture, fragTexcoord);

	gl_FragColor = color * texel * light;
}