#pragma once

// Framework Header
#include "ofMain.h"

// Project Specific Header
#include "../SelectableMovableObject.h"

// Route Node level object which can be selected and moved
// Matthew Cormack

#define OBJECTTYPE_NODE 1

class ObjectRouteNodeClass : public SelectableMovableObjectClass
{
	public:
		ObjectRouteNodeClass();
		~ObjectRouteNodeClass();

		virtual void Initialize( ofShader* shader );
		virtual void Update( float time );
		virtual void Draw( bool select = false );

	protected:
		// Render model
		ofSpherePrimitive Sphere;
};
