// Assocciated Header
#include "Object_Model.h"

ObjectModelClass::ObjectModelClass()
{
	SelectableMovableObjectClass::SelectableMovableObjectClass();

	Type = OBJECTTYPE_MODEL;
}

ObjectModelClass::~ObjectModelClass()
{
	SelectableMovableObjectClass::~SelectableMovableObjectClass();
}

void ObjectModelClass::Initialize( ofShader* shader, string modelpath )
{
	SelectableMovableObjectClass::Initialize( shader );

	Model.loadModel( modelpath );
}

void ObjectModelClass::Update( float time )
{
	SelectableMovableObjectClass::Update( time );


}

void ObjectModelClass::Draw( bool select )
{
	SelectableMovableObjectClass::Draw( select );

	ShaderSelect( select, SelectName );
	{
		ofPushMatrix();
		{
			ofVec3f rot = getGlobalOrientation().getEuler();
			//ofLoadViewMatrix( getGlobalTransformMatrix() );
			//glTranslatef( getGlobalPosition().x, getGlobalPosition().y, getGlobalPosition().z );
			//glRotatef( 1, rot.x, rot.y, rot.z );
			glMultMatrixf( getGlobalTransformMatrix().getPtr() );

			//Model.setPosition( getGlobalPosition().x, getGlobalPosition().y, getGlobalPosition().z );
			for ( int mesh = 0; mesh < Model.getMeshCount(); mesh++ )
			{
				//Model.setRotation( mesh, 1, DEG_TO_RAD * rot.x, DEG_TO_RAD * rot.y, DEG_TO_RAD * rot.z );
			}
			Model.drawFaces();
		}
		ofPopMatrix();
	}
	ShaderSelect( select );
}

void ObjectModelClass::SetRotation( ofVec3f rotation )
{
	ofVec3f offset = ofVec3f( -90, 0, 0 );
	{
		rotation -= offset;
	}
	setOrientation( ofQuaternion( rotation.x, getSideDir(), rotation.y, getUpDir(), rotation.z, getLookAtDir() ) );
}
