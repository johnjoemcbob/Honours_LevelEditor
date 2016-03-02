#version 120

uniform float falloff;
uniform vec3 position;
uniform vec4 colour;

varying vec4 quadCoord;

void main()
{
	float a = smoothstep( 1, 0, distance( quadCoord.xyz, position ) / falloff );
	gl_FragColor = vec4( colour.r, colour.g, colour.b, a );
}