#include "BulletWorld.h"



BulletWorld::BulletWorld()
{
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	broadphase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	shapes.clear();
}

// TODO: fix deletion of objects here
BulletWorld::~BulletWorld()
{
	//for (int i = dynamicsWorld->getNumCollisionObjects() -1; i >= 0; i--)
	//{
	//	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
	//	btRigidBody* body = btRigidBody::upcast(obj);
	//	if (body && body->getMotionState())
	//	{
	//		delete body->getMotionState();
	//	}
	//	dynamicsWorld->removeCollisionObject(obj);
	//	delete obj;
	//}

	//for (int i = 0; i < shapes.size(); i++)
	//{
	//	//dynamicsWorld->removeRigidBody(shapes[i]->rigidBody);
	//	btCollisionShape* shape = shapes[i]->shape;
	//	shapes[i]->shape = 0;
	//	delete shape;
	//}

	/*for (int i = 0; i < shapes.size(); i++)
	{
		dynamicsWorld->removeRigidBody(shapes[i]->rigidBody);
		delete shapes[i]->rigidBody->getMotionState();
		delete shapes[i]->rigidBody->getCollisionShape();
		delete shapes[i]->rigidBody;
	}

	delete dynamicsWorld;
	delete solver;
	delete broadphase;
	delete dispatcher;
	delete collisionConfiguration;	*/
}

void BulletWorld::SceneOne()
{
	btVector3 fall(0, 0, 0);

	btCollisionShape* btcube = new btBoxShape(btVector3(0.2, 0.2, 0.2));
	BulletShape* cube = new BulletShape(btcube, btTransform(btQuaternion(0, 0, 0, 1), btVector3(1, 7, 0)), btScalar(10), 0.2f, shapes.size(), true, true);
	shapes.push_back(cube);

	BulletShape* magnet = new BulletShape(btcube, btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 2, 0)), btScalar(50), 0.2f, shapes.size(), true, true);
	shapes.push_back(magnet);

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
