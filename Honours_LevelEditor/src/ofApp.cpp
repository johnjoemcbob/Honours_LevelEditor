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
	Camera.setPosition( ofVec3f( 500, -500, 0 ) );
	Camera.lookAt( ofVec3f( 0, 0, 0 ) );

	// Init light
	Light_Directional.enable();
	Light_Directional.setPointLight();
	Light_Directional.setPosition( ofVec3f( 0, -100, 0 ) );

	// Setup orbit point as world center
	Node_Center.setPosition( ofVec3f( 0, 0, 0 ) );

	// Init grid plane
	GridPlane.set( 10000, 10000 );
	GridPlane.setMode( ofPrimitiveMode::OF_PRIMITIVE_POINTS );
	GridPlane.setResolution( 100, 100 );
	GridPlane.tilt( 90 );

	// Init test box
	Box_Test.set( 100 );
}

//--------------------------------------------------------------
void ofApp::update()
{
	double speed = 100 * ofGetLastFrameTime();
	if ( KeyPressed[OF_KEY_SHIFT] )
	{
		speed *= 10;
	}
	if ( KeyPressed['w'] )
	{
		Camera.move( Camera.getLookAtDir() * speed );
	}
	if ( KeyPressed['s'] )
	{
		Camera.move( -Camera.getLookAtDir() * speed );
	}
	if ( KeyPressed['a'] )
	{
		Camera.move( -Camera.getSideDir() * speed );
	}
	if ( KeyPressed['d'] )
	{
		Camera.move( Camera.getSideDir() * speed );
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	// Draw for visual
	DrawFrame();

	// Draw for object selection
	if ( KeyPressed['q'] )
	{
		KeyPressed['q'] = false;
		// Initialize buffers
		GLuint buffer[BUF_SIZE] = { 0 };
		glSelectBuffer( BUF_SIZE, buffer );
		GLint viewport[4];
		glGetIntegerv( GL_VIEWPORT, viewport );
		GLfloat proj_matrix[16];
		glGetFloatv( GL_PROJECTION_MATRIX, proj_matrix );

		// Switch to selection mode
		glRenderMode( GL_SELECT );
		{
			// Clear old name listings
			glInitNames();

			// Projection matrix for moving the selection area
			glMatrixMode( GL_PROJECTION );
			glPushMatrix();
			{
				// Define selection area
				glLoadIdentity();
				gluPickMatrix( (GLdouble) mouseX, (GLdouble) ( ofGetHeight() - mouseY ), 1, 1, viewport );
				glMultMatrixf( proj_matrix );
				printf( "%i %i\n\n", mouseX, mouseY );

				// Modelview matrix for displaying the scene
				glMatrixMode( GL_MODELVIEW );
				{
					gluPerspective( Camera.getFov(), Camera.getAspectRatio(), Camera.getNearClip(), Camera.getFarClip() );
					
					DrawFrame( true );
				}
				glMatrixMode( GL_PROJECTION );
			}
			glPopMatrix();
		}
		int hits = glRenderMode( GL_RENDER );
		glMatrixMode( GL_PROJECTION );

		unsigned int i, j;
		GLuint names, *ptr;

		printf( "hits = %d\n", hits );
		ptr = (GLuint *) buffer;
		for ( i = 0; i < hits; i++ )
		{ /*  for each hit  */
			names = *ptr;
			printf( " number of names for this hit = %d\n", names );
			ptr++;
			printf( "  z1 is %g;", (float) *ptr / 0x7fffffff ); ptr++;
			printf( " z2 is %g\n", (float) *ptr / 0x7fffffff ); ptr++;
			printf( "   names are " );
			for ( j = 0; j < names; j++ )
			{ /*  for each name */
				printf( "%d ", *ptr );
				ptr++;
			}
			printf( "\n" );
		}

		glMatrixMode( GL_MODELVIEW );
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed( int key )
{
	key = GetLowerKeyCode( key );

	KeyPressed[key] = true;

	if ( key == 'e' )
	{
		AddRouteNode( Camera.getPosition() );
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased( int key )
{
	key = GetLowerKeyCode( key );

	KeyPressed[key] = false;
}

//--------------------------------------------------------------
void ofApp::mouseMoved( int x, int y )
{
	LastMouse = ofVec2f( x, y );
}

//--------------------------------------------------------------
void ofApp::mouseDragged( int x, int y, int button )
{
	if ( button == INPUT_CAMERA_ROTATE )
	{
		if ( MouseReset )
		{
			MouseReset = false;
			LastMouse = ofVec2f( mouseX, mouseY );
			return;
		}

		// Move camera by mouse drag amount
		float speed = 1.0f / 10;
		float distx = ( LastMouse.x - mouseX );
		float disty = ( mouseY - LastMouse.y );
		//Camera.tilt( disty * speed );
		//Camera.pan( distx * speed );
		CameraRotation.x += distx * speed;
		CameraRotation.y += disty * speed;

		ofVec3f cameraforward = Camera.getLookAtDir();
		cameraforward.rotate( distx * speed, Camera.getUpDir() );
		cameraforward.rotate( disty * speed, Camera.getSideDir() );
		Camera.lookAt( Camera.getPosition() + cameraforward.normalize() );

		// Offset by center of window
		float offx = ofGetWindowPositionX();
		float offy = ofGetWindowPositionY();

		// Confine cursor to area around click
		if ( MouseLockPosition.distance( ofVec2f( mouseX, mouseY ) ) > 5 )
		{
			SetCursorPos( MouseLockPosition.x + offx, MouseLockPosition.y + offy );
			MouseReset = true;
		}
		LastMouse = ofVec2f( mouseX, mouseY );
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed( int x, int y, int button )
{
	if ( button == INPUT_SELECT )
	{
		for each ( ofSpherePrimitive node in RouteNodes )
		{

		}
	}
	if ( button == INPUT_CAMERA_ROTATE )
	{
		ofHideCursor();
		MouseLockPosition = ofVec2f( x, y );
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased( int x, int y, int button )
{
	if ( button == INPUT_CAMERA_ROTATE )
	{
		ofShowCursor();
	}
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

//--------------------------------------------------------------
void ofApp::DrawFrame( bool select )
{
	// Clear screen to gradient
	ofDisableLighting();
	{
		ofBackgroundGradient( ofColor( 64 ), ofColor( 0 ) );
	}
	ofEnableLighting();

	ofSetColor( 255 );
	ofEnableDepthTest();
	{
		ofEnableLighting();
		Light_Directional.enable();
		{
			// Start Projection
			if ( !select )
			{
				Camera.begin();
			}
			// Start Projection
			{
				ofDisableLighting();
				{
					GridPlane.drawVertices();

					ofSetColor( 255, 255, 255 );
					int nodenum = 0;
					for each ( ofSpherePrimitive node in RouteNodes )
					{
						if ( nodenum == SelectedNode )
						{
							ofSetColor( 0, 255, 0 );
						}
						glPushName( nodenum + 1 );
						node.draw();
						glPopName();
						ofSetColor( 255, 255, 255 );
						nodenum++;
					}
				}
				ofEnableLighting();

				ofSetColor( 255, 0, 0 );
				ofFill();
				glPushName( 0 );
				Box_Test.draw();
				glPopName();

				Light_Directional.draw();
			}
			// Stop Projection
			if ( !select )
			{
				Camera.end();
			}
			// Stop Projection
		}
	}
	ofDisableDepthTest();

	ofSetColor( 255 );
}

//--------------------------------------------------------------
int ofApp::GetLowerKeyCode( int key )
{
	// Ignore shift/control + key
	if ( key < 200 )
	{
		if ( KeyPressed[OF_KEY_SHIFT] )
		{
			key += 32; // From upper to lower case
		}
		if ( KeyPressed[OF_KEY_CONTROL] )
		{
			key += 96; // From command to lower case
		}
	}
	return key;
}

//--------------------------------------------------------------
void ofApp::AddRouteNode( ofVec3f pos )
{
	ofSpherePrimitive node;
	{
		node.set( 50, 4 );
		node.setPosition( pos );
	}
	RouteNodes.push_back( node );
}