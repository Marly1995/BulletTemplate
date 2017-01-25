#pragma once
#include "bullet\btBulletDynamicsCommon.h"
class BulletWorld
{
public:
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
public:
	BulletWorld();
	~BulletWorld();
};

