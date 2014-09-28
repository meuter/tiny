#version 120

mat3 computeTBN(mat4 M, vec3 normal, vec3 tangent)
{
	vec3 t = normalize((M * vec4(tangent, 0.0f)).xyz);
	vec3 n = normalize((M * vec4(normal, 0.0f)).xyz);
	vec3 b = cross(t,n);
	
	t = normalize(t - dot(t,b) * n);

	return mat3(t,b,n);
}

attribute vec3 position;
attribute vec2 texcoord;
attribute vec3 normal;
attribute vec3 tangent;

uniform mat4 M;
uniform mat4 MVP;

varying vec3 fragPosition;
varying vec2 fragTexcoord;
varying mat3 fragTBN;

void main()
{
	fragPosition = (M * vec4(position, 1.0f)).xyz;
	fragTexcoord = texcoord;
	fragTBN      = computeTBN(M, normal, tangent);
	gl_Position  = (MVP * vec4(position, 1.0f));
}