#pragma once
#include "bullet\btBulletDynamicsCommon.h"
#include "BulletShape.h"
class BulletWorld
{
public:
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	std::vector <BulletShape*> shapes;
public:
	BulletWorld();
	~BulletWorld();

	void SceneOne();
};

