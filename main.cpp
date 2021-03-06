#include "includes.h"
#include "src/BulletWorld.h"
#include "src/BulletShape.h"
#include "bullet\btBulletDynamicsCommon.h"
#include <chrono>

// tag::globalVariables[]
// tag::using[]
using std::cout;
using std::cerr;
using std::endl;
using std::max;
using std::string;
// end::using[]

// tag::globalVariables[]
std::string exeName;
SDL_Window *win; //pointer to the SDL_Window
SDL_GLContext context; //the SDL_GLContext
int frameCount = 0;
std::string frameLine = "";
double frameTime = 0;
double lastTime = 0;
double currentTime = 0;
double deltaTime = 0.01;
double accumulator = 0;
double ttime = 0;
float testTime = 0;
int testCount = 0;

double physicsSpeed = 0.5;
// end::globalVariables[]
glm::mat4 modelMatrix;
glm::mat4 projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);

// object holders
std::vector <BulletShape*> shapes;
// Bullet vars
BulletWorld bWorld = BulletWorld();
BulletWorld activeScene = BulletWorld();
int magnetismMethod = 0;
// end Bullet vars

//our variables
bool done = false;
bool pause = false;

// tag::gameState[]
glm::vec3 lightColor = { 1.0f, 1.0f, 1.0f };
glm::vec3 lightPosition = { 2.0f, 2.0f, 1.0f };

GLfloat cameraSpeed = 0.05f;
glm::vec3 cameraPosition = glm::vec3(1.0f, 10.0f, -1.0f);
glm::vec3 cameraFront = glm::vec3(-0.5f, 0.5f, -.5f);
glm::vec3 cameraViewUp = glm::vec3(0.0f, 1.0f, 0.0f);

GLfloat pitch, yaw;
GLfloat savePitch, saveYaw;

bool cameraForward = false;
bool cameraBackward = false;
bool cameraLeft = false;
bool cameraRight = false;
bool cameraUp = false;
bool cameraDown = false;

int mouseX, mouseY, mouseLastX, mouseLastY;
GLfloat xOffset;
GLfloat yOffset;
// end::gameState[]

// tag::GLVariables[]
//our GL and GLSL variables
//programIDs
GLuint theProgram; //GLuint that we'll fill in to refer to the GLSL program (only have 1 at this point)

				   //attribute locations
GLint positionLocation; //GLuint that we'll fill in with the location of the `position` attribute in the GLSL
GLint vertexColorLocation; //GLuint that we'll fill in with the location of the `vertexColor` attribute in the GLSL
GLint colorLocation;
GLint normalLocation;
GLint lightColorLocation;
GLint lightPositionLocation;
GLint cameraPositionLocation;

						   //uniform location
GLint modelMatrixLocation;
GLint viewMatrixLocation;
GLint projectionMatrixLocation;

GLuint vertexDataBufferObject;
GLuint vertexArrayObject;
// end::GLVariables[]

void writetolog(std::string text)
{
	std::ofstream filelog("../logfile.txt", std::ios_base::out | std::ios_base::app);
	filelog << text << endl;
}
// end Global Variables
/////////////////////////

// possible redo shaders to include lighting
// tag::loadShader[]
std::string loadShader(const string filePath) {
	std::ifstream fileStream(filePath, std::ios::in | std::ios::binary);
	if (fileStream)
	{
		string fileData((std::istreambuf_iterator<char>(fileStream)),
			(std::istreambuf_iterator<char>()));

		cout << "Shader Loaded from " << filePath << endl;
		return fileData;
	}
	else
	{
		cerr << "Shader could not be loaded - cannot read file " << filePath << ". File does not exist." << endl;
		return "";
	}
}
// end::loadShader[]

// tag::initialise[]
void initialise()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		exit(1);
	}
	cout << "SDL initialised OK!\n";
}
// end::initialise[]

// tag::createWindow[]
void createWindow()
{
	//get executable name, and use as window title
	int beginIdxWindows = exeName.rfind("\\"); //find last occurrence of a backslash
	int beginIdxLinux = exeName.rfind("/"); //find last occurrence of a forward slash
	int beginIdx = max(beginIdxWindows, beginIdxLinux);
	std::string exeNameEnd = exeName.substr(beginIdx + 1);
	const char *exeNameCStr = exeNameEnd.c_str();

	//create window
	win = SDL_CreateWindow(exeNameCStr, 100, 100, 1000, 1000, SDL_WINDOW_OPENGL); //same height and width makes the window square ...

																				//error handling
	if (win == nullptr)
	{
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}
	cout << "SDL CreatedWindow OK!\n";
}
// end::createWindow[]

// tag::setGLAttributes[]
void setGLAttributes()
{
	int major = 3;
	int minor = 3;
	cout << "Built for OpenGL Version " << major << "." << minor << endl; //ahttps://en.wikipedia.org/wiki/OpenGL_Shading_Language#Versions
																		  // set the opengl context version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); //core profile
	cout << "Set OpenGL context to versicreate remote branchon " << major << "." << minor << " OK!\n";
}
// tag::setGLAttributes[]

// tag::createContext[]
void createContext()
{
	setGLAttributes();

	context = SDL_GL_CreateContext(win);
	if (context == nullptr) {
		SDL_DestroyWindow(win);
		std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}
	cout << "Created OpenGL context OK!\n";
}
// end::createContext[]

// tag::initGlew[]
void initGlew()
{
	GLenum rev;
	glewExperimental = GL_TRUE; //GLEW isn't perfect - see https://www.opengl.org/wiki/OpenGL_Loading_Library#GLEW
	rev = glewInit();
	if (GLEW_OK != rev) {
		std::cerr << "GLEW Error: " << glewGetErrorString(rev) << std::endl;
		SDL_Quit();
		exit(1);
	}
	else {
		cout << "GLEW Init OK!\n";
	}
}
// end::initGlew[]

// tag::createShader[]
GLuint createShader(GLenum eShaderType, const std::string &strShaderFile)
{
	GLuint shader = glCreateShader(eShaderType);
	//error check
	const char *strFileData = strShaderFile.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch (eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

	return shader;
}
// end::createShader[]

// tag::createProgram[]
GLuint createProgram(const std::vector<GLuint> &shaderList)
{
	GLuint program = glCreateProgram();

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glAttachShader(program, shaderList[iLoop]);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glDetachShader(program, shaderList[iLoop]);

	return program;
}
// end::createProgram[]

// tag::initializeProgram[]
void initializeProgram()
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(createShader(GL_VERTEX_SHADER, loadShader("../vertexShader.glsl")));
	shaderList.push_back(createShader(GL_FRAGMENT_SHADER, loadShader("../fragmentShader.glsl")));

	theProgram = createProgram(shaderList);
	if (theProgram == 0)
	{
		cerr << "GLSL program creation error." << std::endl;
		SDL_Quit();
		exit(1);
	}
	else {
		cout << "GLSL program creation OK! GLUint is: " << theProgram << std::endl;
	}

	// tag::glGetAttribLocation[]
	positionLocation = glGetAttribLocation(theProgram, "position");
	vertexColorLocation = glGetAttribLocation(theProgram, "vertexColor");
	normalLocation = glGetAttribLocation(theProgram, "normal");
	colorLocation = glGetUniformLocation(theProgram, "color");
	lightColorLocation = glGetUniformLocation(theProgram, "lightColor");
	lightPositionLocation = glGetUniformLocation(theProgram, "lightPos");
	cameraPositionLocation = glGetUniformLocation(theProgram, "cameraPos");
	// end::glGetAttribLocation[]

	// tag::glGetUniformLocation[]
	modelMatrixLocation = glGetUniformLocation(theProgram, "modelMatrix");
	viewMatrixLocation = glGetUniformLocation(theProgram, "viewMatrix");
	projectionMatrixLocation = glGetUniformLocation(theProgram, "projectionMatrix");

	//only generates runtime code in debug mode
	SDL_assert_release(modelMatrixLocation != -1);
	SDL_assert_release(viewMatrixLocation != -1);
	SDL_assert_release(projectionMatrixLocation != -1);
	// end::glGetUniformLocation[]

	//clean up shaders (we don't need them anymore as they are no in theProgram
	for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
}
// end::initializeProgram[]

// only for textured objects
void initializeVertexObjectColor(BulletShape* shape)
{
	glBindVertexArray(shape->arrayBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, shape->textureBuffer);
	glEnableVertexAttribArray(vertexColorLocation);
	glVertexAttribPointer(vertexColorLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);

	glDisableVertexAttribArray(vertexColorLocation);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// TODO: possible needs a dynamic runtime acces for better testing
// tag::initializeVertexArrayObject[]
//setup a GL object (a VertexArrayObject) that stores how to access data and from where
void initializeVertexArrayObject(BulletShape* shape)
{
	glGenVertexArrays(1, &shape->arrayBuffer); //create a Vertex Array Object
	// cout << "Vertex Array Object created OK! GLUint is: " << shape->arrayBuffer << std::endl;

	glBindVertexArray(shape->arrayBuffer); //make the just created vertexArrayObject the active one
	glBindBuffer(GL_ARRAY_BUFFER, shape->vertexBuffer); //bind vertexDataBufferObject
	glEnableVertexAttribArray(positionLocation); //enable attribute at index positionLocation
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(normalLocation); //enable attribute at index positionLocation
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0); //unbind the vertexArrayObject so we can't change it

	//cleanup
	glDisableVertexAttribArray(positionLocation); //disable vertex attribute at index positionLocation
	glDisableVertexAttribArray(normalLocation);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind array buffer

	initializeVertexObjectColor(shape);
}
// end::initializeVertexArrayObject[]

// tag::initializeVertexBuffer[]
void initializeVertexBuffer(BulletShape* shape)
{
	glGenBuffers(1, &shape->vertexBuffer);
	glGenBuffers(1, &shape->textureBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, shape->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, shape->vertexData.size() * sizeof(GLfloat), &shape->vertexData.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, shape->textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape->color), shape->color, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// cout << "vertexDataBufferObject created OK! GLUint is: " << shape->vertexBuffer << std::endl;

	initializeVertexArrayObject(shape);
}
// end::initializeVertexBuffer[]

// tag::loadAssets[]
void loadAssets()
{
	initializeProgram(); //create GLSL Shaders, link into a GLSL program, and get IDs of attributes and variables

	for (int i = 0; i < activeScene.shapes.size(); i++)
	{
		initializeVertexBuffer(activeScene.shapes[i]); //load data into a vertex buffer
	}

	cout << "Loaded Assets OK!\n";
}
// end::loadAssets[]

void initPhysics()
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
	bWorld.dynamicsWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f));
	for (int i = 0; i < shapes.size(); i++)
	{
		shapes[i]->shape->calculateLocalInertia(shapes[i]->mass, fall);
		shapes[i]->rigidBody->setFriction(2.0f);
		bWorld.dynamicsWorld->addRigidBody(shapes[i]->rigidBody);
	}	
	activeScene.SceneOne();
}

void cameraSave()
{
	if (!pause)
	{
		savePitch = pitch;
		saveYaw = yaw;
	}
	else
	{
		pitch = savePitch;
		yaw = saveYaw;
	}
}

// TODO: comtrols to move magnet for testing real time performance
void handleInput()
{
	SDL_Event event; //somewhere to store an event

	while (SDL_PollEvent(&event)) //loop until SDL_PollEvent returns 0 (meaning no more events)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			done = true;

			break;

			//keydown handling - we should to the opposite on key-up for direction controls (generally)
		case SDL_KEYDOWN:
			if (!event.key.repeat)
				switch (event.key.keysym.sym)
				{
					//hit escape to exit
				case SDLK_ESCAPE: done = true;
					break;
				case SDLK_RETURN: 
					cameraSave();
					pause = !pause;
					break;

				case SDLK_w: cameraForward = true;
					break;
				case SDLK_s: cameraBackward = true;
					break;
				case SDLK_a: cameraLeft = true;
					break;
				case SDLK_d: cameraRight = true;
					break;
				case SDLK_q: cameraUp = true;
					break;
				case SDLK_e: cameraDown = true;
					break;
				case SDLK_SPACE: 
					cameraPosition = glm::vec3(0.25f, 1.7f, 6.8f);
					yaw = 270;
					pitch = 4.85;
					break;
				case SDLK_z:
					cameraPosition = glm::vec3(0.64f, 14.3f, -1.0f);
					yaw = 90;
					pitch = 90;
					break;
				case SDLK_x:
					cameraPosition = glm::vec3(7.8f, 0.6f, 0.0f);
					yaw = 180;
					pitch = 2;
					break;

					// scene selection
				case SDLK_1: activeScene = 
					BulletWorld();
					cameraSave();
					pause = true;					
					activeScene.SceneOne();
					loadAssets();
					break;
				case SDLK_2: activeScene =
					BulletWorld();
					cameraSave();
					pause = true;
					activeScene.SceneTwo();
					loadAssets();
					break;
				case SDLK_3: activeScene =
					BulletWorld();
					cameraSave();
					pause = true;
					activeScene.SceneThree();
					loadAssets();
					break;
				case SDLK_4: activeScene =
					BulletWorld();
					cameraSave();
					pause = true;
					activeScene.SceneFour();
					loadAssets();
					break;
				case SDLK_5: activeScene =
					BulletWorld();
					cameraSave();
					pause = true;
					activeScene.SceneFive();
					loadAssets();
					break;
				case SDLK_6: activeScene =
					BulletWorld();
					cameraSave();
					pause = true;
					activeScene.SceneSix();
					loadAssets();
					break;

					// simulation metho selection
				case SDLK_v: magnetismMethod = 0;
					break;
				case SDLK_b: magnetismMethod = 1;
					break;
				case SDLK_n: magnetismMethod = 2;
					break;
				case SDLK_m: magnetismMethod = 3;
					break;

				case SDLK_r:
					cout << "timer result: " << (testTime / testCount) << endl;
					testTime = 0;
					testCount = 0;
					break;
				}
			break;

		case SDL_KEYUP:
			event.key.repeat = true;
			if (event.key.repeat)
				switch (event.key.keysym.sym)
				{
				case SDLK_w: cameraForward = false;
					break;
				case SDLK_s: cameraBackward = false;
					break;
				case SDLK_a: cameraLeft = false;
					break;
				case SDLK_d: cameraRight = false;
					break;
					case SDLK_q: cameraUp = false;
						break;
					case SDLK_e: cameraDown = false;
						break;
				}

		case SDL_MOUSEMOTION:
			break;
		}
	
	}
}

float magneticStrengthCalculation(float I, btVector3 magnet, btVector3 object)
{
	float R = magnet.distance(object);
	float H = 10.0f / (2 * glm::pi<float>() * R);
	//writetolog(std::to_string(H));
	return H;
}

float xfield(float x, float y, float z)
{
	return -(x / y)*glm::sin(x)*x*glm::sinh(y)*y*glm::sin(z)*z;
	//return -x - y;
}

float yfield(float x, float y, float z)
{
	return glm::cos(x)*x*glm::cosh(y)*y*glm::sin(z)*z;
	//return -y - x;
}

float zfield(float x, float y, float z)
{
	return (z/y)*glm::cos(x)*x*glm::sinh(y)*y*glm::cos(z)*z;
	//return -z + x - y;
}

float xcurl(float x)
{
	return cosf(2*glm::pi<float>()*x);
}

float ycurl(float x)
{
	return cosf(2 * glm::pi<float>()*x);
}

float zcurl(float x, float y)
{
	return x-y;
}

btVector3 CustomVectorField(btVector3 A)
{
	float x = xfield(A.x(), A.y(), A.z());
	float y = yfield(A.x(), A.y(), A.z());
	float z = zfield(A.x(), A.y(), A.z());

	return btVector3(x*A.x(), y*A.y(), z*A.z());
}

btVector3 PoleVectorField(btVector3 A)
{
	float x;
	float y;
	float z;

	float r = glm::sqrt(A.x()*A.x()*A.y()*A.y()*A.z()*A.z());
	float rz = glm::sqrt(A.x()*A.x()*A.y()*A.y());

	float costh = A.z() / r;
	float sinth = rz / r;
	double sinph = A.y() / rz;
	float cosph = A.x() / rz;

	float eth = 2 * sinth;

	x = sinth*cosph*costh + costh*cosph*eth;
	y = sinth*sinph*costh + costh*sinph*eth;
	z = costh * costh - sinth * eth;

	//std::cout << x << "   " << y << "   " << z << endl;
	return -btVector3(x*0.003f, y*0.003f, z*0.003f);
}

btVector3 magneticForce(btVector3 point, btVector3 magnet)
{
	btVector3 vec = point - magnet;
	float dist = vec.length();
	float str = 100.0f / dist*dist;
	vec = btVector3(vec.x()*str, vec.y()*str, vec.z()*str);
	return vec;
}

btVector3 fieldCalculation(btVector3 point, btVector3 pole1, btVector3 pole2)
{
	btVector3 v1 = magneticForce(point, pole1);
	btVector3 v2 = magneticForce(pole2, point);
	btVector3 v3 = v1 + v2;
	//std::cout << v3.x() << "   " << v3.y() << "   " << v3.z() << endl;
	v3.normalize();
	v3.operator*=(-point.length());
	return v3 + point;
}

btVector3 LorentzForce(btVector3 v, btVector3 b, btVector3 e)
{
	if (v.x() <= 1 && v.y() <= 1 && v.z() <= 1)
	{
		return 10.0f*e;
	}
	else
	{
		return 10.0f*(e + v.cross(b));
	}
}

btVector3 positionalDifference(btTransform shape, btTransform magnet)
{
	return (magnet.getOrigin() - shape.getOrigin());// .normalized();
}

btVector3 magnetPoint(int point, btVector3 position, float extent)
{
	switch(point)
	{
	case 0:
		position += btVector3(extent, extent, extent);
		break;
	case 1:
		position += btVector3(-extent, extent, extent);
		break;
	case 2:
		position += btVector3(extent, extent, -extent);
		break;
	case 3:
		position += btVector3(-extent, extent, -extent);
		break;
	case 4:
		position += btVector3(extent, -extent, extent);
		break;
	case 5:
		position += btVector3(-extent, -extent, extent);
		break;
	case 6:
		position += btVector3(extent, -extent, -extent);
		break;
	case 7:
		position += btVector3(-extent, -extent, -extent);
		break;

	}
	return position;
}

// simulation methods
void ParticleEstimatedField(double simTime)
{
	bool add = false;
	auto start_time = std::chrono::high_resolution_clock::now();
	activeScene.dynamicsWorld->stepSimulation(simTime * physicsSpeed, 1);
	for (int i = 0; i < activeScene.shapes.size(); i++)
	{
		btTransform shape;
		activeScene.shapes[i]->rigidBody->getMotionState()->getWorldTransform(shape);
		shape.getOpenGLMatrix(glm::value_ptr(activeScene.shapes[i]->GLmatrix));

		for (int k = 0; k < activeScene.shapes.size(); k++)
		{
			btTransform magnet;
			activeScene.shapes[k]->rigidBody->getMotionState()->getWorldTransform(magnet);
			if (k == i || !activeScene.shapes[k]->magnet || !activeScene.shapes[i]->metal) {}
			else if (shape.getOrigin().distance(magnet.getOrigin()) <= 10.0f)
			{
				activeScene.shapes[i]->rigidBody->applyCentralForce(
					positionalDifference(shape, magnet) *
						100.0f * magneticStrengthCalculation(activeScene.shapes[k]->charge, shape.getOrigin(), magnet.getOrigin()));					
				add = true;
			}
		}
	}
	auto end_time = std::chrono::high_resolution_clock::now();
	if (add)
	{
		testTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
		testCount++;
	}
}

void ParticleLorentzVectorField(double simTime)
{
	activeScene.dynamicsWorld->stepSimulation(simTime * physicsSpeed, 1);
	for (int i = 0; i < activeScene.shapes.size(); i++)
	{
		btTransform shape;
		activeScene.shapes[i]->rigidBody->getMotionState()->getWorldTransform(shape);
		shape.getOpenGLMatrix(glm::value_ptr(activeScene.shapes[i]->GLmatrix));

		for (int k = 0; k < activeScene.shapes.size(); k++)
		{
			btTransform magnet;
			activeScene.shapes[k]->rigidBody->getMotionState()->getWorldTransform(magnet);
			if (k == i || !activeScene.shapes[k]->magnet || !activeScene.shapes[i]->metal) {}
			else if (shape.getOrigin().distance(magnet.getOrigin()) <= 5.0f)
			{
				float str = magneticStrengthCalculation(activeScene.shapes[k]->charge, shape.getOrigin(), magnet.getOrigin());
				btVector3 lf = LorentzForce(activeScene.shapes[i]->rigidBody->getLinearVelocity(), CustomVectorField(-positionalDifference(shape, magnet).normalized()) * str, positionalDifference(shape, magnet) * 10.0f);
				//cout << lf.x() << "   " << lf.y() << "   " << lf.z() << endl;
				activeScene.shapes[i]->rigidBody->applyCentralForce(lf);
			}
		}
	}
}

void ParticleVectorField(double simTime)
{
	activeScene.dynamicsWorld->stepSimulation(simTime * physicsSpeed, 1);
	for (int i = 0; i < activeScene.shapes.size(); i++)
	{
		btTransform shape;
		activeScene.shapes[i]->rigidBody->getMotionState()->getWorldTransform(shape);
		shape.getOpenGLMatrix(glm::value_ptr(activeScene.shapes[i]->GLmatrix));

		for (int k = 0; k < activeScene.shapes.size(); k++)
		{
			btTransform magnet;
			activeScene.shapes[k]->rigidBody->getMotionState()->getWorldTransform(magnet);
			if (k == i || !activeScene.shapes[k]->magnet || !activeScene.shapes[i]->metal) {}
			else if (shape.getOrigin().distance(magnet.getOrigin()) <= 5.0f)
			{
				activeScene.shapes[i]->rigidBody->applyCentralForce(PoleVectorField(positionalDifference(shape, magnet)) * magneticStrengthCalculation(activeScene.shapes[k]->charge, shape.getOrigin(), magnet.getOrigin()));
			}
		}
	}
}

void CornerBasedPositionalDifference(double simTime)
{
	bool add = false;
	auto start_time = std::chrono::high_resolution_clock::now();
	activeScene.dynamicsWorld->stepSimulation(simTime * physicsSpeed, 1);
	for (int i = 0; i < activeScene.shapes.size(); i++)
	{
		btTransform shape;
		activeScene.shapes[i]->rigidBody->getMotionState()->getWorldTransform(shape);
		shape.getOpenGLMatrix(glm::value_ptr(activeScene.shapes[i]->GLmatrix));

		for (int k = 0; k < activeScene.shapes.size(); k++)
		{
			btTransform magnet;
			activeScene.shapes[k]->rigidBody->getMotionState()->getWorldTransform(magnet);
			if (k == i || !activeScene.shapes[k]->magnet || !activeScene.shapes[i]->metal) {}
			else
			{
				for (int p = 0; p < 8; p++)
				{
					activeScene.shapes[k]->rigidBody->getMotionState()->getWorldTransform(magnet);
					magnet.setOrigin(magnetPoint(p, magnet.getOrigin(), activeScene.shapes[k]->vertExtent));
					for (int q = 0; q < 8; q++)
					{
						activeScene.shapes[i]->rigidBody->getMotionState()->getWorldTransform(shape);
						shape.setOrigin(magnetPoint(q, shape.getOrigin(), activeScene.shapes[i]->vertExtent));
						if (shape.getOrigin().distance(magnet.getOrigin()) <= 20.0f && activeScene.shapes[k])
						{
							//activeScene.shapes[i]->rigidBody->applyForce(
								//positionalDifference(shape, magnet) * 
								//magneticStrengthCalculation(activeScene.shapes[k]->charge, shape.getOrigin(), magnet.getOrigin()), shape.getOrigin());
							activeScene.shapes[i]->rigidBody->applyForce(positionalDifference(shape, magnet), shape.getOrigin());
							add = true;
						}
					}
				}
			}
		}
	}
	auto end_time = std::chrono::high_resolution_clock::now();
	if (add)
	{
		testTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
		testCount++;
	}
}

void cameraSimulation(double simTime)
{
	cameraSpeed = 2.0f * simTime;
	if (cameraForward == true) {
		cameraPosition += cameraSpeed * cameraFront;
	}
	if (cameraBackward == true) {
		cameraPosition -= cameraSpeed * cameraFront;
	}
	if (cameraLeft == true) {
		cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraViewUp)) * cameraSpeed;
	}
	if (cameraRight == true) {
		cameraPosition += glm::normalize(glm::cross(cameraFront, cameraViewUp)) * cameraSpeed;
	}
	if (cameraUp == true) {
		cameraPosition[1] += cameraSpeed;
	}
	if (cameraDown == true) {
		cameraPosition[1] -= cameraSpeed;
	}

	SDL_GetMouseState(&mouseX, &mouseY);
	SDL_WarpMouseInWindow(win, 500, 500);
	xOffset = mouseX - 500;
	yOffset = mouseY - 500;
	mouseLastX = mouseX;
	mouseLastY = mouseY;

	GLfloat sensitivity = 0.05f;;
	xOffset *= sensitivity;
	yOffset *= sensitivity;


	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(-pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);
}

void mainSimulation(double simTime) //update simulation with an amount of time to simulate for (in seconds)
{
	cameraSimulation(simTime);
}

// tag::preRender[]
void preRender()
{
	glViewport(0, 0, 1000, 1000); //set viewpoint
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //set clear colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the window (technical the scissor box bounds)
}
// end::preRender[]

// tag::render[]
void render()
{
	glUseProgram(theProgram); //installs the program object specified by program as part of current rendering state
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glUniform3f(cameraPositionLocation, cameraPosition[0], cameraPosition[1], cameraPosition[2]);
	glUniform3f(lightColorLocation, lightColor[0], lightColor[1], lightColor[2]);
	glUniform3f(lightPositionLocation, lightPosition[0], lightPosition[1], lightPosition[2]);

	glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraViewUp);

	glUniformMatrix4fv(projectionMatrixLocation, 1, false, glm::value_ptr(projection));

	glUniformMatrix4fv(viewMatrixLocation, 1, false, glm::value_ptr(view));

	for (int i = 0; i < shapes.size(); i++)
	{
		glBindVertexArray(shapes[i]->arrayBuffer);

		glUniform4f(colorLocation, shapes[i]->color[0], shapes[i]->color[1], shapes[i]->color[2], shapes[i]->color[3]);

		//set modelMatrix and draw for triangle 1
		modelMatrix = shapes[i]->GLmatrix;
		glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(modelMatrix));
		glDrawArrays(GL_TRIANGLES, 0, shapes[i]->vertexData.size() / 6);

		glBindVertexArray(0);
	}

	glUseProgram(0); //clean up
}

void scenerender()
{
	glUseProgram(theProgram); //installs the program object specified by program as part of current rendering state
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glUniform3f(cameraPositionLocation, cameraPosition[0], cameraPosition[1], cameraPosition[2]);
	glUniform3f(lightColorLocation, lightColor[0], lightColor[1], lightColor[2]);
	glUniform3f(lightPositionLocation, lightPosition[0], lightPosition[1], lightPosition[2]);

	glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraViewUp);

	glUniformMatrix4fv(projectionMatrixLocation, 1, false, glm::value_ptr(projection));

	glUniformMatrix4fv(viewMatrixLocation, 1, false, glm::value_ptr(view));

	for (int i = 0; i < activeScene.shapes.size(); i++)
	{
		glBindVertexArray(activeScene.shapes[i]->arrayBuffer);

		glUniform4f(colorLocation, activeScene.shapes[i]->color[0], activeScene.shapes[i]->color[1], activeScene.shapes[i]->color[2], activeScene.shapes[i]->color[3]);

		//set modelMatrix and draw for triangle 1
		modelMatrix = activeScene.shapes[i]->GLmatrix;
		glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(modelMatrix));
		glDrawArrays(GL_TRIANGLES, 0, activeScene.shapes[i]->vertexData.size() / 6);

		glBindVertexArray(0);
	}

	glUseProgram(0); //clean up
}
// end::render[]

// tag::postRender[]
void postRender()
{
	SDL_GL_SwapWindow(win);; //present the frame buffer to the display (swapBuffers)
	/*frameLine += "Frame: " + std::to_string(frameCount++);
	cout << "\r" << frameLine << std::flush;
	frameLine = "";*/
}
// end::postRender[]

// tag::cleanUp[]
void cleanUp()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	cout << "Cleaning up OK!\n";
}
// end::cleanUp[]

// tag::main[]
int main(int argc, char* args[])
{
	exeName = args[0];
	//setup
	initialise();

	createWindow();

	createContext();

	initGlew();

	glViewport(0, 0, 1000, 1000); //should check what the actual window res is?

	initPhysics();

	loadAssets();

	

	while (!done) //loop until done flag is set)
	{
		handleInput(); // this should ONLY SET VARIABLES
		if (pause)
		{
			SDL_CaptureMouse(SDL_FALSE);
			SDL_ShowCursor(SDL_ENABLE);
		}
		if (!pause)
		{
			SDL_CaptureMouse(SDL_TRUE);
			SDL_ShowCursor(SDL_DISABLE);
			lastTime = currentTime;
			currentTime = SDL_GetTicks();
			frameTime = (currentTime - lastTime) /1000;
			if (frameTime > 0.25)
				frameTime = 0.25;

			accumulator += frameTime;
		
			while (accumulator >= deltaTime)
			{
				accumulator -= deltaTime;
				ttime += deltaTime;

				switch (magnetismMethod)
				{
				case 0:
					ParticleEstimatedField(deltaTime);
					break;
				case 1:
					ParticleLorentzVectorField(deltaTime);
					break;
				case 2:
					ParticleVectorField(deltaTime);
					break;
				case 3:
					CornerBasedPositionalDifference(deltaTime);
					break;
				}

				mainSimulation(deltaTime);
			}
		}
		//btTransform trans;
		//activeScene.shapes[0]->rigidBody->getMotionState()->getWorldTransform(trans);
		//string pos = trans
		//writetolog()
		preRender();

		//render(); // this should render the world state according to VARIABLES -
		scenerender();
		postRender();

	}

	//cleanup and exit
	cleanUp();
	SDL_Quit();

	return 0;
}
// end::main[]