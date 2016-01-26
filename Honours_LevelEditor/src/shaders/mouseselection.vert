#version 120

uniform mat4 m_pvm;

void main()
{
	vec4 pos = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
    gl_Position = pos;
}
