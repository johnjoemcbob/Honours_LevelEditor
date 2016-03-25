#pragma once

// Framework Header
#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxAssimpModelLoader.h"

// C++ Header
#include <vector>
#include <algorithm>

// Project Specific Header
#include "HeatMap.h"
#include "SelectableObject.h"
#include "SelectableMovableObject.h"
#include "objects/Object_RouteNode.h"
#include "objects/Object_Model.h"

#define BUF_SIZE 512

#define INPUT_SELECT 0
#define INPUT_CAMERA_ROTATE 2

#define SCALEFACTOR_UNITY_TO_EDITOR 128
#define SCALEFACTOR_EDITOR_TO_UNITY 1 / SCALEFACTOR_UNITY_TO_EDITOR

#define CAMERA_MAX_Y 178

#define SELECT_OFFSET 4

#define POSSIBLEMODELS 7

typedef struct
{
	string Key;
	string Value;
	float Timestamp;
} AnalyticDataStruct;

class ofApp : public ofBaseApp
{
	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed( int key );
		void keyReleased( int key );
		void mouseMoved( int x, int y );
		void mouseDragged( int x, int y, int button );
		void mousePressed( int x, int y, int button );
		void mouseReleased( int x, int y, int button );
		void mouseEntered( int x, int y );
		void mouseExited( int x, int y );
		void windowResized( int w, int h );
		void dragEvent( ofDragInfo dragInfo );
		void gotMessage( ofMessage msg );

	private:
		void UpdateCamera( float distx, float disty );
		void DrawFrame( bool select = false );
		void DrawFrame_SelectOnly_Shader_Begin( bool select = false, int name = 0 );
		void DrawFrame_SelectOnly_Shader_End( bool select = false );
		int GetLowerKeyCode( int key );
		void AddRouteNode( ofVec3f pos );
		void SaveLevel();
		bool LoadAnalytics( string dir = "" );
		void ParseAnalytics( ofXml xml_analyticinput );
		bool LoadAverageAnalytics( string dir = "" );
		void ParseAverageAnalytics( ofXml xml_analyticinput );
		void AddAnalyticGraphs();
		void LoadLevel();
		void ParseLevel( ofXml xml_levelinput );
		bool CanLoad( string model );
		ofxAssimpModelLoader* LoadFirstModel( int id, string modelpath );
		void LoadHeatmapData( string dataname );

		void Event_OnButton( ofxDatGuiButtonEvent event );
		void Event_OnTextInput( ofxDatGuiTextInputEvent event );

		// Time
		float CurrentTime;
		float RoundTime;
		float RoundTimeSpeed;
		bool RoundTimeStep;

		// Camera
		ofCamera Camera;
		ofNode Node_Center;
		ofVec2f MouseDragDistance;
		ofVec2f LastMouse;
		ofVec2f MouseLockPosition;
		bool MouseReset;

		// Level objects
		ofLight Light_Directional;
		ofPlanePrimitive GridPlane;
		ofShader Shader_Grid;
		std::vector<SelectableMovableObjectClass*> SelectableObjects;

		// Model Loading
		ofxAssimpModelLoader** Models;

		// Analytics
		std::vector<AnalyticDataStruct> AnalyticData;
		std::vector<AnalyticDataStruct> AnalyticOverallData;

		// Heatmap
		HeatMapClass HeatMap;

		// Object selection data & shader
		int SelectedNode;
		bool Select;
		int AxisSelected;
		ofShader Shader_Selection;

		// Input
		bool* KeyPressed;

		// GUI
		ofxDatGui* GUI_Analytic;
		ofxDatGuiFolder* GUI_Folder_Analytics;
		ofxDatGuiButton* Button_Node_Add;
		ofxDatGuiValueGraph* Graph_Jump_Start;
		ofxDatGuiButton* Button_Timed_Toggle;
		ofxDatGuiButton* Button_TimeStep_Toggle;
		vector<ofxDatGuiButton*> Button_HeatmapData;
		ofxDatGuiTextInput* TextInput_HeatmapData;
		ofxDatGui* GUI_RoundTime;
};
