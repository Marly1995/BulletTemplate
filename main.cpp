#include "includes.h"
#include "src/BulletWorld.h"
#include "src/BulletShape.h"
#include "bullet\btBulletDynamicsCommon.h"

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
// end::globalVariables[]
glm::mat4 modelMatrix;

// Bullet vars
BulletWorld bWorld = BulletWorld();
btCollisionShape* btcube = new btBoxShape(btVector3(0.1, 0.1, 0.1));
BulletShape cube = BulletShape(btcube, btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 25, 0)), btScalar(0.5), 0.1f);
btCollisionShape* btplane = new btStaticPlaneShape(btVector3(0, 1, 0), 0.1);
BulletShape place = BulletShape(btplane, btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)), btScalar(0), 1.0f);
// end Bullet vars

//our variables
bool done = false;

// tag::vertexData[]
//the data about our geometry
const GLfloat vertexData[] = {
	//	 X        Y            Z          R     G     B      A
	-0.1f, -0.1f, -0.1f,
	0.1f, -0.1f, -0.1f,
	0.1f,  0.1f, -0.1f,
	0.1f,  0.1f, -0.1f,
	-0.1f,  0.1f, -0.1f,
	-0.1f, -0.1f, -0.1f,

	-0.1f, -0.1f,  0.1f,
	0.1f, -0.1f,  0.1f,
	0.1f,  0.1f,  0.1f,
	0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f, -0.1f,  0.1f,

	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	-0.1f, -0.1f, -0.1f,
	-0.1f, -0.1f, -0.1f,
	-0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,

	0.1f,  0.1f,  0.1f,
	0.1f,  0.1f, -0.1f,
	0.1f, -0.1f, -0.1f,
	0.1f, -0.1f, -0.1f,
	0.1f, -0.1f,  0.1f,
	0.1f,  0.1f,  0.1f,

	-0.1f, -0.1f, -0.1f,
	0.1f, -0.1f, -0.1f,
	0.1f, -0.1f,  0.1f,
	0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,

	-0.1f,  0.1f, -0.1f,
	0.1f,  0.1f, -0.1f,
	0.1f,  0.1f,  0.1f,
	0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f
};
// end::vertexData[]

// tag::gameState[]
//the translation vector we'll pass to our GLSL program
glm::vec3 position1 = { 0.0f, 0.0f, 0.0f };
glm::vec3 velocity1 = { 0.1f, 0.1f, 0.0f };

glm::vec3 position2 = { 0.0f, 0.0f , 0.0f };
glm::vec3 velocity2 = { -0.2f, 0.15f, 0.0f };

GLfloat cameraSpeed = 0.05f;
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, -15.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool cameraForward = false;
bool cameraBackward = false;
bool cameraLeft = false;
bool cameraRight = false;
bool cameraRotUp = false;
bool cameraRotDown = false;
// end::gameState[]

// tag::GLVariables[]
//our GL and GLSL variables
//programIDs
GLuint theProgram; //GLuint that we'll fill in to refer to the GLSL program (only have 1 at this point)

				   //attribute locations
GLint positionLocation; //GLuint that we'll fill in with the location of the `position` attribute in the GLSL
GLint vertexColorLocation; //GLuint that we'll fill in with the location of the `vertexColor` attribute in the GLSL

						   //uniform location
GLint modelMatrixLocation;
GLint viewMatrixLocation;
GLint projectionMatrixLocation;

GLuint vertexDataBufferObject;
GLuint vertexArrayObject;
// end::GLVariables[]

// end Global Variables
/////////////////////////

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

	shaderList.push_back(createShader(GL_VERTEX_SHADER, loadShader("C:/Users/Computing/Documents/GitHub/BulletTemplate/vertexShader.glsl")));
	shaderList.push_back(createShader(GL_FRAGMENT_SHADER, loadShader("C:/Users/Computing/Documents/GitHub/BulletTemplate/fragmentShader.glsl")));

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

// tag::initializeVertexArrayObject[]
//setup a GL object (a VertexArrayObject) that stores how to access data and from where
void initializeVertexArrayObject()
{
	glGenVertexArrays(1, &vertexArrayObject); //create a Vertex Array Object
	cout << "Vertex Array Object created OK! GLUint is: " << vertexArrayObject << std::endl;

	glBindVertexArray(vertexArrayObject); //make the just created vertexArrayObject the active one

	glBindBuffer(GL_ARRAY_BUFFER, vertexDataBufferObject); //bind vertexDataBufferObject

	glEnableVertexAttribArray(positionLocation); //enable attribute at index positionLocation
	//glEnableVertexAttribArray(vertexColorLocation); //enable attribute at index vertexColorLocation

													// tag::glVertexAttribPointer[]
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); //specify that position data contains four floats per vertex, and goes into attribute index positionLocation
//	glVertexAttribPointer(vertexColorLocation, 4, GL_FLOAT, GL_FALSE, (7 * sizeof(GL_FLOAT)), (GLvoid *)(3 * sizeof(GLfloat))); //specify that position data contains four floats per vertex, and goes into attribute index vertexColorLocation
																																// end::glVertexAttribPointer[]

	glBindVertexArray(0); //unbind the vertexArrayObject so we can't change it

						  //cleanup
	glDisableVertexAttribArray(positionLocation); //disable vertex attribute at index positionLocation
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind array buffer

}
// end::initializeVertexArrayObject[]

// tag::initializeVertexBuffer[]
void initializeVertexBuffer()
{
	glGenBuffers(1, &vertexDataBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, vertexDataBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	cout << "vertexDataBufferObject created OK! GLUint is: " << vertexDataBufferObject << std::endl;

	initializeVertexArrayObject();
}
// end::initializeVertexBuffer[]

// tag::loadAssets[]
void loadAssets()
{
	initializeProgram(); //create GLSL Shaders, link into a GLSL program, and get IDs of attributes and variables

	initializeVertexBuffer(); //load data into a vertex buffer

	cout << "Loaded Assets OK!\n";
}
// end::loadAssets[]

// tag::handleInput[]
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

				case SDLK_UP: cameraForward = true;
					break;
				case SDLK_DOWN: cameraBackward = true;
					break;
				case SDLK_LEFT: cameraLeft = true;
					break;
				case SDLK_RIGHT: cameraRight = true;
					break;
				case SDLK_a: cameraRotUp = true;
					break;
				case SDLK_d: cameraRotDown = true;
					break;
				case SDLK_SPACE: cameraPosition = glm::vec3(0.0f, 0.0f, 2.0f);
					cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
					cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
					break;
				}
			break;

		case SDL_KEYUP:
			event.key.repeat = true;
			if (event.key.repeat)
				switch (event.key.keysym.sym)
				{
				case SDLK_UP: cameraForward = false;
					break;
				case SDLK_DOWN: cameraBackward = false;
					break;
				case SDLK_LEFT: cameraLeft = false;
					break;
				case SDLK_RIGHT: cameraRight = false;
					break;
				case SDLK_a: cameraRotUp = false;
					break;
				case SDLK_d: cameraRotDown = false;
					break;
				}
		}
	}
}
// end::handleInput[]

// tag::updateSimulation[]
void updateSimulation(double simLength = 0.02) //update simulation with an amount of time to simulate for (in seconds)
{
	bWorld.dynamicsWorld->stepSimulation(1 / 60.f, 1);

	btTransform sphereTrans;
	btTransform planeTrans;
	float mat[16];
	cube.rigidBody->getMotionState()->getWorldTransform(sphereTrans);
	sphereTrans.getOpenGLMatrix(glm::value_ptr(cube.GLmatrix));
	//position1 = glm::vec3(sphereTrans.getOrigin().getX(), sphereTrans.getOrigin().getY(), sphereTrans.getOrigin().getZ());
	std::cout << "sphere height: " << sphereTrans.getOrigin().getY() << std::endl;
	place.rigidBody->getMotionState()->getWorldTransform(planeTrans);
	planeTrans.getOpenGLMatrix(glm::value_ptr(place.GLmatrix));
	//position2 = glm::vec3(planeTrans.getOrigin().getX(), planeTrans.getOrigin().getY(), planeTrans.getOrigin().getZ());
	

	if (cameraForward == true) {
		cameraPosition -= cameraSpeed * glm::vec3(0.0f, 0.0f, -1.0f);
	}
	if (cameraBackward == true) {
		cameraPosition += cameraSpeed * glm::vec3(0.0f, 0.0f, -1.0f);
	}
	if (cameraLeft == true) {
		cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (cameraRight == true) {
		cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (cameraRotUp == true) {
		cameraPosition += glm::normalize(glm::cross(cameraFront, glm::vec3(1.0f, 0.0f, 0.0f))) * cameraSpeed;
	}
	if (cameraRotDown == true) {
		cameraPosition -= glm::normalize(glm::cross(cameraFront, glm::vec3(1.0f, 0.0f, 0.0f))) * cameraSpeed;
	}
}
// end::updateSimulation[]

// tag::preRender[]
void preRender()
{
	glViewport(0, 0, 1000, 1000); //set viewpoint
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f); //set clear colour
	glClear(GL_COLOR_BUFFER_BIT); //clear the window (technical the scissor box bounds)
}
// end::preRender[]

// tag::render[]
void render()
{
	glUseProgram(theProgram); //installs the program object specified by program as part of current rendering state

	glBindVertexArray(vertexArrayObject);

	glm::mat4 view = glm::lookAt(cameraPosition, cameraFront, cameraUp);

	glm::mat4 projection;
	projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
	//set projectionMatrix - how we go from 3D to 2D
	glUniformMatrix4fv(projectionMatrixLocation, 1, false, glm::value_ptr(projection));

	//set viewMatrix - how we control the view (viewpoint, view direction, etc)
	glUniformMatrix4fv(viewMatrixLocation, 1, false, glm::value_ptr(view));


	//set modelMatrix and draw for triangle 1
	modelMatrix = cube.GLmatrix;
	glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(modelMatrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//set modelMatrix and draw for triangle 2
	modelMatrix = place.GLmatrix;
	glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(modelMatrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);

	glUseProgram(0); //clean up
}
// end::render[]

// tag::postRender[]
void postRender()
{
	SDL_GL_SwapWindow(win);; //present the frame buffer to the display (swapBuffers)
	frameLine += "Frame: " + std::to_string(frameCount++);
	cout << "\r" << frameLine << std::flush;
	frameLine = "";
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

	loadAssets();

	// bullet sim
	bWorld.dynamicsWorld->addRigidBody(place.rigidBody);
	btVector3 fall(0, 0, 0);
	cube.shape->calculateLocalInertia(cube.mass, fall);
	bWorld.dynamicsWorld->addRigidBody(cube.rigidBody);
	//end bullet sim

	while (!done) //loop until done flag is set)
	{
		handleInput(); // this should ONLY SET VARIABLES

		updateSimulation(); // this should ONLY SET VARIABLES according to simulation

		preRender();

		render(); // this should render the world state according to VARIABLES -

		postRender();

	}

	//cleanup and exit
	cleanUp();
	SDL_Quit();

	return 0;
}
// end::main[]