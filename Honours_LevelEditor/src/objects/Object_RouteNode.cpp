// Assocciated Header
#include "Object_RouteNode.h"

ObjectRouteNodeClass::ObjectRouteNodeClass()
{
	SelectableMovableObjectClass::SelectableMovableObjectClass();

	Type = OBJECTTYPE_NODE;
}

ObjectRouteNodeClass::~ObjectRouteNodeClass()
{
	SelectableMovableObjectClass::~SelectableMovableObjectClass();
}

void ObjectRouteNodeClass::Initialize( ofShader* shader )
{
	SelectableMovableObjectClass::Initialize( shader );

	Sphere.set( 50, 4 );
}

void ObjectRouteNodeClass::Update( float time )
{
	SelectableMovableObjectClass::Update( time );


}

void ObjectRouteNodeClass::Draw( bool select )
{
	SelectableMovableObjectClass::Draw( select );

	ShaderSelect( select, SelectName );
	{
		Sphere.setTransformMatrix( getGlobalTransformMatrix() );
		Sphere.draw();
	}
	ShaderSelect( select );
}