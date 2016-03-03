#pragma once

// Framework Header
#include "ofMain.h"
#include "ofxDatGui.h"

// Base class for any object in the scene which can be selected
// Matthew Cormack

#define OBJECTTYPE_DEFAULT 0

class SelectableObjectClass : public ofNode
{
	public:
		SelectableObjectClass();
		~SelectableObjectClass();

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

		inline void SetSelectName( unsigned int name ) { SelectName = name; };
		inline unsigned int GetSelectName() { return SelectName; };

		void SetSelected( bool select );
		inline bool GetSelected() { return Selected; };

		inline void SetType( unsigned int type ) { Type = type; };
		inline unsigned int GetType() { return Type; };

		inline void SetShader( ofShader* shader ) { Shader_Select = shader; };
		inline ofShader* GetShader() { return Shader_Select; };

		static unsigned int CheckSelected( float mousex, float mousey );

		// References to App specific variables
		ofCamera* Camera;
		bool** KeyPressed;
		int* mouseX;
		int* mouseY;
		int* AxisSelected;

	protected:
		// Enable/Disable the selection render shader
		// Optional functionality to render with a different name
		void ShaderSelect( bool select, unsigned int name = 0 );

		// Create the selected gui
		virtual bool CreateGUI();

		// Reference to the selection render shader
		ofShader* Shader_Select;
		bool ShaderEnabled;

		// The unique id for this selectable, initializes colours in selection render shader
		// NOTE: 0,1,2 are reserved for axes
		unsigned int SelectName;

		// Whether or not this object is selected
		bool Selected;

		// The type of object this is
		unsigned int Type;

		// This object's selected gui popup
		ofxDatGui* GUI;
};
