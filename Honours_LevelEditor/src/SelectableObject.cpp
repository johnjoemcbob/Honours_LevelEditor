// Assocciated Header
#include "SelectableObject.h"

SelectableObjectClass::SelectableObjectClass()
{
	KeyPressed = 0;
	mouseX = 0;
	mouseY = 0;

	ShaderEnabled = false;
	SelectName = 0;
	Selected = false;

	Type = OBJECTTYPE_DEFAULT;

	GUI = 0;
}

SelectableObjectClass::~SelectableObjectClass()
{
	// Cleanup GUI
	if ( GUI )
	{
		delete GUI;
		GUI = 0;
	}
}

void SelectableObjectClass::Initialize( ofShader* shader )
{
	Shader_Select = shader;
}

void SelectableObjectClass::Update( float time )
{
	if ( GUI )
	{
		GUI->update();
	}
}

void SelectableObjectClass::Draw( bool select )
{

}

void SelectableObjectClass::DrawGUI( bool select )
{
	if ( GUI )
	{
		// Draw GUI
		GUI->draw();
	}
}

void SelectableObjectClass::OnPressed( int x, int y, int button )
{

}

void SelectableObjectClass::OnDragged( int x, int y, int button )
{

}

void SelectableObjectClass::OnReleased( int x, int y, int button )
{

}

void SelectableObjectClass::OnHover( float x, float y )
{

}

void SelectableObjectClass::OnUnHover( float x, float y )
{

}

void SelectableObjectClass::OnSelect()
{
	CreateGUI();
}

void SelectableObjectClass::OnUnSelect()
{
	GUI->setVisible( false );
	GUI->setEnabled( false );
	//delete GUI;
	//GUI = 0;
}

void SelectableObjectClass::SetSelected( bool select )
{
	if ( !Selected && select )
	{
		OnSelect();
	}
	if ( Selected && !select )
	{
		OnUnSelect();
	}
	Selected = select;
}

void SelectableObjectClass::ShaderSelect( bool select, unsigned int name )
{
	if ( !select ) return;

	if ( !ShaderEnabled )
	{
		Shader_Select->begin();
		Shader_Select->setUniform1i( "name", name );
	}
	else
	{
		Shader_Select->end();
	}
	ShaderEnabled = !ShaderEnabled;
}

bool SelectableObjectClass::CreateGUI()
{
	if ( GUI )
	{
		GUI->setVisible( true );
		GUI->setEnabled( true );
		return false;
	}

	GUI = new ofxDatGui();
	{
		ofxDatGuiTheme* theme = GUI->getDefaultTheme();
		{
			//theme->font.size = 8;
			//theme->font.load();
			theme->layout.width = 428;
			theme->layout.breakHeight = 1080;
		}
		GUI->setTheme( theme );

		GUI->addHeader( "Selected Object" );
	}
	GUI->setAutoDraw( false );

	return true;
}

// r g b, b fill full first, then g, then r
unsigned int SelectableObjectClass::CheckSelected( float mousex, float mousey )
{
	// Read colour of clicked pixel to determine object
	unsigned char colour[4];
	glReadPixels( mousex, ofGetViewportHeight() - mousey, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &colour );

	// Handle selection logic
	unsigned int id = 0;
	{
		id += colour[0] * 255 * 255;
		id += colour[1] * 255;
		id += colour[2];
	}
	printf( "id: %i\n", id );
	printf( "col: %i %i %i\n", colour[0], colour[1], colour[2] );
	return id;
}
// temp testing
//{
//	for ( int n = 60000; n < 70000; n++ )
//	{
//		ofVec3f col;
//		{
//			col.x = floor( (float) n / ( 255 * 255 ) );
//			col.y = floor( ( (float) n / ( 255 ) ) - ( col.x * 255 ) );
//			col.z = floor( (float) n - ( col.x * 255 * 255 ) - ( col.y * 255 ) );
//		}
//		printf( "Test %i: %f %f %f\n", n, col.x, col.y, col.z );
//	}
//}