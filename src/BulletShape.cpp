#include "BulletShape.h"



BulletShape::BulletShape(btCollisionShape* newShape, btTransform newTransform, btScalar newMass, float vertScale, int buffer, bool isMagnet, bool isMetal)
{
	magnet = isMagnet;
	metal = isMetal;
	shape = newShape;
	motionState = new btDefaultMotionState(newTransform);
	mass = newMass;
	rigidBody = new btRigidBody(mass, motionState, shape, btVector3(0, 0, 0));
	arrayBuffer = buffer;
	vertexBuffer = buffer;
	vertExtent = vertScale;
	CreateVertexData(vertScale);
	charge = 1.0f;
}


BulletShape::~BulletShape()
{
	delete rigidBody->getMotionState();
	delete rigidBody;
	delete shape;
}

void BulletShape::PickColor(int col)
{
	switch (col)
	{
	case 0:
		color[0] = 0.5f;	color[1] = 0.5f;	color[2] = 0.5f;	color[3] = 1.0f;
		break;
	case 1:
		color[0] = 1.0f;	color[1] = 0.2f;	color[2] = 0.2f;	color[3] = 1.0f;
		break;
	case 2:
		color[0] = 0.2f;	color[1] = 0.5f;	color[2] = 1.0f;	color[3] = 1.0f;
		break;
	case 3:
		color[0] = 0.2f;	color[1] = 1.0f;	color[2] = 0.5f;	color[3] = 1.0f;
		break;
	}
}

void BulletShape::CreateVertexData(float vs)
{
	std::string name = shape->getName();
	if (name == "Box")
	{
		CreateBox(vs);
		PickColor(2);
	}
	else if (name == "STATICPLANE")
	{
		CreatePlane();
		PickColor(0);
	}
}

void BulletShape::CreateBox(float vs)
{
	//							VERTEX DATA																NORMAL DATA
	vertexData.push_back(-vs);	vertexData.push_back(-vs);	vertexData.push_back(-vs);		vertexData.push_back(0.0f);	vertexData.push_back(0.0f);	vertexData.push_back(-1.0f);
	vertexData.push_back(vs);	vertexData.push_back(-vs);	vertexData.push_back(-vs);		vertexData.push_back(0.0f);	vertexData.push_back(0.0f);	vertexData.push_back(-1.0f);
	vertexData.push_back(vs);	vertexData.push_back(vs);	vertexData.push_back(-vs);		vertexData.push_back(0.0f);	vertexData.push_back(0.0f);	vertexData.push_back(-1.0f);
	vertexData.push_back(vs);	vertexData.push_back(vs);	vertexData.push_back(-vs);		vertexData.push_back(0.0f);	vertexData.push_back(0.0f);	vertexData.push_back(-1.0f);
	vertexData.push_back(-vs);	vertexData.push_back(vs);	vertexData.push_back(-vs);		vertexData.push_back(0.0f);	vertexData.push_back(0.0f);	vertexData.push_back(-1.0f);
	vertexData.push_back(-vs);	vertexData.push_back(-vs);	vertexData.push_back(-vs);		vertexData.push_back(0.0f);	vertexData.push_back(0.0f);	vertexData.push_back(-1.0f);

	vertexData.push_back(-vs);	vertexData.push_back(-vs);	vertexData.push_back(vs);		vertexData.push_back(0.0f);	vertexData.push_back(0.0f);	vertexData.push_back(1.0f);
	vertexData.push_back(vs);	vertexData.push_back(-vs);	vertexData.push_back(vs);		vertexData.push_back(0.0f);	vertexData.push_back(0.0f);	vertexData.push_back(1.0f);
	vertexData.push_back(vs);	vertexData.push_back(vs);	vertexData.push_back(vs);		vertexData.push_back(0.0f);	vertexData.push_back(0.0f);	vertexData.push_back(1.0f);
	vertexData.push_back(vs);	vertexData.push_back(vs);	vertexData.push_back(vs);		vertexData.push_back(0.0f);	vertexData.push_back(0.0f);	vertexData.push_back(1.0f);
	vertexData.push_back(-vs);	vertexData.push_back(vs);	vertexData.push_back(vs);		vertexData.push_back(0.0f);	vertexData.push_back(0.0f);	vertexData.push_back(1.0f);
	vertexData.push_back(-vs);	vertexData.push_back(-vs);	vertexData.push_back(vs);		vertexData.push_back(0.0f);	vertexData.push_back(0.0f);	vertexData.push_back(1.0f);

	vertexData.push_back(-vs);	vertexData.push_back(vs);	vertexData.push_back(vs);		vertexData.push_back(-1.0f);	vertexData.push_back(0.0f);	vertexData.push_back(0.0f);
	vertexData.push_back(-vs);	vertexData.push_back(vs);	vertexData.push_back(-vs);		vertexData.push_back(-1.0f);	vertexData.push_back(0.0f);	vertexData.push_back(0.0f);
	vertexData.push_back(-vs);	vertexData.push_back(-vs);	vertexData.push_back(-vs);		vertexData.push_back(-1.0f);	vertexData.push_back(0.0f);	vertexData.push_back(0.0f);
	vertexData.push_back(-vs);	vertexData.push_back(-vs);	vertexData.push_back(-vs);		vertexData.push_back(-1.0f);	vertexData.push_back(0.0f);	vertexData.push_back(0.0f);
	vertexData.push_back(-vs);	vertexData.push_back(-vs);	vertexData.push_back(vs);		vertexData.push_back(-1.0f);	vertexData.push_back(0.0f);	vertexData.push_back(0.0f);
	vertexData.push_back(-vs);	vertexData.push_back(vs);	vertexData.push_back(vs);		vertexData.push_back(-1.0f);	vertexData.push_back(0.0f);	vertexData.push_back(0.0f);

	vertexData.push_back(vs);	vertexData.push_back(vs);	vertexData.push_back(vs);		vertexData.push_back(1.0f);	vertexData.push_back(0.0f);	vertexData.push_back(0.0f);
	vertexData.push_back(vs);	vertexData.push_back(vs);	vertexData.push_back(-vs);		vertexData.push_back(1.0f);	vertexData.push_back(0.0f);	vertexData.push_back(0.0f);
	vertexData.push_back(vs);	vertexData.push_back(-vs);	vertexData.push_back(-vs);		vertexData.push_back(1.0f);	vertexData.push_back(0.0f);	vertexData.push_back(0.0f);
	vertexData.push_back(vs);	vertexData.push_back(-vs);	vertexData.push_back(-vs);		vertexData.push_back(1.0f);	vertexData.push_back(0.0f);	vertexData.push_back(0.0f);
	vertexData.push_back(vs);	vertexData.push_back(-vs);	vertexData.push_back(vs);		vertexData.push_back(1.0f);	vertexData.push_back(0.0f);	vertexData.push_back(0.0f);
	vertexData.push_back(vs);	vertexData.push_back(vs);	vertexData.push_back(vs);		vertexData.push_back(1.0f);	vertexData.push_back(0.0f);	vertexData.push_back(0.0f);

	vertexData.push_back(-vs);	vertexData.push_back(-vs);	vertexData.push_back(-vs);		vertexData.push_back(0.0f);	vertexData.push_back(-1.0f);	vertexData.push_back(0.0f);
	vertexData.push_back(vs);	vertexData.push_back(-vs);	vertexData.push_back(-vs);		vertexData.push_back(0.0f);	vertexData.push_back(-1.0f);	vertexData.push_back(0.0f);
	vertexData.push_back(vs);	vertexData.push_back(-vs);	vertexData.push_back(vs);		vertexData.push_back(0.0f);	vertexData.push_back(-1.0f);	vertexData.push_back(0.0f);
	vertexData.push_back(vs);	vertexData.push_back(-vs);	vertexData.push_back(vs);		vertexData.push_back(0.0f);	vertexData.push_back(-1.0f);	vertexData.push_back(0.0f);
	vertexData.push_back(-vs);	vertexData.push_back(-vs);	vertexData.push_back(vs);		vertexData.push_back(0.0f);	vertexData.push_back(-1.0f);	vertexData.push_back(0.0f);
	vertexData.push_back(-vs);	vertexData.push_back(-vs);	vertexData.push_back(-vs);		vertexData.push_back(0.0f);	vertexData.push_back(-1.0f);	vertexData.push_back(0.0f);

	vertexData.push_back(-vs);	vertexData.push_back(vs);	vertexData.push_back(-vs);		vertexData.push_back(0.0f);	vertexData.push_back(1.0f);	vertexData.push_back(0.0f);
	vertexData.push_back(vs);	vertexData.push_back(vs);	vertexData.push_back(-vs);		vertexData.push_back(0.0f);	vertexData.push_back(1.0f);	vertexData.push_back(0.0f);
	vertexData.push_back(vs);	vertexData.push_back(vs);	vertexData.push_back(vs);		vertexData.push_back(0.0f);	vertexData.push_back(1.0f);	vertexData.push_back(0.0f);
	vertexData.push_back(vs);	vertexData.push_back(vs);	vertexData.push_back(vs);		vertexData.push_back(0.0f);	vertexData.push_back(1.0f);	vertexData.push_back(0.0f);
	vertexData.push_back(-vs);	vertexData.push_back(vs);	vertexData.push_back(vs);		vertexData.push_back(0.0f);	vertexData.push_back(1.0f);	vertexData.push_back(0.0f);
	vertexData.push_back(-vs);	vertexData.push_back(vs);	vertexData.push_back(-vs);		vertexData.push_back(0.0f);	vertexData.push_back(1.0f);	vertexData.push_back(0.0f);
}

void BulletShape::CreatePlane()
{
	float vs = 10.0f;
	vertexData.push_back(-vs);	vertexData.push_back(1);	vertexData.push_back(-vs);		vertexData.push_back(0.0f);	vertexData.push_back(1.0f);	vertexData.push_back(0.0f);
	vertexData.push_back(vs);	vertexData.push_back(1);	vertexData.push_back(-vs);		vertexData.push_back(0.0f);	vertexData.push_back(1.0f);	vertexData.push_back(0.0f);
	vertexData.push_back(vs);	vertexData.push_back(1);	vertexData.push_back(vs);		vertexData.push_back(0.0f);	vertexData.push_back(1.0f);	vertexData.push_back(0.0f);
	vertexData.push_back(vs);	vertexData.push_back(1);	vertexData.push_back(vs);		vertexData.push_back(0.0f);	vertexData.push_back(1.0f);	vertexData.push_back(0.0f);
	vertexData.push_back(-vs);	vertexData.push_back(1);	vertexData.push_back(vs);		vertexData.push_back(0.0f);	vertexData.push_back(1.0f);	vertexData.push_back(0.0f);
	vertexData.push_back(-vs);	vertexData.push_back(1);	vertexData.push_back(-vs);		vertexData.push_back(0.0f);	vertexData.push_back(1.0f);	vertexData.push_back(0.0f);
}