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
	ofEnableAlphaBlending();
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

	AxisSelected = -1;

	shader.load( "shaders/mouseselection.vert", "shaders/mouseselection.frag" );
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
	if ( Select )
	{
		DrawFrame( true );

		// Read colour of clicked pixel to determine object
		unsigned char colour[4];
		glReadPixels( mouseX, ofGetViewportHeight() - mouseY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &colour );

		// Handle selection logic
		if ( colour[0] < 253 )
		{
			// Is an object
			SelectedNode = colour[0] - 1;
		}
		else
		{
			// Is an axis for the currently selected object
			AxisSelected = colour[0] - 253;
		}

		// Flagged as true when the mouse is clicked
		Select = false;
	}

	// Draw for visual
	ofClear( ofColor::black );
	DrawFrame();
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
	if ( MouseReset )
	{
		MouseReset = false;
		LastMouse = ofVec2f( mouseX, mouseY );
		return;
	}
	// Determine the difference in mouse positions since last frame
	float distx = ( LastMouse.x - mouseX );
	float disty = ( mouseY - LastMouse.y );

	if ( button == INPUT_CAMERA_ROTATE )
	{
		// Move camera by mouse drag amount
		float speed = 1.0f / 10;
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

	// Move selected object along its axis if it was dragged
	if ( AxisSelected >= 0 )
	{
		ofSpherePrimitive& node = RouteNodes.at( SelectedNode );

		ofVec3f offset;
		{
			switch ( AxisSelected )
			{
				case 0:
					offset = node.getLookAtDir() * distx;
					break;
				case 1:
					offset = node.getSideDir() * distx;
					break;
				case 2:
					offset = node.getUpDir() * disty;
					break;
				default:
					break;
			}

		}
		node.move( offset );
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed( int x, int y, int button )
{
	if ( button == INPUT_SELECT )
	{
		Select = true;
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

	// No more axis moving
	AxisSelected = -1;
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
	if ( !select )
	{
		ofDisableLighting();
		{
			ofBackgroundGradient( ofColor( 64 ), ofColor( 0 ) );
		}
		ofEnableLighting();
	}

	ofSetColor( 255 );
	ofFill();
	ofEnableDepthTest();
	{
		ofEnableLighting();
		Light_Directional.enable();
		{
			// Start Projection
			Camera.begin();
			{
				ofDisableLighting();
				{
					if ( !select )
					{
						GridPlane.drawVertices();
					}

					ofSetColor( 255, 255, 255 );
					int nodenum = 0;
					for each ( ofSpherePrimitive node in RouteNodes )
					{
						if ( nodenum == SelectedNode )
						{
							ofSetLineWidth( 100 );
							float length = 200;

							// Forward Axis
							DrawFrame_SelectOnly_Shader_Begin( select, 253 );
							{
								ofSetColor( ofColor::blue );
								ofDrawLine( node.getPosition(), node.getPosition() + ( node.getLookAtDir() * length ) );
							}
							DrawFrame_SelectOnly_Shader_End( select );

							// Right Axis
							DrawFrame_SelectOnly_Shader_Begin( select, 254 );
							{
								ofSetColor( ofColor::red );
								ofDrawLine( node.getPosition(), node.getPosition() + ( node.getSideDir() * length ) );
							}
							DrawFrame_SelectOnly_Shader_End( select );

							// Up Axis
							DrawFrame_SelectOnly_Shader_Begin( select, 255 );
							{
								ofSetColor( ofColor::green );
								ofDrawLine( node.getPosition(), node.getPosition() + ( node.getUpDir() * -length ) );
							}
							DrawFrame_SelectOnly_Shader_End( select );
						}

						DrawFrame_SelectOnly_Shader_Begin( select, 1 + nodenum );
						{
							node.draw();
						}
						DrawFrame_SelectOnly_Shader_End( select );

						ofSetColor( 255, 255, 255 );
						nodenum++;
					}
				}
				ofEnableLighting();

				ofSetColor( 255, 0, 0 );
				DrawFrame_SelectOnly_Shader_Begin( select, 0 );
				{
					Box_Test.draw();
				}
				DrawFrame_SelectOnly_Shader_End( select );

				Light_Directional.draw();
			}
			Camera.end();
			// Stop Projection
		}
	}
	ofDisableDepthTest();

	ofSetColor( 255 );
}

void ofApp::DrawFrame_SelectOnly_Shader_Begin( bool select, int name )
{
	if ( !select ) return;

	shader.begin();
	shader.setUniform1i( "name", name );
}

void ofApp::DrawFrame_SelectOnly_Shader_End( bool select )
{
	if ( !select ) return;

	shader.end();
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