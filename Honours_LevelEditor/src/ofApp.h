#pragma once

#include "ofMain.h"

#include <vector>

#define BUF_SIZE 512

#define INPUT_SELECT 1
#define INPUT_CAMERA_ROTATE 2

class ofApp : public ofBaseApp
{
	public:
		void setup();
		void update();
		void draw();

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
		int GetLowerKeyCode( int key );
		void AddRouteNode( ofVec3f pos );

		ofCamera Camera;
		ofLight Light_Directional;
		ofNode Node_Center;
		ofVec2f CameraRotation;
		ofVec2f LastMouse;
		ofVec2f MouseLockPosition;
		bool MouseReset;

		ofPlanePrimitive GridPlane;
		ofBoxPrimitive Box_Test;
		std::vector<ofSpherePrimitive> RouteNodes;
		int SelectedNode;

		bool KeyPressed[3000];
};
