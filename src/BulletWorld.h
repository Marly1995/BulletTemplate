#pragma once
#include "bullet\btBulletDynamicsCommon.h"
#include "BulletShape.h"
class BulletWorld
{
public:
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* broadphase;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	std::vector <BulletShape*> shapes;
public:
	BulletWorld();
	~BulletWorld();

	void SceneOne();
	void SceneTwo();
	void SceneThree();
	void SceneFour();
	void SceneFive();
};

