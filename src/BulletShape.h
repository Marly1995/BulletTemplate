#pragma once
#include "bullet\btBulletDynamicsCommon.h"
class BulletShape
{
public:
	btCollisionShape* shape;
	btDefaultMotionState* motionState;
	btScalar mass;
	btRigidBody* rigidBody;
public:
	BulletShape(btCollisionShape* newShape, btTransform newTransform, btScalar newMass);
	~BulletShape();
};

