#pragma once
#include "..\includes.h"
#include "bullet\btBulletDynamicsCommon.h"
class BulletShape
{
public:
	// bullet physics data
	btCollisionShape* shape;
	btDefaultMotionState* motionState;
	btScalar mass;
	btRigidBody* rigidBody;

	// opengl rendering data
	GLuint vertexBuffer;
	GLuint arrayBuffer;
	GLuint textureBuffer;
	GLfloat color[4];
	std::vector<GLfloat> vertexData;
	glm::mat4 GLmatrix;

	// magnet sim data
	bool magnet;
	bool metal;
	float vertExtent;
public:
	BulletShape(btCollisionShape* newShape, btTransform newTransform, btScalar newMass, float vertScale, int buffer, bool isMagnet, bool isMetal);
	~BulletShape();

	void CreateVertexData(float vs);
	void PickColor(int col);
	void CreateBox(float vs);
	void CreatePlane();
};