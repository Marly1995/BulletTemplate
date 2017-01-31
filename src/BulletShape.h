#pragma once
#include "..\includes.h"
#include "bullet\btBulletDynamicsCommon.h"
class BulletShape
{
public:
	btCollisionShape* shape;
	btDefaultMotionState* motionState;
	btScalar mass;
	btRigidBody* rigidBody;

	// opengl rendering data
	std::vector<GLfloat> vertexData;
	glm::mat4 GLmatrix;
public:
	BulletShape(btCollisionShape* newShape, btTransform newTransform, btScalar newMass);
	~BulletShape();

	void CreateVertexData();
	void CreateBox();
};