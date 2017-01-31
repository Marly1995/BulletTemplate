#include "BulletShape.h"



BulletShape::BulletShape(btCollisionShape* newShape, btTransform newTransform, btScalar newMass, float vertScale)
{
	shape = newShape;
	motionState = new btDefaultMotionState(newTransform);
	mass = newMass;
	rigidBody = new btRigidBody(mass, motionState, shape, btVector3(0, 0, 0));
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
}

void BulletShape::CreateBox(float vs)
{
	//vertexData.push_back()
}