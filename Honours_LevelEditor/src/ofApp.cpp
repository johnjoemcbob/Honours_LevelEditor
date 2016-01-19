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
}

//--------------------------------------------------------------
void ofApp::update()
{

}

//--------------------------------------------------------------
void ofApp::draw()
{

}

//--------------------------------------------------------------
void ofApp::keyPressed( int key )
{

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
