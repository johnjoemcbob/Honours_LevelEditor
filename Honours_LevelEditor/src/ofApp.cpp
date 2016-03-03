// Assocciated Header
#include "ofApp.h"

// Framework Headers
#include "ofXml.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	// Init time
	CurrentTime = 0;

	// Init camera
	ofEnableAlphaBlending();
	Camera.setupPerspective();
	Camera.setPosition( ofVec3f( 500, -500, 0 ) );
	Camera.lookAt( ofVec3f( 0, 0, 0 ) );

	// Init light
	Light_Directional.enable();
	Light_Directional.setDirectional();
	Light_Directional.setPosition( ofVec3f( 0.5f, 1, 0.2f ) );
	Light_Directional.setDiffuseColor( ofFloatColor::white );
	Light_Directional.setAmbientColor( ofFloatColor::white );

	ofSetGlobalAmbientColor( ofFloatColor::lightGrey );

	// Init grid plane
	GridPlane.set( 10000, 10000 );
	GridPlane.setMode( ofPrimitiveMode::OF_PRIMITIVE_POINTS );
	GridPlane.setResolution( 100, 100 );
	GridPlane.tilt( 90 );

	AxisSelected = -1;

	Shader_Grid.load( "shaders/fadefrom.vert", "shaders/fadefrom.frag" );
	Shader_Selection.load( "shaders/mouseselection.vert", "shaders/mouseselection.frag" );

	// Load the initial stored analytic data (if it exists)
	LoadAnalytics();

	// Load the current level data (if it exists) - requires Shader_Selection to be loaded
	LoadLevel();

	// Create heatmap
	HeatMap.Initialize();
	{
		int count = 0;
		{
			for each ( AnalyticDataStruct data in AnalyticData )
			{
				if ( data.Key == "JumpStart" )
				{
					count++;
				}
			}
		}
		float* heatpositions = new float[4 * count];
		{
			int current = 0;
			for each ( AnalyticDataStruct data in AnalyticData )
			{
				if ( data.Key == "JumpStart" )
				{
					float x = 0, y = 0, z = 0;
					sscanf( data.Value.c_str(), "%f %f %f", &x, &y, &z );
					heatpositions[current + 0] = x;
					heatpositions[current + 1] = z;
					heatpositions[current + 2] = y;
					heatpositions[current + 3] = 1;
					current += 4;
				}
			}
		}
		HeatMap.SetData( heatpositions, count );
	}

	// Create GUI (Last!)
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
				//
				ofxDatGuiSlider* slider = folder_analytics->addSlider( "Heatmap Strength", 0.5f, 10 );
				{
					slider->bind( HeatMap.GetStrengthReference() );
				}
			}
			//
			GUI_Analytic->addBreak();
		}
	}

	// Initialize test model
	ObjectModelClass* model = new ObjectModelClass();
	{
		model->Initialize( &Shader_Selection, "models/castle_wall.fbx" );
		model->Camera = &Camera;
		model->KeyPressed = &KeyPressed;
		model->mouseX = &mouseX;
		model->mouseY = &mouseY;
		model->AxisSelected = &AxisSelected;
	}
	SelectableObjects.push_back( model );

	// Initialize key presses
	KeyPressed = new bool[3000] { false };
}

//--------------------------------------------------------------
void ofApp::update()
{
	// Update time
	CurrentTime += ofGetLastFrameTime();

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
	HeatMap.Update( CurrentTime );

	for each ( SelectableMovableObjectClass* obj in SelectableObjects )
	{
		obj->Update( CurrentTime );
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	if ( Select )
	{
		ofClear( ofColor::black );
		DrawFrame( true );

		int node = SelectableObjectClass::CheckSelected( mouseX, mouseY );
		if ( node < 3 )
		{
			AxisSelected = node;
		}
		else if ( node > 3 )
		{
			SelectedNode = node;
			AxisSelected = -1;
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
		{
			// Rotate by mouse movement
			cameraforward.rotate( distx * speed, Camera.getUpDir() );
			// Clamp the y axis rotation
			float yangchange = disty * speed;
			{
				float ang = cameraforward.angle( ofVec3f( 0, -1, 0 ) );
				if ( ( ang + yangchange ) > CAMERA_MAX_Y )
				{
					yangchange = CAMERA_MAX_Y - ang;
				}
			}
			cameraforward.rotate( yangchange, Camera.getSideDir() );
		}
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

	}

	for each ( SelectableMovableObjectClass* obj in SelectableObjects )
	{
		if ( obj->GetSelected() )
		{
			obj->OnDragged( x, y, button );
		}
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

	for each ( SelectableMovableObjectClass* obj in SelectableObjects )
	{
		if ( obj->GetSelected() )
		{
			obj->OnPressed( x, y, button );
		}
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

	for each ( SelectableMovableObjectClass* obj in SelectableObjects )
	{
		if ( obj->GetSelected() )
		{
			obj->OnReleased( x, y, button );
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseEntered( int x, int y )
{
	for each ( SelectableMovableObjectClass* obj in SelectableObjects )
	{
		obj->OnHover( x, y );
	}
}

//--------------------------------------------------------------
void ofApp::mouseExited( int x, int y )
{
	for each ( SelectableMovableObjectClass* obj in SelectableObjects )
	{
		obj->OnUnHover( x, y );
	}
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
	ofEnableAlphaBlending();
	{
		//ofEnableLighting();
		//Light_Directional.enable();
		ofDisableLighting();
		Light_Directional.disable();
		{
			// Start Projection
			Camera.begin();
			{
				// Draw nodes
				ofDisableLighting();
				{
					int nodenum = 0;
					for each ( SelectableMovableObjectClass* node in SelectableObjects )
					{
						node->SetSelectName( SELECT_OFFSET + nodenum );
						if ( node->GetSelectName() == SelectedNode )
						{
							node->SetSelected( true );
						}
						else
						{
							node->SetSelected( false );
						}
						node->SetShader( &Shader_Selection );
						node->Draw( select );

						nodenum++;
					}
				}
				ofEnableLighting();

				// Draw grid and heatmap last
				ofDisableLighting();
				{
					if ( !select )
					{
						Shader_Grid.begin();
						{
							Shader_Grid.setUniform1f( "falloff", 5000 );
							Shader_Grid.setUniform3f( "position", ofVec3f::zero() );
							Shader_Grid.setUniform4f( "colour", ofColor::lightGrey );

							GridPlane.drawVertices();
						}
						Shader_Grid.end();
						HeatMap.Draw();
					}
				}
				ofEnableLighting();
			}
			Camera.end();
			// Stop Projection
		}
		Light_Directional.disable();
	}
	ofDisableAlphaBlending();
	ofDisableDepthTest();

	// Draw GUI
	ofDisableLighting();
	{
		DrawFrame_SelectOnly_Shader_Begin( select, 3 );
		{
			GUI_Analytic->draw();

			for each ( SelectableMovableObjectClass* node in SelectableObjects )
			{
				node->DrawGUI( select );
			}
		}
		DrawFrame_SelectOnly_Shader_End( select );
	}
	ofEnableLighting();
}

void ofApp::DrawFrame_SelectOnly_Shader_Begin( bool select, int name )
{
	if ( !select ) return;

	Shader_Selection.begin();
	Shader_Selection.setUniform1i( "name", name );
}

void ofApp::DrawFrame_SelectOnly_Shader_End( bool select )
{
	if ( !select ) return;

	Shader_Selection.end();
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
	ObjectRouteNodeClass* node = new ObjectRouteNodeClass();
	{
		node->Initialize( &Shader_Selection );
		node->Camera = &Camera;
		node->KeyPressed = &KeyPressed;
		node->mouseX = &mouseX;
		node->mouseY = &mouseY;
		node->AxisSelected = &AxisSelected;
	}
	SelectableObjects.push_back( (SelectableMovableObjectClass*) node );
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
			int node = 0;
			for each ( SelectableMovableObjectClass* prim in SelectableObjects )
			{
				if ( prim->GetType() == OBJECTTYPE_NODE )
				{
					XML_LevelOutput.addChild( "node" );
					XML_LevelOutput.setToChild( node );
					{
						XML_LevelOutput.setAttribute( "x", std::to_string( prim->getPosition().x * SCALEFACTOR_EDITOR_TO_UNITY ) );
						XML_LevelOutput.setAttribute( "y", std::to_string( prim->getPosition().y * SCALEFACTOR_EDITOR_TO_UNITY ) );
						XML_LevelOutput.setAttribute( "z", std::to_string( prim->getPosition().z * SCALEFACTOR_EDITOR_TO_UNITY ) );
					}
					XML_LevelOutput.setToParent();
					node++;
				}
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
			float time;
			{
				xml_analyticinput.setToChild( 0 );
				time = xml_analyticinput.getFloatValue();
				xml_analyticinput.setToParent();
			}
			// Get key (second child)
			string key;
			{
				xml_analyticinput.setToChild( 1 );
				key = xml_analyticinput.getValue();
				xml_analyticinput.setToParent();
			}
			// Get value (third child)
			string value;
			{
				xml_analyticinput.setToChild( 2 );
				value = xml_analyticinput.getValue();
				xml_analyticinput.setToParent();
			}
			AnalyticDataStruct data;
			{
				data.Key = key;
				data.Value = value;
				data.Timestamp = time;
			}
			AnalyticData.push_back( data );
		}
		// Go from the current AnalyticDataIndividual to Data
		xml_analyticinput.setToParent();
	}
}

//--------------------------------------------------------------
void ofApp::LoadLevel()
{
	// Read the raw data
	ofFile file;
	{
		file.open( "testlevel.xml" );
		if ( !file.is_open() )
		{
			printf( "Error loading level xml file\n" );
			return;
		}
	}
	ofBuffer buffer = file.readToBuffer();

	// Parse as xml
	ofXml xml_levelinput;
	xml_levelinput.loadFromBuffer( buffer.getText() );

	// Application unique xml parsing (get the data from the tables)
	ParseLevel( xml_levelinput );
}

//--------------------------------------------------------------
void ofApp::ParseLevel( ofXml xml_levelinput )
{
	// Start at data path
	xml_levelinput.setToChild( 0 );

	// Run through each data instance and extract information
	int children = xml_levelinput.getNumChildren();
	for ( int child = 0; child < children; child++ )
	{
		// Go from all level objects to the current level object instance
		xml_levelinput.setToChild( child );
		{
			// Get timestamp (first child)
			string mesh;
			{
				xml_levelinput.setToChild( 0 );
				mesh = xml_levelinput.getValue();
				xml_levelinput.setToParent();
			}
			// Get position (second child with 3 children)
			ofVec3f position;
			{
				xml_levelinput.setToChild( 1 );
				{
					float x, y, z;
					{
						xml_levelinput.setToChild( 0 );
						x = xml_levelinput.getFloatValue();
						xml_levelinput.setToParent();

						xml_levelinput.setToChild( 1 );
						y = xml_levelinput.getFloatValue();
						xml_levelinput.setToParent();

						xml_levelinput.setToChild( 2 );
						z = xml_levelinput.getFloatValue();
						xml_levelinput.setToParent();
					}
					position = ofVec3f( x, -y, z ) * SCALEFACTOR_UNITY_TO_EDITOR;
				}
				xml_levelinput.setToParent();
			}
			// Get value (third child with 3 children)
			ofVec3f rotation;
			{
				xml_levelinput.setToChild( 2 );
				{
					float x, y, z;
					{
						xml_levelinput.setToChild( 0 );
						x = xml_levelinput.getFloatValue();
						xml_levelinput.setToParent();

						xml_levelinput.setToChild( 1 );
						y = xml_levelinput.getFloatValue();
						xml_levelinput.setToParent();

						xml_levelinput.setToChild( 2 );
						z = xml_levelinput.getFloatValue();
						xml_levelinput.setToParent();
					}
					rotation = ofVec3f( x, y, z );
				}
				xml_levelinput.setToParent();
			}

			// Create and position this level element
			//if ( mesh == "Wall" )
			{
				ObjectModelClass* model = new ObjectModelClass();
				{
					model->setPosition( position );
					model->SetRotation( rotation );

					if ( mesh == "Wall" || mesh == "Wall2" )
					{
						model->Initialize( &Shader_Selection, "models/castle_wall.fbx" );
					}
					else if ( mesh == "Wall_Corner" )
					{
						model->Initialize( &Shader_Selection, "models/castle_wallcorner.fbx" );
					}
					else if ( mesh == "GroundTile" )
					{
						model->Initialize( &Shader_Selection, "models/castle_ground.fbx" );
						model->setScale( model->getScale() / 2 );
					}
					else if ( mesh == "Column" )
					{
						model->Initialize( &Shader_Selection, "models/castle_column.fbx" );
					}
					else if ( mesh == "Grass" )
					{
						model->Initialize( &Shader_Selection, "models/castle_grass.fbx" );
					}
					else if ( mesh == "Cube" )
					{
						model->Initialize( &Shader_Selection, "models/roundcube.fbx" );
						model->setScale( model->getScale() / 2 );
					}
					else if ( mesh == "Gate_Big" )
					{
						model->Initialize( &Shader_Selection, "models/castle_gate_big.fbx" );
					}
					model->Camera = &Camera;
					model->KeyPressed = &KeyPressed;
					model->mouseX = &mouseX;
					model->mouseY = &mouseY;
					model->AxisSelected = &AxisSelected;
				}
				SelectableObjects.push_back( model );
			}
		}
		// Go from the current level object back to all level objects
		xml_levelinput.setToParent();
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