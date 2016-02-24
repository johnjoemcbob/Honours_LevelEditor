#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"

#include <vector>

#include "HeatMap.h"

#define BUF_SIZE 512

#define INPUT_SELECT 0
#define INPUT_CAMERA_ROTATE 2

#define SCALEFACTOR_EDITOR_TO_UNITY 1 / 100

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
		void DrawFrame( bool select = false );
		void DrawFrame_SelectOnly_Shader_Begin( bool select = false, int name = 0 );
		void DrawFrame_SelectOnly_Shader_End( bool select = false );
		int GetLowerKeyCode( int key );
		void AddRouteNode( ofVec3f pos );
		void SaveLevel();
		void LoadAnalytics();
		void ParseAnalytics( ofXml xml_analyticinput );

		void Event_OnButton( ofxDatGuiButtonEvent event );

		// Time
		float CurrentTime;

		// Camera
		ofCamera Camera;
		ofNode Node_Center;
		ofVec2f LastMouse;
		ofVec2f MouseLockPosition;
		bool MouseReset;

		// Level objects
		ofLight Light_Directional;
		ofPlanePrimitive GridPlane;
		std::vector<ofSpherePrimitive> RouteNodes;

		// Analytics
		std::vector<AnalyticDataStruct> AnalyticData;

		// Heatmap
		HeatMapClass HeatMap;

		// Object selection data & shader
		int SelectedNode;
		bool Select;
		int AxisSelected;
		ofShader shader;

		// Input
		bool KeyPressed[3000];

		// GUI
		ofxDatGui* GUI_Analytic;
		ofxDatGuiButton* Button_Node_Add;
		ofxDatGuiValueGraph* Graph_Jump_Start;
		ofxDatGuiWaveMonitor* test;
};
