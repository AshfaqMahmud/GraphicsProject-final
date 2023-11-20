#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "curve.h"
#include "cube.h"
#include "frustum.h"
#include "hexagon.h"
#include "wing.h"
#include "trapezoid.h"
#include "pentagon.h"
#include "stb_image.h"
#include "sphere.h"

#include <iostream>

using namespace std;

//function declaration
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);
void drawPlane(Shader lightingShaderWithTexture, Wing& wing, BezierCurve& plane_back, BezierCurve& plane_mid, BezierCurve& plane_front, BezierCurve& plane_front_face, glm::mat4 model);
void drawseat(Cube &seat,Cube& back, Cube& baseSeat, Shader lightingShaderWithTexture, glm::mat4 tranMat);
void player(Cube cubeFor_head, Shader lightingShaderWithTexture, glm::mat4 model, glm::mat4 combineMat);
void drawAirport(Pentagon& pent, Trapezoid& trapezoid, Cube& cube, Cube& wall, Shader lightingShaderWithTexture,glm::mat4 modelforairport);
void drawHeli(BezierCurve& dndi, BezierCurve& bj, Cube& cube, Shader lShader,glm::mat4 tran);
void drawCtrlTower(BezierCurve& curve, Frustum& frstm, Sphere& globe, Hexagon& hex, Shader lshader, Shader lshader2, glm::mat4 model);
void passGate(Cube& wall, Shader lshdader, glm::mat4 modelmat);
void drawTreeWithFractiles(Cube& cube, Shader& lightingShader, glm::mat4 alTogether, float L, float H, float W, int N);
void drawCounter(Cube& table, Cube& wall, Shader ls, glm::mat4 model);

//void basement(glm::mat4 model = glm::mat4(1.0f));

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);

// settings
const unsigned int SCR_WIDTH = 1300;
const unsigned int SCR_HEIGHT = 700;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
bool rot = false;
float r = 0.0;

//textures
static string dmapMath[] = { "skybox.jpg","MOON.png","marbel.png","rece.jpg","plane_texture.png","plane2.png","vending.jpg","whiteBackground.png","sofa.png","wood.png","wall.png","globe.jpg","brick.jpg","runway.jpg","floor.png","tran.png","lounge.jpg","cv.png"};
static string smapMath[] = { "skybox.jpg","MOON.png","marbel.png","rece.jpg","plane_texture.png","plane2.png","vending.jpg","whiteBackground.png","sofa.png","wood.png","wall.png","globe.jpg","brick.jpg","runway.jpg","floor.png","tran.png","lounge.jpg","cv.png"};

static string dAir[] = { "airport.jpg","etihad.png","emirates.png","flybiman.png","door.png","usbangla.png","kuet.png","helipad.png"};
static string sAir[] = { "airport.jpg","etihad.png","emirates.png","flybiman.png","door.png","usbangla.png","kuet.png","helipad.png" };
// camera
Camera camera(glm::vec3(15.0f, 5.0f, 30.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


float eyeX = 10.0, eyeY = -10.0, eyeZ = -3.0;
float lookAtX = 10.0, lookAtY = -10.0, lookAtZ = -3.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);



// lights
// positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.5f,  15.0f,  25.0f),
	glm::vec3(30.5f,  15.0f,  25.0f),
	glm::vec3(30.5f,  15.0f,  5.0f),
	glm::vec3(0.5f,  15.0f,  5.0f),
	glm::vec3(-10.5f,  15.5f,  5.0f),
	glm::vec3(10.5f,  15.5f,  5.0f)
};
PointLight pointlight1(

	pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
	0.05f, 0.05f, 0.05f,     // ambient
	1.8f, 1.8f, 1.8f,     // diffuse
	1.0f, 1.0f, 1.0f,        // specular
	0.05f,   //k_c
	0.09f,  //k_l
	0.0032f, //k_q
	1       // light number
);
PointLight pointlight2(

	pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
	0.05f, 0.05f, 0.05f,     // ambient
	1.8f, 1.8f, 1.8f,     // diffuse
	1.0f, 1.0f, 1.0f,        // specular
	0.05f,   //k_c
	0.09f,  //k_l
	0.0032f, //k_q
	2       // light number
);
PointLight pointlight3(

	pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
	0.05f, 0.05f, 0.05f,     // ambient
	1.8f, 1.8f, 1.8f,     // diffuse
	1.0f, 1.0f, 1.0f,        // specular
	0.05f,   //k_c
	0.09f,  //k_l
	0.0032f, //k_q
	3       // light number
);
PointLight pointlight4(

	pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
	0.05f, 0.05f, 0.05f,     // ambient
	1.8f, 1.8f, 1.8f,     // diffuse
	1.0f, 1.0f, 1.0f,        // specular
	0.05f,   //k_c
	0.09f,  //k_l
	0.0032f, //k_q
	4       // light number
);



// light settings
bool dirLightOn = true;
bool pointLightOn = true;
bool spotLightOn = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

float curve_Cpoints_back[] = {
			-0.2800, 2.4932, 5.1000,
			-0.4050, 2.3803, 5.1000,
			-0.5050, 2.2551, 5.1000,
			-0.6200, 2.0934, 5.1000,
			-0.7350, 1.9225, 5.1000,
			-0.8200, 1.7089, 5.1000,
			-0.8150, 1.5196, 5.1000,
			-0.8300, 1.3609, 5.1000,
			-0.8550, 1.1778, 5.1000,
			-0.8500, 0.9672, 5.1000,
			-0.8750, 0.6926, 5.1000,
			-0.8050, 0.5430, 5.1000,
			-0.6550, 0.3325, 5.1000,
			-0.5750, 0.1799, 5.1000,
			-0.4500, -0.0094, 5.1000
};
float curve_Cpoints_mid[] = {
			-0.7400, 2.5400, 5.1000,
			-0.8450, 2.1700, 5.1000,
			-0.8450, 1.8600, 5.1000,
			-0.8500, 1.5550, 5.1000,
			-0.8950, 1.1800, 5.1000,
			-0.8550, 0.8400, 5.1000,
			-0.8000, 0.3500, 5.1000,
			-0.7350, 0.0800, 5.1000,
};
float curve_Cpoints_front[] = {

			-0.5450, 2.1600, 5.1000,
			-0.7500, 1.8550, 5.1000,
			-0.8850, 1.4000, 5.1000,
			-0.9550, 0.8900, 5.1000,
			-0.9700, 0.4500, 5.1000,
			-0.9550, 0.0550, 5.1000,
			-0.2900, 2.4050, 5.1000
};

float test[] = {
			-0.9000, 2.4900, 5.1000,
			-0.9800, 2.2550, 5.1000,
			-1.0200, 2.0350, 5.1000,
			-1.0400, 1.8750, 5.1000,
			-1.0450, 1.6100, 5.1000,
			-1.0550, 1.4250, 5.1000,
			-1.0450, 1.2450, 5.1000,
			-1.0650, 1.0600, 5.1000,
			-1.0650, 0.9200, 5.1000,
			-1.0650, 0.7250, 5.1000,
			-1.0250, 0.5750, 5.1000,
			-0.9900, 0.4000, 5.1000,
			-0.9550, 0.2150, 5.1000,
			-0.9000, 0.0750, 5.1000,
			-0.8100, -0.0500, 5.1000,
			-0.6650, -0.1500, 5.1000,
};
float test2[] = {
			
			-0.0350, 2.3200, 5.1000,
			-0.5550, 2.1950, 5.1000,
			-0.7200, 2.0900, 5.1000,
			-0.8600, 1.9200, 5.1000,
			-1.0050, 1.7700, 5.1000,
			-1.1400, 1.5700, 5.1000,
			-1.2450, 1.3600, 5.1000,
			-1.2950, 1.1600, 5.1000,
			-1.3400, 0.9200, 5.1000,
			-1.3750, 0.6200, 5.1000,
			-1.4050, 0.3550, 5.1000,
			-1.4250, 0.1400, 5.1000,
};
float front_face[] = {
			 0.0010, 2.3300, 5.1000,
			-0.5600, 2.2750, 5.1000,
			-0.8250, 2.1750, 5.1000,
			-0.9950, 2.0350, 5.1000,
			-1.1000, 1.8450, 5.1000,
			-1.1900, 1.7050, 5.1000,
			-1.2750, 1.4650, 5.1000,
			-1.3400, 1.2550, 5.1000,
			-1.3800, 1.0750, 5.1000,
			-1.4450, 0.9250, 5.1000,
			-1.5200, 0.6600, 5.1000,
			-1.5550, 0.3850, 5.1000,
			-1.5800, 0.1850, 5.1000,
};
float dandi[] = {
-0.6150, 2.3950, 5.1000,
-0.7200, 1.8800, 5.1000,
-0.7350, 1.3200, 5.1000,
-0.6950, 0.7900, 5.1000,
-0.5700, 0.4800, 5.1000,
};


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
		// ------------------------------------
	Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
	Shader ourShader("vertexShader.vs", "fragmentShader.fs");

	unsigned int cvnd = loadTexture(dmapMath[17].c_str(), GL_LINEAR, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int recep = loadTexture(dmapMath[3].c_str(), GL_LINEAR, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int dvnd = loadTexture(dmapMath[6].c_str(), GL_LINEAR, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int dwall = loadTexture(dmapMath[10].c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int swall = loadTexture(smapMath[10].c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int diffMap = loadTexture(dmapMath[0].c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int specMap = loadTexture(smapMath[0].c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int dsofa = loadTexture(dmapMath[8].c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int ssofa = loadTexture(smapMath[8].c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int dsofabase = loadTexture(dmapMath[9].c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int ssofabase = loadTexture(smapMath[9].c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int dtable = loadTexture(dmapMath[2].c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int stable = loadTexture(smapMath[2].c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int dplane = loadTexture(dmapMath[4].c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int splane = loadTexture(smapMath[4].c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int dglb = loadTexture(dmapMath[11].c_str(), GL_LINEAR, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int sglb = loadTexture(smapMath[11].c_str(), GL_LINEAR, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int dhx = loadTexture(dmapMath[12].c_str(), GL_LINEAR, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int shx = loadTexture(smapMath[12].c_str(), GL_LINEAR, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int drw = loadTexture(dmapMath[13].c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int srw = loadTexture(smapMath[13].c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int hrw = loadTexture(dmapMath[5].c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int dfloor = loadTexture(smapMath[14].c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int sfloor = loadTexture(smapMath[14].c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int dtran = loadTexture(smapMath[16].c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int stran = loadTexture(smapMath[16].c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);


	Trapezoid trapezoid = Trapezoid(dfloor, sfloor, 32.0f, 0.0f, 0.0f, 4.0f, 4.0f);
	Trapezoid trapeTable = Trapezoid(dtable, stable, 32.0f, 0.0f, 0.0f, 4.0f, 4.0f);
	Cube floor = Cube(dfloor, sfloor, 32.0f, 0.0f, 0.0f, 5.0f, 2.0f);
	Cube rece = Cube(recep, stran, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	Cube cv = Cube(cvnd, cvnd, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	Cube vend = Cube(dvnd, dvnd, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	Cube cube = Cube(hrw, srw, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	Cube cube1 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	Cube runway = Cube(drw, srw, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	Cube cube_player = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	Cube wall = Cube(dplane, shx, 32.0f, 0.0f, 0.0f, 3.0f, 3.0f);
	Cube twall = Cube(dtran, stran, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	Cube seat = Cube(dsofa, ssofa, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	Cube back = Cube(dsofa, ssofa, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	Cube baseSeat = Cube(dsofabase, ssofabase, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	Cube table = Cube(dtable, stable, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	Pentagon pent = Pentagon(dfloor, dfloor, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	Wing wing = Wing(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	Cube cubeetihad = Cube(loadTexture(dAir[1].c_str(), GL_LINEAR, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(sAir[1].c_str(), GL_LINEAR, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 32.0f, 0.0, 0.0, 1.0, 1.0);
	Cube airP = Cube(loadTexture(dAir[0].c_str(), GL_LINEAR, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(sAir[0].c_str(), GL_LINEAR, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 32.0f, 0.0, 0.0, 1.0, 1.0);
	Cube cubeemirates = Cube(loadTexture(dAir[2].c_str(), GL_LINEAR, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(sAir[2].c_str(), GL_LINEAR, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 32.0f, 0.0, 0.0, 1.0, 1.0);
	Cube cubefly = Cube(loadTexture(dAir[3].c_str(), GL_LINEAR, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(sAir[3].c_str(), GL_LINEAR, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 32.0f, 0.0, 0.0, 1.0, 1.0);
	Cube door = Cube(loadTexture(dAir[4].c_str(), GL_LINEAR, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(sAir[4].c_str(), GL_LINEAR, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 32.0f, 0.0, 0.0, 1.0, 1.0);
	Cube usbangla = Cube(loadTexture(dAir[5].c_str(), GL_LINEAR, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(sAir[5].c_str(), GL_LINEAR, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 32.0f, 0.0, 0.0, 1.0, 1.0);
	Cube kuet = Cube(loadTexture(dAir[6].c_str(), GL_LINEAR, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(sAir[6].c_str(), GL_LINEAR, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 32.0f, 0.0, 0.0, 1.0, 1.0);
	Cube helipad = Cube(loadTexture(dAir[7].c_str(), GL_LINEAR, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(sAir[7].c_str(), GL_LINEAR, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 32.0f, 0.0, 0.0, 1.0, 1.0);


	//BezierCurve plane_test = BezierCurve(curve_Cpoints_back, 15, diffMap, specMap);
	/*BezierCurve plane_back = BezierCurve(curve_Cpoints_back, 15,dplane,splane);
	BezierCurve plane_mid = BezierCurve(curve_Cpoints_mid, 8, dplane, splane);
	BezierCurve plane_front = BezierCurve(test, 15, dplane, splane);
	BezierCurve plane_front_face = BezierCurve(front_face, 12, dplane, splane);*/
	Sphere sphere = Sphere(diffMap, specMap, 0.0f, 0.0f, 4.0f, 4.0f);
	Sphere glb = Sphere(dglb, sglb, 0.0f, 0.0f, 1.0f, 1.0f);
	/*BezierCurve bj = BezierCurve(test2,12);
	BezierCurve dndi = BezierCurve(dandi, 8);*/
	BezierCurve plane_test = BezierCurve(curve_Cpoints_back, 15, dglb);
	BezierCurve plane_back = BezierCurve(curve_Cpoints_back, 15, dplane);
	BezierCurve plane_mid = BezierCurve(curve_Cpoints_mid, 8, dglb);
	BezierCurve plane_front = BezierCurve(test, 15, dplane);
	BezierCurve plane_front_face = BezierCurve(front_face, 12, dplane);
	BezierCurve bj = BezierCurve(test2, 12, dplane);
	BezierCurve dndi = BezierCurve(dandi, 8, dplane);

	Frustum ftest = Frustum(dhx, shx, 32.0f, 0.0f, 0.0f, 3.0f, 3.0f);
	Hexagon hex = Hexagon(dhx, shx, 32.0f, 0.0f, 0.0f, 3.0f, 3.0f);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// 
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
	glDrawElements(GL_TRIANGLES, 5, GL_UNSIGNED_INT, 0);
	int i = 0;
	int j;

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		glGetError();

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShaderWithTexture.use();
		lightingShaderWithTexture.setVec3("viewPos", camera.Position);

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		//glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
		lightingShaderWithTexture.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		//glm::mat4 view = basic_camera.createViewMatrix();
		lightingShaderWithTexture.setMat4("view", view);


		// Modelling Transformation
		glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
		glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
		translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
		rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
		rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
		rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
		scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
		model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix* scaleMatrix; 
		
		ourShader.setMat4("model", model);
		
		lightingShaderWithTexture.use();

		// point light 1
		pointlight1.setUpPointLight(lightingShaderWithTexture);
		// point light 2
		pointlight2.setUpPointLight(lightingShaderWithTexture);
		// point light 3
		pointlight3.setUpPointLight(lightingShaderWithTexture);
		// point light 4
		pointlight4.setUpPointLight(lightingShaderWithTexture);

		//trape.drawTrapezoid(lightingShaderWithTexture, model, 1.0f, 1.0f, 1.0f);
		//plane_test.drawBezierCurvewithTex(lightingShaderWithTexture,identityMatrix, glm::vec3(0.9098039215686274, 0.8, 1.0f));
		//drawPlane(lightingShaderWithTexture ,wing, plane_back, plane_mid, plane_front, plane_front_face, model);
		//DrawCylinder(0.5f, 2.0f, lightingShaderWithTexture);
		//plane_test.drawBezierCurve(lightingShaderWithTexture, identityMatrix);

		modelMatrixForContainer = glm::translate(model, glm::vec3(-0.45f, -0.4f, -2.8f));
		rotateAngle_X = 0;
		rotateAngle_Y = 0;
		rotateAngle_Z = 0;
		modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-20.0f, 0.0f, -8.5f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(identityMatrix, glm::vec3(35.0, 10, 0.01));
		runway.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f))* glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f))* glm::scale(identityMatrix, glm::vec3(30.0, 0.1, 30.0)));
		runway.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(identityMatrix, glm::vec3(30.0, 0.1, 30.0)));
		runway.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(identityMatrix, glm::vec3(30.0, 0.1, 30.0)));
		runway.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(identityMatrix, glm::vec3(30.0, 0.1, 30.0)));
		runway.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(30.0, 0.0, 0.0)) * glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(identityMatrix, glm::vec3(30.0, 0.1, 30.0)));
		runway.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(30.0, 0.0, 0.0)) * glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(identityMatrix, glm::vec3(30.0, 0.1, 30.0)));
		runway.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(30.0, 0.0, 30.0)) * glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(identityMatrix, glm::vec3(30.0, 0.1, 30.0)));
		runway.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 30.0)) * glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(identityMatrix, glm::vec3(30.0, 0.1, 30.0)));

		//ftest.drawFrustumWithTexture(lightingShaderWithTexture, identityMatrix);
		
		//string diffuseMapPath = "road_texture.png";
		//string specularMapPath = "road_texture.png";

		//unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
		//unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
		//Cube cube1 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
		//modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-20.0f, 0.0f, -8.5f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(identityMatrix, glm::vec3(35.0, 0.01, 15.0));
		//cube1.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);
		//modelMatrixForContainer = glm::translate(model, glm::vec3(0.0f, 7.0f, -4.0f)) * glm::scale(identityMatrix, glm::vec3(0.5, 0.5, 0.5));
		sphere.drawSphereWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(20.0f, 3.0f, 10.5f)) * glm::scale(identityMatrix, glm::vec3(70.0f, 70.0f, 70.0f))*glm::rotate(identityMatrix, glm::radians(i*0.05f), glm::vec3(0.0f, 1.0f, 0.0f)));
		i++;
	
		sphere.drawSphereWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(20.0f, 0.0f, 10.5f))* glm::scale(identityMatrix, glm::vec3(60.0f, 60.0f, 60.0f)));

		//drawSeat(VAO, lightingShaderWithTexture,cube, identityMatrix);
		//player(cube_player,lightingShaderWithTexture,identityMatrix,model);

		drawPlane(lightingShaderWithTexture, wing, plane_back, plane_mid, plane_front, plane_front_face, glm::translate(identityMatrix, glm::vec3(25.0f - (0.01 * i), -7.0f + (0.01 * i), 12.0f - 0.01 * i)) * glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(identityMatrix, glm::vec3(0.8, 1.2, 0.8)));

		
		
		drawCtrlTower(bj,ftest,glb,hex, lightingShaderWithTexture,ourShader, glm::translate(identityMatrix, glm::vec3(7.0f, 0.0f, 5.5f)) * glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(identityMatrix, glm::vec3(1.5, 1.5, 1.5)));
		//draw Airport
		drawAirport(pent,trapezoid,floor,wall,lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(20.0f, 0.0f, 10.5f)) * glm::scale(identityMatrix, glm::vec3(2.5, 2.5, 2.5)));
		//drawing sofas of lounge of first floor
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(11.0f, 0.3f, 6.0f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(11.0f, 0.3f, 7.0f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.1f, 0.3f, 6.0f)) * glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.1f, 0.3f, 7.0f)) * glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(11.0f, 0.3f, 8.0f)) * glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.1f, 0.3f, 8.0f)) * glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(11.0f, 0.3f, 9.0f)) * glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.1f, 0.3f, 9.0f)) * glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(10.0f, 0.3f, 9.0f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(8.9f, 0.3f, 9.0f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.1f, 0.3f, 10.0f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(10.0f, 0.3f, 10.0f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(8.9f, 0.3f, 10.0f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(11.0f, 0.3f, 10.0f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

		twall.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(7.5f, 0.0f, 11.0f))* glm::scale(identityMatrix, glm::vec3(5.0, 2.0, 0.1)));
		rece.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(19.9f, 0.8f, 13.3f))* glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f))* glm::scale(identityMatrix, glm::vec3(2.25, 1.0, 0.05)));
		vend.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(7.5f, 2.6f, 15.0f))* glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f))* glm::scale(identityMatrix, glm::vec3(0.5, 0.67, 0.2)));
		vend.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(7.5f, 2.6f, 14.3f))* glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f))* glm::scale(identityMatrix, glm::vec3(0.5, 0.67, 0.2)));
		cv.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(7.5f, 2.7f, 13.5f))* glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f))* glm::scale(identityMatrix, glm::vec3(0.4, 0.8, 0.2)));
		
		//drawing sofas of lounge of second floor
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(11.0f, 2.8f, 6.0f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(11.0f, 2.8f, 7.0f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.1f, 2.8f, 6.0f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.1f, 2.8f, 7.0f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(11.0f, 2.8f, 8.0f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.1f, 2.8f, 8.0f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(11.0f, 2.8f, 9.0f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.1f, 2.8f, 9.0f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(10.0f, 2.8f, 9.0f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(8.9f, 2.8f, 9.0f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.1f, 2.8f, 10.0f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(10.0f, 2.8f, 10.0f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(8.9f, 2.8f, 10.0f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawseat(seat, back, baseSeat, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(11.0f, 2.8f, 10.0f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		//drawing reception table
		
		//drawPlane(lightingShaderWithTexture, wing, plane_back, plane_mid, plane_front, plane_front_face, glm::translate(identityMatrix, glm::vec3(25.0f - (0.01 * i), -7.0f + (0.01 * i), 12.0f -0.01*i)) * glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(identityMatrix, glm::vec3(0.8, 0.8, 0.8)));
		
		table.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(19.5f, 0.0f, 11.6f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f))* glm::scale(identityMatrix, glm::vec3(1.2, 0.8f, 0.35f)));
		trapeTable.drawTrapezoidWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(19.15f, 0.0f, 12.8f))* glm::rotate(identityMatrix, glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f))* glm::scale(identityMatrix, glm::vec3(-0.5, 0.8f, 0.2f)));
		trapeTable.drawTrapezoidWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(19.15f, 0.0f, 11.6f))* glm::rotate(identityMatrix, glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f))* glm::scale(identityMatrix, glm::vec3(0.5, 0.8f, 0.2f)));
		
		passGate(wall, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.1f, 0.0f, 11.2f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f))* glm::scale(identityMatrix, glm::vec3(0.2, 0.4, 0.5)));
		passGate(wall, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.1f, 0.0f, 11.7f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f))* glm::scale(identityMatrix, glm::vec3(0.2, 0.4, 0.5)));
		passGate(wall, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.1f, 0.0f, 12.2f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f))* glm::scale(identityMatrix, glm::vec3(0.2, 0.4, 0.5)));
		passGate(wall, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.1f, 0.0f, 12.7f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f))* glm::scale(identityMatrix, glm::vec3(0.2, 0.4, 0.5)));
		passGate(wall, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.1f, 0.0f, 13.2f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f))* glm::scale(identityMatrix, glm::vec3(0.2, 0.4, 0.5)));
		passGate(wall, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.1f, 0.0f, 13.7f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f))* glm::scale(identityMatrix, glm::vec3(0.2, 0.4, 0.5)));
		passGate(wall, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.1f, 0.0f, 14.2f))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f))* glm::scale(identityMatrix, glm::vec3(0.2, 0.4, 0.5)));

		wall.drawCubeWithMaterialisticProperty(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(8.0f, 0.5f, 14.6f))* glm::scale(identityMatrix, glm::vec3(4.0, 0.3, 0.02)));
		wall.drawCubeWithMaterialisticProperty(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(8.0f, 0.5f, 14.1f))* glm::scale(identityMatrix, glm::vec3(4.0, 0.3, 0.02)));
		wall.drawCubeWithMaterialisticProperty(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(8.0f, 0.5f, 13.6f))* glm::scale(identityMatrix, glm::vec3(4.0, 0.3, 0.02)));
		wall.drawCubeWithMaterialisticProperty(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(8.0f, 0.5f, 13.1f))* glm::scale(identityMatrix, glm::vec3(4.0, 0.3, 0.02)));
		wall.drawCubeWithMaterialisticProperty(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(8.0f, 0.5f, 12.6f))* glm::scale(identityMatrix, glm::vec3(4.0, 0.3, 0.02)));
		wall.drawCubeWithMaterialisticProperty(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(8.0f, 0.5f, 12.1f))* glm::scale(identityMatrix, glm::vec3(4.0, 0.3, 0.02)));
		wall.drawCubeWithMaterialisticProperty(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(8.0f, 0.5f, 11.6f))* glm::scale(identityMatrix, glm::vec3(4.0, 0.3, 0.02)));

		wall.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(11.3f, 0.0f, 0.9f))* glm::scale(identityMatrix, glm::vec3(5.1, 5.2f, 0.1f)));
		wall.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(15.0f, 0.0f, 3.6f))* glm::rotate(identityMatrix, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f))* glm::scale(identityMatrix, glm::vec3(6.3, 5.2f, 0.1f)));
		wall.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(16.0f, 0.0f, 1.1f))* glm::rotate(identityMatrix, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f))* glm::scale(identityMatrix, glm::vec3(6.3, 5.2f, 0.1f)));
		wall.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.23f, 0.0f, 3.53f))* glm::rotate(identityMatrix, glm::radians(110.0f), glm::vec3(0.0f, 1.0f, 0.0f))* glm::scale(identityMatrix, glm::vec3(2.8, 5.2f, 0.1f)));
		hex.drawHexagonWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.4f, 0.0f, 3.53f))* glm::scale(identityMatrix, glm::vec3(0.2, 5.37f, 0.2f)));
		wall.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(10.0f, 0.0f, 5.9f))* glm::scale(identityMatrix, glm::vec3(2.8, 5.2f, 0.1f)));
		wall.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(14.8f, 0.0f, 5.9f))* glm::scale(identityMatrix, glm::vec3(2.8, 5.2f, 0.1f)));
		wall.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(9.95f, 0.0f, 8.0f))* glm::rotate(identityMatrix, glm::radians(208.0f), glm::vec3(0.0f, 1.0f, 0.0f))* glm::scale(identityMatrix, glm::vec3(2.8, 5.2f, 0.1f)));
		wall.drawCube2(ourShader, glm::translate(identityMatrix, glm::vec3(17.5f, 0.0f, 8.0f))* glm::rotate(identityMatrix, glm::radians(-25.0f), glm::vec3(0.0f, 1.0f, 0.0f))* glm::scale(identityMatrix, glm::vec3(2.8, 5.2f, 0.1f)));


		//drawTreeWithFractiles(cube, ourShader, glm::translate(identityMatrix, glm::vec3(10.5f, 0.0f, 18.0f)), 0,0,0,0);
		//window
		wall.drawCube2(ourShader, glm::translate(identityMatrix, glm::vec3(7.5f, 0.0f, 15.2f))* glm::scale(identityMatrix, glm::vec3(12.5, 5.2, 0.02)));

		wall.drawCubeWithMaterialisticProperty(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(7.5f, 2.2f, 15.1f))* glm::scale(identityMatrix, glm::vec3(12.5, 0.8, 0.3)));

		airP.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(11.5f, 3.0f, 15.0f))* glm::scale(identityMatrix, glm::vec3(4.0, 2.25, 0.7)));
		door.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.0f, 0.0f, 14.9f))* glm::scale(identityMatrix, glm::vec3(3.0, 2.25, 0.6)));
		//wall.dra
		drawCounter(table, wall, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(19.0f, 2.7f, 10.0f)));
		drawCounter(table, wall, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(19.0f, 2.7f, 12.0f)));
		drawCounter(table, wall, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(18.5f, 2.7f, 14.0))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawCounter(table, wall, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(16.5f, 2.7f, 14.0))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawCounter(table, wall, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(14.5f, 2.7f, 14.0))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		drawCounter(table, wall, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.5f, 2.7f, 14.0))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

		cubeetihad.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(18.9f, 3.0f, 10.2f))* glm::scale(identityMatrix, glm::vec3(0.05, 0.5, 1.0)));
		cubeemirates.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(18.9f, 3.0f, 12.2f))* glm::scale(identityMatrix, glm::vec3(0.05, 0.5, 1.0)));
		cubefly.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(18.3f, 3.0f, 13.9))*glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f))* glm::scale(identityMatrix, glm::vec3(0.05, 0.5, 1.0)));
		usbangla.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(16.3f, 3.0f, 13.9))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f))* glm::scale(identityMatrix, glm::vec3(0.05, 0.5, 1.0)));
		kuet.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(14.3f, 3.0f, 13.9))* glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f))* glm::scale(identityMatrix, glm::vec3(0.05, 0.5, 1.0)));
		plane_front_face.drawBezierCurve(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.4f, 16.5f, 3.53f))* glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f))*glm::scale(identityMatrix, glm::vec3(3.0, 2.5, 3.0)));
		helipad.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.0f, 5.1f, 12.0f))* glm::scale(identityMatrix, glm::vec3(3.0, 0.3, 3.0)));
		//drawHeli(dndi, bj, cube, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.0f, 5.6f, 12.4f)));

		if (rot)
		{
			float temp = 15.6f;
			j = 1.0;
			if (0.01*i < 10)
			{
				drawHeli(dndi, bj, cube, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.0f, 5.6f + 0.01 * i, 12.4f)));
				//temp = 5.6 + 0.01 * i;
				
			}
			else
			{
				drawHeli(dndi, bj, cube, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(21.5f-0.01*i, temp, 12.4f)));
				j++;
			}
			drawHeli(dndi, bj, cube, lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(20.0f-0.01*i, 10.0f, 15.5f)));
			r = r + 7.0;
		}
		

		// also draw the lamp object(s)
		ourShader.use();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		//we now draw as many light bulbs as we have point lights.
		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			cube.drawCube(ourShader, model, 0.8f, 0.8f, 0.8f); //these are for light

		}
		/*cout << "value of i " << i << endl;
		if (i == 600)
		{
			pointLightOn = false;
			if(i==2000000000)
			break;
		}*/

		// render boxes

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, 0.1);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, 0.1);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, 0.1);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, 0.1);
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		if (rotateAxis_X) rotateAngle_X -= 0.1f;
		else if (rotateAxis_Y) rotateAngle_Y -= 0.1f;
		else rotateAngle_Z -= 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
	{
		camera.MoveToPosition(glm::vec3(15.0f, 20.0f, 30.0f));
		camera.MoveLookAtPosition(glm::vec3(5.0f, -10.0f, -15.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
	{
		camera.MoveToPosition(glm::vec3(16.0f, 3.8f, 8.0f));
		camera.MoveLookAtPosition(glm::vec3(5.0f, 2.0f, 19.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
	{
		camera.MoveToPosition(glm::vec3(16.0f, 1.8f, 8.0f));
		camera.MoveLookAtPosition(glm::vec3(5.0f, 2.0f, 19.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
	{
		camera.MoveToPosition(glm::vec3(15.0f, 5.0f, 30.0f));
		camera.MoveLookAtPosition(glm::vec3(15.0f, 3.0f, -3.0f));
	}

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.001f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.001f;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.001f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.001f;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.001f;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.001f;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.001f;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.001f;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.001f;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.001f;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.001f;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.001f;

	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		rotateAngle_X += 0.1f;
		rotateAxis_X = 1.0f;
		rotateAxis_Y = 0.0f;
		rotateAxis_Z = 0.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
	{
		rotateAngle_Y += 0.1f;
		rotateAxis_X = 0.0f;
		rotateAxis_Y = 1.0f;
		rotateAxis_Z = 0.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		rotateAngle_Z += 0.1f;
		rotateAxis_X = 0.0f;
		rotateAxis_Y = 0.0f;
		rotateAxis_Z = 1.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
	{
		eyeX += 2.5 * deltaTime;
		basic_camera.changeEye(eyeX, eyeY, eyeZ);
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		eyeX -= 2.5 * deltaTime;
		basic_camera.changeEye(eyeX, eyeY, eyeZ);
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
	{
		eyeZ += 2.5 * deltaTime;
		basic_camera.changeEye(eyeX, eyeY, eyeZ);
	}
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
	{
		eyeZ -= 2.5 * deltaTime;
		basic_camera.changeEye(eyeX, eyeY, eyeZ);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		eyeY += 2.5 * deltaTime;
		basic_camera.changeEye(eyeX, eyeY, eyeZ);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		eyeY -= 2.5 * deltaTime;
		basic_camera.changeEye(eyeX, eyeY, eyeZ);
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
	{
		rot = !rot;
	}

}

void drawPlane(Shader lightingShaderWithTexture, Wing &wing, BezierCurve &plane_back, BezierCurve &plane_mid, BezierCurve &plane_front, BezierCurve &plane_front_face, glm::mat4 model)
{
	glm::mat4 identityMatrix = glm::mat4(1.0f); 
	glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
	glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix;
	translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
	rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
	rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
	scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
	glm::mat4 modelforwingl = model * glm::translate(model, glm::vec3(0.7f, 1.8f, -0.3f)) * glm::scale(identityMatrix, glm::vec3(0.5, -0.35, 0.2));
	wing.drawWingWithMaterialisticProperty(lightingShaderWithTexture, modelforwingl);

	glm::mat4 modelforwingr = model * glm::translate(model, glm::vec3(-0.7f, 1.8f, 0.3f)) * glm::scale(identityMatrix, glm::vec3(-0.5, -0.35, -0.2));
	wing.drawWingWithMaterialisticProperty(lightingShaderWithTexture, modelforwingr);

	glm::mat4 modelforwingt = model * glm::translate(model, glm::vec3(0.25f, 4.8f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(identityMatrix, glm::vec3(0.25, -0.2, 0.2));
	wing.drawWingWithMaterialisticProperty(lightingShaderWithTexture, modelforwingt);

	glm::mat4 modelforwingbl = model * glm::translate(model, glm::vec3(0.0f, 5.5f, -0.2f)) * glm::scale(identityMatrix, glm::vec3(0.2, -0.15, 0.15));
	wing.drawWingWithMaterialisticProperty(lightingShaderWithTexture, modelforwingbl);

	glm::mat4 modelforwingbr = model * glm::translate(model, glm::vec3(-0.2f, 5.5f, 0.2f)) * glm::scale(identityMatrix, glm::vec3(-0.2, -0.15, -0.15));
	wing.drawWingWithMaterialisticProperty(lightingShaderWithTexture, modelforwingbr);

	glm::mat4 modelforback = model * glm::translate(model, glm::vec3(0.0f, -0.25f, 0.0f)) * glm::scale(identityMatrix, glm::vec3(1.0, 2.5, 1.0));
	plane_back.drawBezierCurve(lightingShaderWithTexture, modelforback);

	glm::mat4 modelformid = model * glm::translate(model, glm::vec3(0.0f, -13.2f, 0.0f)) * glm::scale(identityMatrix, glm::vec3(1.0, 7.0, 1.0));
	plane_mid.drawBezierCurve(lightingShaderWithTexture, modelformid);

	glm::mat4 modelforfront = model * glm::translate(model, glm::vec3(0.0f, 5.3f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(identityMatrix, glm::vec3(0.81, -2.0, 0.81));
	plane_front.drawBezierCurve(lightingShaderWithTexture, modelforfront);

	glm::mat4 modelforface = model * glm::translate(model, glm::vec3(0.0f, 21.0f, 0.0f)) * glm::scale(identityMatrix, glm::vec3(0.78, -10.0, 0.78));
	plane_front_face.drawBezierCurve(lightingShaderWithTexture, modelforface);

}

void passGate(Cube& wall, Shader lshdader, glm::mat4 modelmat)
{
	glm::mat4 identityMatrix = glm::mat4(1.0f);
	glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
	glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix;
	translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
	rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
	rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
	scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
	wall.drawCubeWithTexture(lshdader, modelmat*glm::scale(identityMatrix, glm::vec3(1.0f, 2.0f, 0.3f)) * glm::translate(identityMatrix, glm::vec3(0.0f, 1.5f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(identityMatrix, glm::vec3(2.0, 2.5f, 0.05f)));
	//wall.drawCubeWithTexture(lightingShaderWithTexture, glm::scale(identityMatrix, glm::vec3(1.0f, 2.0f, 0.3f))* glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 2.5f)) * glm::scale(identityMatrix, glm::vec3(2.0, 1.5f, 0.05f)));
	wall.drawCubeWithTexture(lshdader, modelmat*glm::scale(identityMatrix, glm::vec3(1.0f, 2.0f, 0.3f)) * glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(identityMatrix, glm::vec3(2.55, 1.5f, 0.05f)));
	//wall.drawCubeWithTexture(lightingShaderWithTexture, glm::scale(identityMatrix, glm::vec3(1.0f, 2.0f, 0.3f))* glm::translate(identityMatrix, glm::vec3(1.0f, 1.5f, 0.0f))* glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f))* glm::scale(identityMatrix, glm::vec3(2.0, 2.5f, 0.05f)));
	wall.drawCubeWithTexture(lshdader, modelmat*glm::scale(identityMatrix, glm::vec3(1.0f, 2.0f, 0.3f)) * glm::translate(identityMatrix, glm::vec3(2.0f, 0.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(identityMatrix, glm::vec3(2.55, 1.5f, 0.05f)));

}

void player(Cube cubeFor_head, Shader lightingShaderWithTexture, glm::mat4 model, glm::mat4 combineMat)
{
	glm::mat4 char_head = glm::mat4(1.0f);
	char_head = glm::translate(model, glm::vec3(-1.2f, 0.8f, -0.63f));
	char_head = glm::scale(char_head, glm::vec3(0.4f, 0.4f, 0.25f));
	char_head = char_head * combineMat;
	cubeFor_head.drawCubeWithTexture(lightingShaderWithTexture, char_head);

	glm::mat4 char_body = glm::mat4(1.0f);
	char_body = glm::translate(model, glm::vec3(-1.3f, 0.45f, -0.68f));
	char_body = glm::scale(char_body, glm::vec3(0.8f, 0.8f, 0.4f));
	char_body = char_body * combineMat;
	cubeFor_head.drawCubeWithTexture(lightingShaderWithTexture, char_body);

	glm::mat4 char_leftLeg = glm::mat4(1.0f);
	char_leftLeg = glm::translate(model, glm::vec3(-1.0f, 0.08f, -0.62f));
	char_leftLeg = glm::scale(char_leftLeg, glm::vec3(0.2f, 1.0f, 0.15f));
	char_body = char_leftLeg * combineMat;
	cubeFor_head.drawCubeWithTexture(lightingShaderWithTexture, char_leftLeg);

	glm::mat4 char_rightLeg = glm::mat4(1.0f);
	char_rightLeg = glm::translate(model, glm::vec3(-1.3f, 0.08f, -0.62f));
	char_rightLeg = glm::scale(char_rightLeg, glm::vec3(0.2f, 1.0f, 0.15f));
	char_body = char_rightLeg * combineMat;
	cubeFor_head.drawCubeWithTexture(lightingShaderWithTexture, char_rightLeg);

}

void drawAirport(Pentagon &pent,Trapezoid &trapezoid, Cube &cube,Cube &wall, Shader lightingShaderWithTexture, glm::mat4 modelforairport)
{
	glm::mat4 identityMatrix = glm::mat4(1.0f);
	glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
	glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix;
	translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
	rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
	rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
	scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
	float i = 0;
		while (i < 3)
		{
		    rotateAngle_X = 90;
		    rotateAngle_Y = 0;
		    rotateAngle_Z = 4.5;

		    modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(1.8f, i - 0.0f, -4.5f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(identityMatrix, glm::vec3(0.445, 0.445, 2.18));
		    pent.drawPentagonWithTexture(lightingShaderWithTexture, modelforairport* modelMatrixForContainer);

		    rotateAngle_Z = 0;
		    modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-5.0f, i + 0.08f, -0.5f)) * glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(identityMatrix, glm::vec3(5.0, 0.08, 0.5));
		    trapezoid.drawTrapezoidWithTexture(lightingShaderWithTexture, modelforairport * modelMatrixForContainer);

		    rotateAngle_X = 0;
		    rotateAngle_Y = 0;
		    rotateAngle_Z = 0;
		    modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-5.0f, i - 0.0f, -0.5f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(identityMatrix, glm::vec3(5.0, 0.08, 2.5));
		    cube.drawCubeWithTexture(lightingShaderWithTexture, modelforairport * modelMatrixForContainer);
		    modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-4.0f, i - 0.0f, -1.8f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(identityMatrix, glm::vec3(3.0, 0.08, 0.8));
		    cube.drawCubeWithTexture(lightingShaderWithTexture, modelforairport * modelMatrixForContainer);

		    modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-2.75f, i - 0.0f, -2.8f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(identityMatrix, glm::vec3(0.5, 0.08, 1.0));
		    cube.drawCubeWithTexture(lightingShaderWithTexture, modelforairport * modelMatrixForContainer);
		    rotateAngle_X = 0;
		    rotateAngle_Y = 0;
		    rotateAngle_Z = 0;
		    modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-3.46f, i - 0.0f, -3.8f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(identityMatrix, glm::vec3(2.0, 0.08, 1.0));
		    trapezoid.drawTrapezoidWithTexture(lightingShaderWithTexture, modelforairport * modelMatrixForContainer);

		    modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-2.85f, i - 0.0f, -2.8f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(identityMatrix, glm::vec3(0.8, 0.08, 1.0));
		    cube.drawCubeWithTexture(lightingShaderWithTexture, modelforairport * modelMatrixForContainer);
		    rotateAngle_Y = -70;
		    rotateAngle_X = 180;
		     //Define the shear factors for shearing along the X-axis
		    float shearFactorX = 0.9f; // Adjust this value as needed

		     //Create a 4x4 identity matrix
		    glm::mat4 shearMatrix = glm::mat4(1.0f);

		     //Apply shear along the X-axis
		    shearMatrix[0][2] = shearFactorX; // Shear in the Z axis
		    shearMatrix[1][2] = shearFactorX;
		    modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(0.88f, i - 0.0f, -4.65f)) * glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f)) * shearMatrix * glm::scale(identityMatrix, glm::vec3(0.8, 0.08, 2.5));
		    cube.drawCubeWithTexture(lightingShaderWithTexture, modelforairport * modelMatrixForContainer);

		    rotateAngle_Y = -112;
		    rotateAngle_Z = 0;
		    modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(2.4f, i - 0.0f, -4.42f)) * glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(identityMatrix, glm::vec3(1.8, 1.1, 1.5));
		    trapezoid.drawTrapezoidWithMaterialisticProperty(lightingShaderWithTexture, modelforairport * modelMatrixForContainer);
		    

			

		    i=i+1;
		}
		wall.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(7.4f, 0.0f, 9.25f)) * glm::scale(identityMatrix, glm::vec3(0.1, 2.7f, 6.3f)));
		wall.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(7.4f, 2.5f, 9.25f)) * glm::scale(identityMatrix, glm::vec3(0.1, 2.7f, 6.3f)));
		wall.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(20.0f, 0.0f, 9.25f)) * glm::scale(identityMatrix, glm::vec3(0.1, 2.7f, 6.3f)));
		wall.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(20.0f, 2.5f, 9.25f)) * glm::scale(identityMatrix, glm::vec3(0.1, 2.7f, 6.3f)));

		wall.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(17.5f, 0.0f, 6.00f)) * glm::scale(identityMatrix, glm::vec3(0.1, 2.7f, 2.0f)));
		wall.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(17.5f, 2.5f, 6.00f)) * glm::scale(identityMatrix, glm::vec3(0.1, 2.7f, 2.0f)));
		wall.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(9.9f, 0.0f, 6.00f)) * glm::scale(identityMatrix, glm::vec3(0.1, 2.7f, 2.0f)));
		wall.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(9.9f, 2.5f, 6.00f)) * glm::scale(identityMatrix, glm::vec3(0.1, 2.7f, 2.0f)));

		wall.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(14.84f, 0.0f, 3.50f)) * glm::scale(identityMatrix, glm::vec3(0.1, 2.7f, 2.4f)));
		wall.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(14.84f, 2.5f, 3.50f)) * glm::scale(identityMatrix, glm::vec3(0.1, 2.7f, 2.4f)));
		wall.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.8f, 0.0f, 3.50f)) * glm::scale(identityMatrix, glm::vec3(0.1, 2.7f, 2.5f)));
		wall.drawCubeWithTexture(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(12.8f, 2.5f, 3.50f)) * glm::scale(identityMatrix, glm::vec3(0.1, 2.7f, 2.5f)));
}

void drawseat(Cube& seat, Cube &back, Cube &baseSeat, Shader lightingShaderWithTexture, glm::mat4 tranMat)
{
	glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
	glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
	translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
	rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
	rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
	scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
	float tranX = 0.0;
	glm::mat4 modelforseats = identityMatrix;
	for (int i = 0; i < 3; i++)
	{
		modelforseats = modelforseats * glm::translate(identityMatrix, glm::vec3(0.25f, 0.0f, 0.0));
		scale_X = 0.2f;
		scale_Y = 0.05f;
		scale_Z = 0.2f;
		seat.drawCubeWithTexture(lightingShaderWithTexture, tranMat* modelforseats * scaleMatrix);
		rotateAngle_X = -80.0f;
		translate_Z = 0.23f;
		back.drawCubeWithTexture(lightingShaderWithTexture, tranMat * modelforseats * translateMatrix * glm::rotate(identityMatrix, glm::radians(-80.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * scaleMatrix);

		baseSeat.drawCubeWithTexture(lightingShaderWithTexture, tranMat * glm::translate(identityMatrix, glm::vec3(0.25f, -0.11f, 0.2f)) * glm::scale(identityMatrix, glm::vec3(0.7, 0.15f, 0.04f)));
		baseSeat.drawCubeWithTexture(lightingShaderWithTexture, tranMat * glm::translate(identityMatrix, glm::vec3(0.25f, -0.11f, 0.03f)) * glm::scale(identityMatrix, glm::vec3(0.7, 0.15f, 0.04f)));
	}
	modelforseats = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.35f));
	for (int i = 0; i < 3; i++)
	{
		modelforseats = modelforseats * glm::translate(identityMatrix, glm::vec3(0.25f, 0.0f, 0.0f));
		scale_X = 0.2f;
		scale_Y = 0.05f;
		scale_Z = 0.2f;
		seat.drawCubeWithTexture(lightingShaderWithTexture, tranMat * modelforseats * scaleMatrix);
		rotateAngle_X = -80.0f;
		translate_Z = 0.23f;
		back.drawCubeWithTexture(lightingShaderWithTexture, tranMat * modelforseats * translateMatrix * glm::rotate(identityMatrix, glm::radians(-80.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * scaleMatrix);

		baseSeat.drawCubeWithTexture(lightingShaderWithTexture, tranMat * glm::translate(identityMatrix, glm::vec3(0.25f, -0.11f, 0.55f)) * glm::scale(identityMatrix, glm::vec3(0.7, 0.15f, 0.04f)));
		baseSeat.drawCubeWithTexture(lightingShaderWithTexture, tranMat * glm::translate(identityMatrix, glm::vec3(0.25f, -0.11f, 0.38f)) * glm::scale(identityMatrix, glm::vec3(0.7, 0.15f, 0.04f)));
	}
	modelforseats = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.70f));
	for (int i = 0; i < 3; i++)
	{
		modelforseats = modelforseats * glm::translate(identityMatrix, glm::vec3(0.25f, 0.0f, 0.0f));
		scale_X = 0.2f;
		scale_Y = 0.05f;
		scale_Z = 0.2f;
		seat.drawCubeWithTexture(lightingShaderWithTexture, tranMat * modelforseats * scaleMatrix);
		rotateAngle_X = -80.0f;
		translate_Z = 0.23f;
		back.drawCubeWithTexture(lightingShaderWithTexture, tranMat * modelforseats * translateMatrix * glm::rotate(identityMatrix, glm::radians(-80.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * scaleMatrix);

		baseSeat.drawCubeWithTexture(lightingShaderWithTexture, tranMat * glm::translate(identityMatrix, glm::vec3(0.25f, -0.11f, 0.72f)) * glm::scale(identityMatrix, glm::vec3(0.7, 0.15f, 0.04f)));
		baseSeat.drawCubeWithTexture(lightingShaderWithTexture, tranMat * glm::translate(identityMatrix, glm::vec3(0.25f, -0.11f, 0.89f)) * glm::scale(identityMatrix, glm::vec3(0.7, 0.15f, 0.04f)));
	}

}

void drawHeli(BezierCurve& dndi,BezierCurve& bj, Cube& cube, Shader lShader,glm::mat4 tran)
{

	glm::mat4 identityMatrix = glm::mat4(1.0f);
	glm::mat4 model= glm::mat4(1.0f);
	glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix;
	translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
	rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
	rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
	scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));

	//fan spindle
	scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 1.0f, 0.2f));
	translateMatrix = glm::translate(identityMatrix, glm::vec3(0.9f, 0.0f, 0.27f));
	model = translateMatrix * scaleMatrix;
	cube.drawCubeWithTexture(lShader,tran*model);

	//fan blades
	//b1 b3
	rotateYMatrix = glm::rotate(identityMatrix, glm::radians(r), glm::vec3(0.0f, 0.075f, 0.0f));
	scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.015f, 1.6f));
	translateMatrix = glm::translate(identityMatrix, glm::vec3(0.08f, 1.0f, -0.7f));
	model = glm::translate(identityMatrix, glm::vec3(1.0f, 0.0f, 0.27f)) * rotateYMatrix * glm::translate(identityMatrix, glm::vec3(-0.08f, -0.02f, -0.07f)) * translateMatrix * scaleMatrix;
	cube.drawCubeWithTexture(lShader,tran* model);

	//b2 b4
	scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.015f, 1.6f));
	rotateYMatrix = glm::rotate(identityMatrix, glm::radians(r), glm::vec3(0.0f, 0.075f, 0.0f));
	translateMatrix = glm::translate(identityMatrix, glm::vec3(0.08f, 1.0f, -0.7f));
	model =glm::translate(identityMatrix, glm::vec3(1.0f, 0.0f, 0.37f)) * rotateYMatrix * glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(identityMatrix, glm::vec3(-0.08f, -0.02f, -0.07f)) * translateMatrix * scaleMatrix;
	cube.drawCubeWithTexture(lShader,tran* model);

	scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 0.5f, 0.6f));
	cube.drawCubeWithTexture(lShader,tran* scaleMatrix);

	bj.drawBezierCurve(lShader,tran*glm::translate(identityMatrix, glm::vec3(4.0f, 0.3, 0.32))* glm::scale(identityMatrix, glm::vec3(2.0, 0.3, 0.4)) * glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	//bj.drawBezierCurve(lShader, glm::translate(identityMatrix, glm::vec3(1.8f, 2.0, 0.4)) * glm::rotate(identityMatrix, glm::radians(105.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(identityMatrix, glm::vec3(0.4, 1.2, 0.4)));
	bj.drawBezierCurve(lShader,tran* glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f))* glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f))* glm::translate(identityMatrix, glm::vec3(3.4f, -0.12f, -0.325f)) * glm::scale(identityMatrix, glm::vec3(2.0, 0.3, 0.4)) * glm::rotate(identityMatrix, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	//glm::translate(identityMatrix, glm::vec3(2.0f, 0.1f, 0.32f))

	scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 0.35f, 0.4f));
	cube.drawCubeWithTexture(lShader, tran*glm::translate(identityMatrix, glm::vec3(0.5f, 0.35, 0.1))* scaleMatrix);
	dndi.drawBezierCurve(lShader,tran* glm::translate(identityMatrix, glm::vec3(11.3f, 0.5, 0.32)) * glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(identityMatrix, glm::vec3(0.1f, 4.0f, 0.15f)));
	bj.drawBezierCurve(lShader,tran* glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(identityMatrix, glm::vec3(1.9f, 0.62, -0.32)) * glm::scale(identityMatrix, glm::vec3(2.3, 0.2, 0.2)) * glm::rotate(identityMatrix, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f)));

	////fan blades
	////b1 b3
	////rotateYMatrix = glm::rotate(identityMatrix, glm::radians(r), glm::vec3(0.0f, 0.075f, 0.0f));
	//scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.015f, 1.6f));
	//translateMatrix = glm::translate(identityMatrix, glm::vec3(0.08f, 1.0f, -0.7f));
	//model = glm::translate(identityMatrix, glm::vec3(3.3f, 0.6f, 0.33f)) * glm::scale(identityMatrix, glm::vec3(0.3f, 0.3f, 0.1f))* glm::rotate(identityMatrix, glm::radians(r), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(identityMatrix, glm::vec3(1.0f, 0.0f, 0.27f)) * glm::translate(identityMatrix, glm::vec3(-0.3f, -0.3f, -0.1f)) * translateMatrix * scaleMatrix;
	//cube.drawCubeWithTexture(lShader, tran*model);

	////b2 b4
	//scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.015f, 1.6f));
	//translateMatrix = glm::translate(identityMatrix, glm::vec3(0.08f, 1.0f, -0.7f));
	//model = glm::translate(identityMatrix, glm::vec3(3.3f, 0.6f, 0.33f)) * glm::scale(identityMatrix, glm::vec3(0.3f, 0.3f, 0.1f))* glm::rotate(identityMatrix, glm::radians(r+90.0f), glm::vec3(.0f, 0.0f, 1.0f)) * glm::translate(identityMatrix, glm::vec3(1.0f, 0.0f, 0.27f)) * glm::translate(identityMatrix, glm::vec3(-0.3f, -0.3f, -0.1f)) * translateMatrix * scaleMatrix;
	//cube.drawCubeWithTexture(lShader,tran* model);

	scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.15f, 0.01f));
	translateMatrix = glm::translate(identityMatrix, glm::vec3(0.08f, 1.0f, -0.7f));
	model = glm::translate(identityMatrix, glm::vec3(3.5f, 0.6f, 0.3f)) * glm::scale(identityMatrix, glm::vec3(0.3f, 0.6f, 0.1f));
	cube.drawCubeWithTexture(lShader,tran*model);

}

void drawCtrlTower(BezierCurve& curve,Frustum& frstm, Sphere& globe, Hexagon& hex, Shader lshader, Shader lshader2, glm::mat4 model)
{
	glm::mat4 identityMatrix = glm::mat4(1.0f);
	
	glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix;
	translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
	rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
	rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
	scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
	hex.drawHexagonWithTexture(lshader, model*glm::scale(identityMatrix, glm::vec3(1.0, 0.25, 1.0)));
	hex.drawHexagonWithMaterialisticProperty(lshader, model*glm::scale(identityMatrix, glm::vec3(0.4, 4.0, 0.4)));
	frstm.drawFrustumWithTexture(lshader, model*glm::scale(identityMatrix, glm::vec3(0.5, 4.0, 0.5)));
	frstm.drawFrustumWithTexture(lshader, model* glm::translate(identityMatrix, glm::vec3(0.0, 5.2, 0.0))* glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(identityMatrix, glm::vec3(0.5, 3.0, 0.5)));
	globe.drawSphereWithTexture(lshader, model*glm::translate(identityMatrix, glm::vec3(0.0, 8.5, 0.0)) * glm::scale(identityMatrix, glm::vec3(2.5, 2.6, 2.5)));
	//curve.drawBezierCurve(lshader, model*glm::translate(identityMatrix, glm::vec3(0.0, 10.5, 0.0))* glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(identityMatrix, glm::vec3(1.5, 2.0, 1.5)));
}

void drawTreeWithFractiles(Cube &cube, Shader& lightingShader, glm::mat4 alTogether, float L, float H, float W, int N)
{
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 translate = glm::mat4(1.0f);
	glm::mat4 identityMatrix = glm::mat4(1.0f);
	glm::mat4 scale = glm::mat4(1.0f);
	glm::mat4 next = glm::mat4(1.0f);
	glm::mat4 rotate = glm::mat4(1.0f);


	model = identityMatrix;
	

	if (N == 0) {
		float length = 0.5;
		float height = 2.0;
		float width = 0.5;

		float mvx = length * 0.05;
		float mvy = height - height * 0.1;
		float mvz = width * 0.05;


		translate = glm::translate(identityMatrix, glm::vec3(mvx, mvy, mvz));
		next = translate * alTogether;


		scale = glm::scale(identityMatrix, glm::vec3(length, height, width));
		model = alTogether * scale;
		cube.drawCube(lightingShader, model, 0.0, 0.95, 0.95);
		drawTreeWithFractiles(cube, lightingShader, next, length, height, width, N + 1);
	}

	else if (N > 0 && N < 5) {
		float length = L * 0.6;
		float height = H * 0.6;
		float width = W * 0.6;

		scale = glm::scale(identityMatrix, glm::vec3(length, height, width));
		rotate = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = alTogether * rotate * scale;
		cube.drawCube(lightingShader, model, 0.23, 0.08, 0.08);

		scale = glm::scale(identityMatrix, glm::vec3(length, height, width));
		rotate = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = alTogether * rotate * scale;
		cube.drawCube(lightingShader, model, 0.23, 0.08, 0.08);

		scale = glm::scale(identityMatrix, glm::vec3(length, height, width));
		rotate = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = alTogether * rotate * scale;
		cube.drawCube(lightingShader, model, 0.23, 0.08, 0.08);

		scale = glm::scale(identityMatrix, glm::vec3(length, height, width));
		rotate = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = alTogether * rotate * scale;
		cube.drawCube(lightingShader, model, 0.23, 0.08, 0.08);

		scale = glm::scale(identityMatrix, glm::vec3(length, height*1.5, width));
		model = alTogether * scale;
		cube.drawCube(lightingShader, model, 0.23, 0.08, 0.08);


		float mvx = length * 0.3 + height * 0.7071;
		float mvy = height * 0.7071;
		float mvz = width * 0.3;
		translate = glm::translate(identityMatrix, glm::vec3(mvx, mvy, mvz));
		next = translate * alTogether;
		drawTreeWithFractiles(cube, lightingShader, next, length, height, width, N + 1);


		mvx = length * 0.3;
		mvy = height * 0.7071;
		mvz = width * 0.3 - height * 0.7071;
		translate = glm::translate(identityMatrix, glm::vec3(mvx, mvy, mvz));
		next = translate * alTogether;
		drawTreeWithFractiles(cube, lightingShader, next, length, height, width, N + 1);


		mvx = length * 0.3 - height * 0.7071;
		mvy = height * 0.7071;
		mvz = width * 0.3;
		translate = glm::translate(identityMatrix, glm::vec3(mvx, mvy, mvz));
		next = translate * alTogether;
		drawTreeWithFractiles(cube, lightingShader, next, length, height, width, N + 1);


		mvx = length * 0.3;
		mvy = height * 0.7071;
		mvz = width * 0.3 + height * 0.7071;
		translate = glm::translate(identityMatrix, glm::vec3(mvx, mvy, mvz));
		next = translate * alTogether;
		drawTreeWithFractiles(cube, lightingShader, next, length, height, width, N + 1);

		mvx = length * 0.3;
		mvy = height;
		mvz = width * 0.3;
		translate = glm::translate(identityMatrix, glm::vec3(mvx, mvy *  1.5, mvz));
		next = translate * alTogether;
		drawTreeWithFractiles(cube, lightingShader, next, length, height, width, N + 1);
	}

	else{
		float length = L * 0.6;
		float height = H * 0.6;
		float width = W * 0.6;

		scale = glm::scale(identityMatrix, glm::vec3(length, height, width));
		rotate = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = alTogether * rotate * scale;
		cube.drawCube(lightingShader, model, 0.01, 0.23, 0.03);

		scale = glm::scale(identityMatrix, glm::vec3(length, height, width));
		rotate = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = alTogether * rotate * scale;
		cube.drawCube(lightingShader, model, 0.01, 0.23, 0.03);

		scale = glm::scale(identityMatrix, glm::vec3(length, height, width));
		rotate = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = alTogether * rotate * scale;
		cube.drawCube(lightingShader, model, 0.01, 0.23, 0.03);

		scale = glm::scale(identityMatrix, glm::vec3(length, height, width));
		rotate = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = alTogether * rotate * scale;
		cube.drawCube(lightingShader, model, 0.01, 0.23, 0.03);

		scale = glm::scale(identityMatrix, glm::vec3(length, height, width));
		model = alTogether * scale;
		cube.drawCube(lightingShader, model, 0.01, 0.23, 0.03);
	}

}

void drawCounter(Cube& table, Cube& wall, Shader ls, glm::mat4 model)
{
	glm::mat4 identityMatrix = glm::mat4(1.0f);
	wall.drawCubeWithTexture(ls, model * glm::scale(identityMatrix, glm::vec3(1.0f, 1.65f, 0.05f)));
	wall.drawCubeWithTexture(ls, model * glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 1.4)) * glm::scale(identityMatrix, glm::vec3(1.0f, 1.65f, 0.05f)));
	wall.drawCubeWithTexture(ls, model * glm::translate(identityMatrix, glm::vec3(0.0, 1.4, 0.0)) * glm::scale(identityMatrix, glm::vec3(1.0f, 0.05f, 1.4f)));
	table.drawCubeWithTexture(ls, model * glm::translate(identityMatrix, glm::vec3(-0.05f, 0.0f, 0.1f)) * glm::scale(identityMatrix, glm::vec3(0.4f, 0.8f, 1.2f)));
}

//void basement(Trapezoid trapezoid, Cube cube, Pentagon pent,glm::mat4 model)
//{
//    
//
//    glm::mat4 identityMatrix1 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix1, rotateXMatrix1, rotateYMatrix1, rotateZMatrix1, scaleMatrix1;
//    //translation matrix
//    translateMatrix1 = glm::translate(identityMatrix1, glm::vec3(translate_X, translate_Y, translate_Z));
//    //rotation matrices
//    
//    rotateYMatrix1 = glm::rotate(identityMatrix1, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
//    rotateZMatrix1 = glm::rotate(identityMatrix1, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
//    //scaling matrices
//    scaleMatrix1 = glm::scale(identityMatrix1, glm::vec3(scale_X, scale_Y, scale_Z));
//
//    //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
//    rotateAngle_X = 90;
//    translate_X = 2;
//    translate_Y = 0;
//    translate_Z = -5;
//    scale_X = 0.02;
//    scale_Y = 0.02;
//    scale_Z = 0.02;
//    rotateXMatrix1 = glm::rotate(identityMatrix1, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
//    translateMatrix1 = glm::translate(identityMatrix1, glm::vec3(translate_X, translate_Y, translate_Z));
//    model = translateMatrix1 * rotateXMatrix1 * scaleMatrix1;
//    trapezoid.drawTrapezoidWithTexture(lightingShaderWithTexture, model);
//}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		if (pointLightOn)
		{
			pointlight1.turnOff();
			pointlight2.turnOff();
			pointlight3.turnOff();
			pointlight4.turnOff();
			pointLightOn = !pointLightOn;
		}
		else
		{
			pointlight1.turnOn();
			pointlight2.turnOn();
			pointlight3.turnOn();
			pointlight4.turnOn();
			pointLightOn = !pointLightOn;
		}
	}

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

