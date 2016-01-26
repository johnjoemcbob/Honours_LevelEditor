#version 120

uniform int name;

void main()
{
	gl_FragColor = vec4( name / 255.0, 0, 0, 1 );
}