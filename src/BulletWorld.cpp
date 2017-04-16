#include "BulletWorld.h"



BulletWorld::BulletWorld()
{
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
}

// TODO: fix deletion of objects here
BulletWorld::~BulletWorld()
{
	delete dynamicsWorld;
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;
}

void BulletWorld::SceneOne()
{
	btVector3 fall(0, 0, 0);

	btCollisionShape* btcube = new btBoxShape(btVector3(0.1, 0.1, 0.1));
	BulletShape* cube = new BulletShape(btcube, btTransform(btQuaternion(0, 0, 0, 1), btVector3(-2, 5, 1)), btScalar(1), 0.1f, shapes.size(), false, true);
	shapes.push_back(cube);

	BulletShape* cube1 = new BulletShape(btcube, btTransform(btQuaternion(0, 0, 0, 1), btVector3(-2.5, 5, 0)), btScalar(1), 0.1f, shapes.size(), false, true);
	//shapes.push_back(cube1);
	/*
	BulletShape* cube2 = new BulletShape(btcube, btTransform(btQuaternion(0, 0, 0, 1), btVector3(2.5, 30, 2.5)), btScalar(1), 0.1f, shapes.size(), false, true);
	shapes.push_back(cube2);*/

	/*for (int i = 0; i < 10; i++)
	{
	BulletShape* cube2 = new BulletShape(btcube, btTransform(btQuaternion(0, 0, 0, 1), btVector3(rand() % 8 - 4, 201.0f, rand() % 8 - 4)), btScalar(1), 0.1f, shapes.size(), false, true);
	shapes.push_back(cube2);
	}*/

	btCollisionShape* btcubestatic = new btBoxShape(btVector3(0.2, 0.2, 0.2));
	BulletShape* magnet = new BulletShape(btcubestatic, btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)), btScalar(0), 0.2f, shapes.size(), true, true);
	shapes.push_back(magnet);

	BulletShape* magnet1 = new BulletShape(btcubestatic, btTransform(btQuaternion(0, 0, 0, 1), btVector3(-2, 1, 0)), btScalar(50), 0.2f, shapes.size(), true, true);
	//shapes.push_back(magnet1);

	btCollisionShape* btPlane = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	BulletShape* plane = new BulletShape(btPlane, btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)), btScalar(0), 1.0f, shapes.size(), false, false);
	shapes.push_back(plane);

	// define gravity
	dynamicsWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f));
	for (int i = 0; i < shapes.size(); i++)
	{
		shapes[i]->shape->calculateLocalInertia(shapes[i]->mass, fall);
		shapes[i]->rigidBody->setFriction(2.0f);
		dynamicsWorld->addRigidBody(shapes[i]->rigidBody);
	}
}
