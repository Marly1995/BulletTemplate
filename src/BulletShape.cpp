#include "BulletShape.h"



BulletShape::BulletShape(btCollisionShape* newShape, btTransform newTransform, btScalar newMass)
{
	shape = newShape;
	motionState = new btDefaultMotionState(newTransform);
	mass = newMass;
	rigidBody = new btRigidBody(mass, motionState, shape, btVector3(0, 0, 0));
}


BulletShape::~BulletShape()
{
	delete rigidBody->getMotionState();
	delete rigidBody;
	delete shape;
}
