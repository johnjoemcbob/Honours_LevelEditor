// Assocciated Header
#include "SelectableMovableObject.h"

SelectableMovableObjectClass::SelectableMovableObjectClass()
{
	SelectableObjectClass::SelectableObjectClass();

	LastMouse = ofVec2f::zero();

	// Initialize gui variables
	SliderX = 0;
	SliderY = 0;
	SliderZ = 0;
}

SelectableMovableObjectClass::~SelectableMovableObjectClass()
{
	SelectableObjectClass::~SelectableObjectClass();
}

void SelectableMovableObjectClass::Initialize( ofShader* shader )
{
	SelectableObjectClass::Initialize( shader );

	// Initialize gui variables
	SliderX = getPosition().x;
	SliderY = getPosition().y;
	SliderZ = getPosition().z;
}

void SelectableMovableObjectClass::Update( float time )
{
	SelectableObjectClass::Update( time );


}

void SelectableMovableObjectClass::Draw( bool select )
{
	setPosition( SliderX, SliderY, SliderZ );

	SelectableObjectClass::Draw( select );
}

void SelectableMovableObjectClass::DrawGUI( bool select )
{
	// Draw axes (if selected)
	if ( Selected )
	{
		ofSetLineWidth( 100 );
		float length = 200;

		Camera->begin();
		{
			// Forward Axis (Blue)
			ShaderSelect( select, 0 );
			{
				ofSetColor( ofColor::blue );
				ofDrawLine( getPosition(), getPosition() + ( getLookAtDir() * length ) );
			}
			ShaderSelect( select );

			// Right Axis (Red)
			ShaderSelect( select, 1 );
			{
				ofSetColor( ofColor::red );
				ofDrawLine( getPosition(), getPosition() + ( getSideDir() * length ) );
			}
			ShaderSelect( select );

			// Up Axis (Green)
			ShaderSelect( select, 2 );
			{
				ofSetColor( ofColor::green );
				ofDrawLine( getPosition(), getPosition() + ( getUpDir() * -length ) );
			}
			ShaderSelect( select );
		}
		Camera->end();

		ofSetColor( 255, 255, 255 );
	}

	ShaderSelect( select, 3 );
	{
		SelectableObjectClass::DrawGUI( select );
	}
	ShaderSelect( select );
}

void SelectableMovableObjectClass::OnPressed( int x, int y, int button )
{
	SelectableObjectClass::OnPressed( x, y, button );


}

void SelectableMovableObjectClass::OnDragged( int x, int y, int button )
{
	SelectableObjectClass::OnDragged( x, y, button );

	// Move along the axis selected
	{
		// Different directions depending on axis selected
		ofVec3f offset;
		{
			// Get different axis depending on selected
			switch ( *AxisSelected )
			{
				case 0: // Blue forward
					offset = getLookAtDir();
					break;
				case 1: // Red right
					offset = getSideDir();
					break;
				case 2: // Green up
					offset = -getUpDir();
					break;
				default:
					break;
			}
		}

		// Calculate direction of axis in screen space
		ofVec2f direction = Camera->worldToScreen( getPosition() - offset, ofGetCurrentViewport() );
		direction /= ofVec2f( ofGetViewportWidth(), ofGetViewportHeight() );

		// Calculate the position of the object in screen space
		ofVec2f position = Camera->worldToScreen( getPosition(), ofGetCurrentViewport() );
		position /= ofVec2f( ofGetViewportWidth(), ofGetViewportHeight() );

		// Get the direction vector from these two aspects
		ofVec2f axisdir = direction - position;
		axisdir.normalize();

		// Check the movement of the cursor in relation to the axis line
		ofVec2f mousedir = LastMouse - ofVec2f( *mouseX, *mouseY );
		mousedir.y *= -1;
		float length = mousedir.length();
		{
			if ( length > 10 )
			{
				length = 10;
			}
		}
		float directionamount = axisdir.dot( mousedir ) * length / 2;

		// Check for snapping enabled
		if ( KeyPressed[OF_KEY_SHIFT] )
		{
			if ( abs( directionamount ) > GRID_SNAP_FORCE )
			{
				// Get the length along this axis
				ofVec3f nodedir = getPosition() * offset;
				float length = nodedir.length();
				if ( ( nodedir.x > 0 ) || ( nodedir.z > 0 ) )
				{
					length *= -1;
				}

				// Divide and floor to get closest minimum grid snap point on this axis
				length = floor( length / GRID_SNAP_DISTANCE );

				// Correct to move in the right direction (i.e. backwards or forwards along the vector)
				if ( signbit( directionamount ) )
				{
					// Take 1 to move to the next grid point
					length--;
				}
				else
				{
					// Plus 1 to move to the next grid point
					length++;
				}

				// Move
				length *= GRID_SNAP_DISTANCE;
				ofVec3f pos = getPosition();
				{
					if ( offset.x != 0 )
					{
						pos.x = length;
					}
					if ( offset.y != 0 )
					{
						pos.y = -length;
					}
					if ( offset.z != 0 )
					{
						pos.z = length;
					}
				}
				setPosition( pos );
			}
		}
		else
		{
			// Move the object
			move( offset * directionamount );
		}

		// Update lastmouse to this frame for mousedir calculation
		LastMouse = ofVec2f( *mouseX, *mouseY );

		// Update slider variables
		SliderX = CLAMP( getPosition().x, -AXISLIMIT_X, AXISLIMIT_X );
		SliderY = CLAMP( getPosition().y, -AXISLIMIT_Y, AXISLIMIT_Y );
		SliderZ = CLAMP( getPosition().z, -AXISLIMIT_Z, AXISLIMIT_Z );
	}
}

void SelectableMovableObjectClass::OnReleased( int x, int y, int button )
{
	SelectableObjectClass::OnReleased( x, y, button );


}

void SelectableMovableObjectClass::OnHover( float x, float y )
{
	SelectableObjectClass::OnHover( x, y );


}

void SelectableMovableObjectClass::OnUnHover( float x, float y )
{
	SelectableObjectClass::OnUnHover( x, y );


}

void SelectableMovableObjectClass::OnSelect()
{
	SelectableObjectClass::OnSelect();


}

void SelectableMovableObjectClass::OnUnSelect()
{
	SelectableObjectClass::OnUnSelect();


}

bool SelectableMovableObjectClass::CreateGUI()
{
	bool cont = SelectableObjectClass::CreateGUI();
	if ( !cont ) return false;

	// Add extra gui controls
	{
		ofxDatGuiSlider* slider;

		slider = GUI->addSlider( "Position X", -AXISLIMIT_X, AXISLIMIT_X );
		{
			slider->bind( SliderX );
		}
		//
		slider = GUI->addSlider( "Position Y", -AXISLIMIT_Y, AXISLIMIT_Y );
		{
			slider->bind( SliderY );
		}
		//
		slider = GUI->addSlider( "Position Z", -AXISLIMIT_Z, AXISLIMIT_Z );
		{
			slider->bind( SliderZ );
		}
	}

	return true;
}
