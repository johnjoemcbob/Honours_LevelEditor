#pragma once

// Framework Header
#include "ofMain.h"

// Project Specific Header
#include "SelectableObject.h"

// Base class for any object in the scene which can be selected and moved
// Matthew Cormack

#define GRID_SNAP_FORCE 1
#define GRID_SNAP_DISTANCE 100

#define AXISLIMIT_X 5000
#define AXISLIMIT_Y 2000
#define AXISLIMIT_Z 5000

class SelectableMovableObjectClass : public SelectableObjectClass
{
	public:
		SelectableMovableObjectClass();
		~SelectableMovableObjectClass();

		virtual void Initialize( ofShader* shader );
		virtual void Update( float time );
		virtual void Draw( bool select = false );
		virtual void DrawGUI( bool select = false );

		virtual void OnPressed( int x, int y, int button );
		virtual void OnDragged( int x, int y, int button );
		virtual void OnReleased( int x, int y, int button );
		virtual void OnHover( float x, float y );
		virtual void OnUnHover( float x, float y );
		virtual void OnSelect();
		virtual void OnUnSelect();

	protected:
		// Create the selected gui
		virtual bool CreateGUI();

		ofVec2f LastMouse;

		// GUI Variables
		float SliderX, SliderY, SliderZ;
};
