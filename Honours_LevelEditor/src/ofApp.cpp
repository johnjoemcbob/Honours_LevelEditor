#include "ofApp.h"

#include "ofXml.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofXml XML_LevelOutput;
	XML_LevelOutput.addChild( "level" );
	{
		XML_LevelOutput.addChild( "route" );
		XML_LevelOutput.setToChild( 0 );
		{
			XML_LevelOutput.addChild( "node" );
			XML_LevelOutput.setToChild( 0 );
			{
				XML_LevelOutput.setAttribute( "x", "2.54" );
				XML_LevelOutput.setAttribute( "y", "2.54" );
				XML_LevelOutput.setAttribute( "z", "2.54" );
			}
			XML_LevelOutput.setToParent();

			XML_LevelOutput.addChild( "node" );
			XML_LevelOutput.setToChild( 1 );
			{
				XML_LevelOutput.setAttribute( "x", "4.54" );
				XML_LevelOutput.setAttribute( "y", "2.54" );
				XML_LevelOutput.setAttribute( "z", "2.54" );
			}
			XML_LevelOutput.setToParent();
		}
		XML_LevelOutput.setToParent();
	}
	XML_LevelOutput.save( "test.xml" );

	// Init camera
	Camera.setupPerspective();
	Camera.setPosition( ofVec3f( 500, 500, 0 ) );
	Camera.lookAt( ofVec3f( 0, 0, 0 ) );

	// Init light
	Light_Directional.enable();
	Light_Directional.setDirectional();
	Light_Directional.tilt( 30 );
	Light_Directional.roll( 30 );

	// Setup orbit point as world center
	Node_Center.setPosition( ofVec3f( 0, 0, 0 ) );
	x = 0;
}

//--------------------------------------------------------------
void ofApp::update()
{
	x++;
	Camera.orbit( x, 0, 500, Node_Center );
}

//--------------------------------------------------------------
void ofApp::draw()
{
	Light_Directional.draw();
	Camera.begin();
	{
		ofSetColor( 255, 0, 0 );
		ofFill();
		ofDrawBox( 0, 0, 0, 200, 100, 200 );
	}
	Camera.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed( int key )
{
	if ( key == 'w' )
	{
		Camera.tilt( -1 );
	}
	if ( key == 's' )
	{
		Camera.tilt( 1 );
	}
	if ( key == 'a' )
	{
		Camera.pan( 1 );
	}
	if ( key == 'd' )
	{
		Camera.pan( -1 );
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased( int key )
{

}

//--------------------------------------------------------------
void ofApp::mouseMoved( int x, int y )
{

}

//--------------------------------------------------------------
void ofApp::mouseDragged( int x, int y, int button )
{

}

//--------------------------------------------------------------
void ofApp::mousePressed( int x, int y, int button )
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased( int x, int y, int button )
{

}

//--------------------------------------------------------------
void ofApp::mouseEntered( int x, int y )
{

}

//--------------------------------------------------------------
void ofApp::mouseExited( int x, int y )
{

}

//--------------------------------------------------------------
void ofApp::windowResized( int w, int h )
{

}

//--------------------------------------------------------------
void ofApp::gotMessage( ofMessage msg )
{

}

//--------------------------------------------------------------
void ofApp::dragEvent( ofDragInfo dragInfo )
{

}
