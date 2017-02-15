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
	CreateVertexData(vertScale);
}


BulletShape::~BulletShape()
{
	delete rigidBody->getMotionState();
	delete rigidBody;
	delete shape;
}

void BulletShape::CreateVertexData(float vs)
{
	std::string name = shape->getName();
	if (name == "Box")
	{
		CreateBox(vs);
	}
	else if (name == "STATICPLANE")
	{
		CreatePlane();
	}
}

void BulletShape::CreateBox(float vs)
{
	vertexData.push_back(-vs);	vertexData.push_back(-vs);	vertexData.push_back(-vs);
	vertexData.push_back(vs);	vertexData.push_back(-vs);	vertexData.push_back(-vs);
	vertexData.push_back(vs);	vertexData.push_back(vs);	vertexData.push_back(-vs);
	vertexData.push_back(vs);	vertexData.push_back(vs);	vertexData.push_back(-vs);
	vertexData.push_back(-vs);	vertexData.push_back(vs);	vertexData.push_back(-vs);
	vertexData.push_back(-vs);	vertexData.push_back(-vs);	vertexData.push_back(-vs);

	vertexData.push_back(-vs);	vertexData.push_back(-vs);	vertexData.push_back(vs);
	vertexData.push_back(vs);	vertexData.push_back(-vs);	vertexData.push_back(vs);
	vertexData.push_back(vs);	vertexData.push_back(vs);	vertexData.push_back(vs);
	vertexData.push_back(vs);	vertexData.push_back(vs);	vertexData.push_back(vs);
	vertexData.push_back(-vs);	vertexData.push_back(vs);	vertexData.push_back(vs);
	vertexData.push_back(-vs);	vertexData.push_back(-vs);	vertexData.push_back(vs);

	vertexData.push_back(-vs);	vertexData.push_back(vs);	vertexData.push_back(vs);
	vertexData.push_back(-vs);	vertexData.push_back(vs);	vertexData.push_back(-vs);
	vertexData.push_back(-vs);	vertexData.push_back(-vs);	vertexData.push_back(-vs);
	vertexData.push_back(-vs);	vertexData.push_back(-vs);	vertexData.push_back(-vs);
	vertexData.push_back(-vs);	vertexData.push_back(-vs);	vertexData.push_back(vs);
	vertexData.push_back(-vs);	vertexData.push_back(vs);	vertexData.push_back(vs);

	vertexData.push_back(vs);	vertexData.push_back(vs);	vertexData.push_back(vs);
	vertexData.push_back(vs);	vertexData.push_back(vs);	vertexData.push_back(-vs);
	vertexData.push_back(vs);	vertexData.push_back(-vs);	vertexData.push_back(-vs);
	vertexData.push_back(vs);	vertexData.push_back(-vs);	vertexData.push_back(-vs);
	vertexData.push_back(vs);	vertexData.push_back(-vs);	vertexData.push_back(vs);
	vertexData.push_back(vs);	vertexData.push_back(vs);	vertexData.push_back(vs);

	vertexData.push_back(-vs);	vertexData.push_back(-vs);	vertexData.push_back(-vs);
	vertexData.push_back(vs);	vertexData.push_back(-vs);	vertexData.push_back(-vs);
	vertexData.push_back(vs);	vertexData.push_back(-vs);	vertexData.push_back(vs);
	vertexData.push_back(vs);	vertexData.push_back(-vs);	vertexData.push_back(vs);
	vertexData.push_back(-vs);	vertexData.push_back(-vs);	vertexData.push_back(vs);
	vertexData.push_back(-vs);	vertexData.push_back(-vs);	vertexData.push_back(-vs);

	vertexData.push_back(-vs);	vertexData.push_back(vs);	vertexData.push_back(-vs);
	vertexData.push_back(vs);	vertexData.push_back(vs);	vertexData.push_back(-vs);
	vertexData.push_back(vs);	vertexData.push_back(vs);	vertexData.push_back(vs);
	vertexData.push_back(vs);	vertexData.push_back(vs);	vertexData.push_back(vs);
	vertexData.push_back(-vs);	vertexData.push_back(vs);	vertexData.push_back(vs);
	vertexData.push_back(-vs);	vertexData.push_back(vs);	vertexData.push_back(-vs);
}

void BulletShape::CreatePlane()
{
	float vs = 0.0f;
	vertexData.push_back(-vs);	vertexData.push_back(vs);	vertexData.push_back(-vs);
	vertexData.push_back(vs);	vertexData.push_back(vs);	vertexData.push_back(-vs);
	vertexData.push_back(vs);	vertexData.push_back(vs);	vertexData.push_back(vs);
	vertexData.push_back(vs);	vertexData.push_back(vs);	vertexData.push_back(vs);
	vertexData.push_back(-vs);	vertexData.push_back(vs);	vertexData.push_back(vs);
	vertexData.push_back(-vs);	vertexData.push_back(vs);	vertexData.push_back(-vs);
}