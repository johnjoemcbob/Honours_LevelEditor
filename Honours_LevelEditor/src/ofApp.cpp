#include "ofApp.h"

#include "ofXml.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	// Init camera
	ofEnableAlphaBlending();
	Camera.setupPerspective();
	Camera.setPosition( ofVec3f( 500, -500, 0 ) );
	Camera.lookAt( ofVec3f( 0, 0, 0 ) );

	// Init light
	Light_Directional.enable();
	Light_Directional.setPointLight();
	Light_Directional.setPosition( ofVec3f( 0, -100, 0 ) );

	// Init grid plane
	GridPlane.set( 10000, 10000 );
	GridPlane.setMode( ofPrimitiveMode::OF_PRIMITIVE_POINTS );
	GridPlane.setResolution( 100, 100 );
	GridPlane.tilt( 90 );

	AxisSelected = -1;

	shader.load( "shaders/mouseselection.vert", "shaders/mouseselection.frag" );

	// Load the initial stored analytic data (if it exists)
	LoadAnalytics();

	// Create GUI
	GUI_Analytic = new ofxDatGui( ofxDatGuiAnchor::TOP_RIGHT );
	{
		ofxDatGuiTheme* theme = GUI_Analytic->getDefaultTheme();
		{
			theme->font.size = 8;
			theme->font.load();
			theme->layout.width = 196;
			theme->layout.breakHeight = 1080;
		}
		GUI_Analytic->setTheme( theme );
		GUI_Analytic->onButtonEvent( this, &ofApp::Event_OnButton );
		GUI_Analytic->setAutoDraw( false );

		// Add Elements
		{
			GUI_Analytic->addLabel( "HONOURS LEVEL EDITOR" );
			//
			Button_Node_Add = GUI_Analytic->addButton( "- Create Path Node" );
			//
			ofxDatGuiFolder* folder_analytics = GUI_Analytic->addFolder( "Analytics" );
			{
				Graph_Jump_Start = folder_analytics->addValueGraph( "Jump Start", 0, 1 );
				{
					Graph_Jump_Start->setValue( 0, 0.1f );
					Graph_Jump_Start->setValue( 0.2f, 0.2f );
					Graph_Jump_Start->setValue( 0.4f, 0.5f );
					Graph_Jump_Start->setValue( 0.6f, 0.6f );
					Graph_Jump_Start->setValue( 0.8f, 0.6f );
					Graph_Jump_Start->setValue( 1, 0 );
					Graph_Jump_Start->setDrawMode( ofxDatGuiGraph::FILLED );
					Graph_Jump_Start->setEnabled( false );
				}
			}
			//
			GUI_Analytic->addBreak();
		}
	}
}

//--------------------------------------------------------------
void ofApp::update()
{
	// Update input
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
	if ( KeyPressed['o'] )
	{
		SaveLevel();
		KeyPressed['o'] = false;
	}

	GUI_Analytic->update();
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
		printf( "%i\n", colour[0] );
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
void ofApp::exit()
{
	// Cleanup GUI
	delete GUI_Analytic;
	GUI_Analytic = 0;
}

//--------------------------------------------------------------
void ofApp::keyPressed( int key )
{
	key = GetLowerKeyCode( key );

	KeyPressed[key] = true;
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
	else if ( AxisSelected >= 0 )
	{
		ofSpherePrimitive& node = RouteNodes.at( SelectedNode );

		// Reverse directions of movement on the x and y axes dependant on the camera's position respective to the moving object
		float dirx = 1; // Red Axis (Right)
		{
			if ( Camera.getPosition().x <= node.getPosition().x )
			{
				//dirx = -1;
			}
		}
		float diry = 1; // Green Axis (Up)
		float dirz = 1; // Blue Axis (Forward)
		{
			if ( Camera.getPosition().z <= node.getPosition().z )
			{
				//dirz = -1;
			}
		}

		ofVec3f direction;
		ofVec3f offset;
		{
			switch ( AxisSelected )
			{
				case 0: // Blue forward
					offset = node.getLookAtDir();
					direction = Camera.worldToScreen( node.getLookAtDir() * ofVec3f( 0, 0, dirz ), ofGetCurrentViewport() );
					break;
				case 1: // Red right
					offset = node.getSideDir();
					direction = Camera.worldToScreen( node.getSideDir() * ofVec3f( dirx, 0, 0 ), ofGetCurrentViewport() );
					break;
				case 2: // Green up
					offset = node.getUpDir();
					direction = Camera.worldToScreen( node.getUpDir() * ofVec3f( 0, diry, 0 ), ofGetCurrentViewport() );
					break;
				default:
					break;
			}
		}

		// Calculate screen position
		ofVec3f center = Camera.worldToScreen( node.getPosition(), ofGetCurrentViewport() );
		direction /= ofVec3f( ofGetViewportWidth(), ofGetViewportHeight(), 0 );
		center /= ofVec3f( ofGetViewportWidth(), ofGetViewportHeight(), 0 );

		// Check the movement of the cursor in relation to the axis line
		ofVec2f mousedir = LastMouse - ofVec2f( mouseX, mouseY );
		float length = mousedir.length();
		{
			if ( length > 10 )
			{
				length = 10;
			}
		}
		float directionamount = direction.dot( mousedir ) * length / 2;

		// Move the object
		node.move( offset * directionamount );

		// Update lastmouse to this frame for mousedir calculation
		LastMouse = ofVec2f( mouseX, mouseY );
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
	ofVec3f center, forward;

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

							// Forward Axis (Blue)
							DrawFrame_SelectOnly_Shader_Begin( select, 253 );
							{
								ofSetColor( ofColor::blue );
								ofDrawLine( node.getPosition(), node.getPosition() + ( node.getLookAtDir() * length ) );
							}
							DrawFrame_SelectOnly_Shader_End( select );

							// Right Axis (Red)
							DrawFrame_SelectOnly_Shader_Begin( select, 254 );
							{
								ofSetColor( ofColor::red );
								ofDrawLine( node.getPosition(), node.getPosition() + ( node.getSideDir() * length ) );
							}
							DrawFrame_SelectOnly_Shader_End( select );

							// Up Axis (Green)
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
			}
			Camera.end();
			// Stop Projection
		}
	}
	ofDisableDepthTest();

	// Draw GUI
	ofDisableLighting();
	{
		GUI_Analytic->draw();
	}
	ofEnableLighting();
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

//--------------------------------------------------------------
void ofApp::SaveLevel()
{
	ofXml XML_LevelOutput;
	XML_LevelOutput.addChild( "level" );
	{
		XML_LevelOutput.addChild( "route" );
		XML_LevelOutput.setToChild( 0 );
		{
			for ( int node = 0; node < RouteNodes.size(); node++ )
			{
				ofSpherePrimitive prim = RouteNodes.at( node );

				XML_LevelOutput.addChild( "node" );
				XML_LevelOutput.setToChild( node );
				{
					XML_LevelOutput.setAttribute( "x", std::to_string( prim.getPosition().x * SCALEFACTOR_EDITOR_TO_UNITY ) );
					XML_LevelOutput.setAttribute( "y", std::to_string( prim.getPosition().y * SCALEFACTOR_EDITOR_TO_UNITY ) );
					XML_LevelOutput.setAttribute( "z", std::to_string( prim.getPosition().z * SCALEFACTOR_EDITOR_TO_UNITY ) );
				}
				XML_LevelOutput.setToParent();
			}
		}
		XML_LevelOutput.setToParent();
	}
	XML_LevelOutput.save( "test.xml" );
}

//--------------------------------------------------------------
void ofApp::LoadAnalytics()
{
	// Read the raw data
	ofFile file;
	{
		file.open( "testanalytic.xml" );
		if ( !file.is_open() )
		{
			printf( "Error loading analytics xml file\n" );
			return;
		}
	}
	ofBuffer buffer = file.readToBuffer();

	// Parse as xml
	ofXml xml_analyticinput;
	xml_analyticinput.loadFromBuffer( buffer.getText() );

	// Application unique xml parsing (get the data from the tables)
	ParseAnalytics( xml_analyticinput );
}

//--------------------------------------------------------------
void ofApp::ParseAnalytics( ofXml xml_analyticinput )
{
	// Start at data path
	xml_analyticinput.setToChild( 0 );

	// Run through each data instance and extract information
	int children = xml_analyticinput.getNumChildren();
	for ( int child = 0; child < children; child++ )
	{
		// Go from Data to the current AnalyticDataIndividual instance
		xml_analyticinput.setToChild( child );
		{
			// Get timestamp (first child)
			string time;
			{
				xml_analyticinput.setToChild( 0 );
				time = xml_analyticinput.getValue();
				xml_analyticinput.setToParent();
			}
			// Get timestamp (second child)
			string key;
			{
				xml_analyticinput.setToChild( 1 );
				key = xml_analyticinput.getValue();
				xml_analyticinput.setToParent();
			}
			// Get timestamp (third child)
			string value;
			{
				xml_analyticinput.setToChild( 2 );
				value = xml_analyticinput.getValue();
				xml_analyticinput.setToParent();
			}
			printf( "%s %s %s\n", time.c_str(), key.c_str(), value.c_str() );
		}
		// Go from the current AnalyticDataIndividual to Data
		xml_analyticinput.setToParent();
	}
}

//--------------------------------------------------------------
void ofApp::Event_OnButton( ofxDatGuiButtonEvent event )
{
	if ( event.target == Button_Node_Add )
	{
		AddRouteNode( Camera.getPosition() );
	}
}