#pragma once

// Framework Header
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

// Project Specific Header
#include "../SelectableMovableObject.h"

// Visual model object which can be selected and moved
// Matthew Cormack

#define OBJECTTYPE_MODEL 2

class ObjectModelClass : public SelectableMovableObjectClass
{
	public:
		ObjectModelClass();
		~ObjectModelClass();

		virtual void Initialize( ofShader* shader, string modelpath );
		virtual void Initialize( ofShader* shader, ofxAssimpModelLoader* model );
		virtual void Update( float time );
		virtual void Draw( bool select = false );

		void SetRotation( ofVec3f rotation );

		inline ofxAssimpModelLoader* GetModel() { return Model; };

	protected:
		// Render model
		ofxAssimpModelLoader* Model;
};
