#version 120

varying vec4 quadCoord;

void main()
{
	quadCoord = gl_Vertex;
	vec4 pos = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
    gl_Position = pos;
}
