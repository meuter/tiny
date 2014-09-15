#version 120

struct LightSource
{
	vec3 color;
	float intensity;
};

struct DirectionalLight
{
	LightSource source;
	vec3 direction;
};

uniform sampler2D texture;
uniform vec3 ambient;
uniform DirectionalLight directionalLight;

varying vec2 texcoord0;
varying vec3 normal0;

vec4 calcLight(LightSource source, vec3 direction, vec3 normal)
{
	float diffuseFactor = dot(normal, -direction);

	vec4 diffuseColor = vec4(0,0,0,0);

	if (diffuseFactor > 0)
		diffuseColor = vec4(source.color, 1.0f) * source.intensity * diffuseFactor;

	return diffuseColor;
}

vec4 calcDirectionalLight(DirectionalLight directionalLight, vec3 normal)
{
	return calcLight(directionalLight.source, directionalLight.direction, normal);
}

void main()
{
	vec4 totalLight   = vec4(ambient, 1);
	vec4 color        = vec4(1,1,1,1);
	vec4 textureColor = texture2D(texture, texcoord0);

	if (textureColor != vec4(0,0,0,0))
		color *= textureColor;

	totalLight += calcDirectionalLight(directionalLight, normal0);

	gl_FragColor = color * totalLight;
}