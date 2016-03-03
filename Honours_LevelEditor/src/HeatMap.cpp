// Assocciated Header
#include "HeatMap.h"

HeatMapClass::HeatMapClass()
{

}

HeatMapClass::~HeatMapClass()
{

}

void HeatMapClass::Initialize()
{
	// Init plane plane
	Plane.set( 10000, 10000 );
	Plane.setMode( ofPrimitiveMode::OF_PRIMITIVE_POINTS );
	Plane.setResolution( 100, 100 );
	Plane.tilt( 90 );

	Strength = 1;

	Shader.load( "shaders/heatmap.vert", "shaders/heatmap.frag" );
}

void HeatMapClass::Update( float time )
{
	Time = time;
}

void HeatMapClass::Draw()
{
	Shader.begin();
	{
		Shader.setUniform1f( "time", Time );
		Shader.setUniform1f( "points", DataCount );
		Shader.setUniform1f( "strength", Strength );

		GLuint program = Shader.getProgram();
		GLint loc = glGetUniformLocation( program, "heatposition" );
		glUseProgram( program );
		glUniform4fv( loc, DataCount, (float*) &Data[0] );

		Plane.draw();
	}
	Shader.end();
}

void HeatMapClass::SetData( float* data, float count )
{
	Data = data;
	DataCount = count;
}