#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rot = 0.0f;

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float spotAngle = 0.0f;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);// posicion de luz direccional
bool active;

//mis variables
float var = 0.0;
float var2 = 0.0;
float var3 = 3.0;



// Positions of the point lights ////////posiciones para nuestros pointligths uno por posicion
glm::vec3 pointLightPositions[] = {
	glm::vec3(-50.0f,  2.0f, -12.0f),
	glm::vec3(-50.0f,  2.0f, -12.0f),
	glm::vec3(-50.0f,  2.0f, -12.0f),
	glm::vec3(-50.0f,  2.0f, -12.0f)
};

glm::vec3 LightP1; //posicion extra para la luz

//******************** mias
glm::vec3 spotLight[] = {
	glm::vec3(0.0f,  0.0f,  0.0f)
};




// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 8", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;//****************me deja mover sin que se salga el mouse de la ventana***************
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");

	//Model Mymodel((char*)"Models/Fishes/TropicalFish02.obj");

	// Build and compile our shader program


	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		//la fachada 
		//inferior izq
		// Positions            // Normals              // Texture Coords
		//-6.0f, 0.0f, 0.0f,    0.0f, 0.0f,-1.0f,		0.0f,0.0f,
		//0.0f, 0.0f, 0.0f,	   0.0f, 0.0f,-1.0f,		4.0f,0.0f,//carga en u el # de veces que le demos
		//0.0f,  3.0f, 0.0f,     0.0f, 0.0f,-1.0f,		4.0f,4.0,//carga en u el # de veces que le demos
		//-6.0f,  3.0f, 0.0f,    0.0f, 0.0f,-1.0f,		0.0f,4.0f,

		//////inf der
		//6.0f, 0.0f, 0.0f,    0.0f, 0.0f,-1.0f,		0.0f,0.0f,
		//0.0f, 0.0f, 0.0f,	   0.0f, 0.0f,-1.0f,		4.0f,0.0f,//carga en u el # de veces que le demos
		//0.0f,  3.0f, 0.0f,     0.0f, 0.0f,-1.0f,		4.0f,4.0,//carga en u el # de veces que le demos
		//6.0f,  3.0f, 0.0f,    0.0f, 0.0f,-1.0f,		0.0f,4.0f,

		//////sup der

		////carga en u el # de veces que le demos
		//0.0f,  3.0f, 0.0f,     0.0f, 0.0f,-1.0f,		0.0f,0.0f,//carga en u el # de veces que le demos
		//-6.0f,  3.0f, 0.0f,    0.0f, 0.0f,-1.0f,		4.0f,0.0f,
		//-6.0f,  15.0f, 0.0f,    0.0f, 0.0f,-1.0f,		4.0f,4.0f,
		//0.0f,  15.0f, 0.0f,	   0.0f, 0.0f,-1.0f,		0.0f,4.0f,

		//////sup izq
		//0.0f,  3.0f, 0.0f,    0.0f, 0.0f,-1.0f,		0.0f,0.0f,
		//6.0f,  3.0f, 0.0f,	   0.0f, 0.0f,-1.0f,		4.0f,0.0f,//carga en u el # de veces que le demos
		//6.0f,  15.0f, 0.0f,     0.0f, 0.0f,-1.0f,		4.0f,4.0f,//carga en u el # de veces que le demos
		//0.0f,  15.0f, 0.0f,    0.0f, 0.0f,-1.0f,		0.0f,4.0f,
//*****************************frente de casa********************************
		//eje origen 0 y30 largo |  0 en origen desde el suelo y 7alto | profundidad*************´por la puerta
		0.0f, 0.0f,  -10.0f,    0.0f,  0.0f,  -1.0f,     0.0f,  0.0f,//frente cubo
		30.0f, 0.0f,  -10.0f,     0.0f,  0.0f,  -1.0f,     6.0f,  0.0f,//izq arriba 
		30.0f,  7.0f,  -10.0f,     0.0f,  0.0f,  -1.0f,     6.0f,  6.0f,//este
		30.0f,  7.0f,  -10.0f,     0.0f,  0.0f,  -1.0f,  	6.0f,  6.0f,//y este sale el derecho abajo
		0.0f,  7.0f,  -10.0f,    0.0f,  0.0f,  -1.0f,     0.0f,  6.0f,
		0.0f, 0.0f,  -10.0f,    0.0f,  0.0f,  -1.0f,     0.0f,  0.0f,//5
//***************************fin de frente de casa************************

//**********************triangulo arriba casa********************************
		0.0f, 7.0f,  -10.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,//frente cubo
		15.0f, 7.0f,  -10.0f,     0.0f,  0.0f,  1.0f,     6.0f,  0.0f,//izq arriba 
		15.0f,  20.0f,  -10.0f,     0.0f,  0.0f,  1.0f,     6.0f,  6.0f,
		15.0f,  20.0f,  -10.0f,     0.0f,  0.0f,  1.0f,  	6.0f,  6.0f,//este
		0.0f,  20.0f,  -10.0f,    0.0f,  0.0f,  1.0f,     0.0f,  6.0f,//y este sale el derecho abajo
		0.0f, 7.0f,  -10.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,//11

		15.0f, 7.0f,  -10.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,//frente cubo
		30.0f, 7.0f,  -10.0f,     0.0f,  0.0f,  1.0f,     6.0f,  0.0f,//izq arriba 
		30.0f,  20.0f,  -10.0f,     0.0f,  0.0f,  1.0f,     6.0f,  6.0f,
		30.0f,  20.0f,  -10.0f,     0.0f,  0.0f,  1.0f,  	6.0f,  6.0f,//este
		15.0f,  20.0f,  -10.0f,    0.0f,  0.0f,  1.0f,     0.0f,  6.0f,//y este sale el derecho abajo
		15.0f, 7.0f,  -10.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,//17
//******************fin triangulo arriba casa*************************************
//*****************muro der***************************
		//cambia en x 30 la pos de mi vertice |  0 base del suelo y 7 altura |  largo esto porque ahora nos extendemos en funcion de z
		30.0f,  0.0f,  -10.0f,    -1.0f,  0.0f,  0.0f,    6.0f,  0.0f,//lado derecho *** aquí reduce en 10 unidades
		30.0f,  0.0f, -40.0f,    -1.0f,  0.0f,  0.0f,    6.0f,  6.0f,
		30.0f, 7.0f, -40.0f,    -1.0f,  0.0f,  0.0f,    0.0f,  6.0f,
		30.0f, 7.0f, -40.0f,    -1.0f,  0.0f,  0.0f,    0.0f,  6.0f,
		30.0f, 7.0f,  -10.0f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,//**reduce en 10 unidades
		30.0f,  0.0f,  -10.0f,    -1.0f,  0.0f,  0.0f,    6.0f,  0.0f,//reduce en 10 unidades profundidad
//********************fin muro der*************************************//23
//************************muro izq**********************
		0.0f,  0.0f,  -10.0f,    -1.0f,  0.0f,  0.0f,    6.0f,  0.0f,//lado izquierdo
		0.0f,  0.0f, -40.0f,    -1.0f,  0.0f,  0.0f,    6.0f,  6.0f,
		0.0f, 7.0f, -40.0f,    -1.0f,  0.0f,  0.0f,    0.0f,  6.0f,
		0.0f, 7.0f, -40.0f,    -1.0f,  0.0f,  0.0f,    0.0f,  6.0f,
		0.0f, 7.0f,  -10.0f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		0.0f,  0.0f,  -10.0f,    -1.0f,  0.0f,  0.0f,    6.0f,  0.0f,//29
//************fin muro izq***********************
//***********************base********************
////0 vertice origen y 30 ancho en X | posicoin del vertice | 30 largo y retrocedemos -10 unidades para que quede dentro el suelo
//		0.0f, 0.0f, -10.0f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,//base
//		30.0f, 0.0f, -10.0f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
//		30.0f, 0.0f,  -40.0f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
//		30.0f, 0.0f,  -40.0f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
//		0.0f, 0.0f,  -40.0f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
//		0.0f, 0.0f, -10.0f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,//35
////************************fin base************************
////***************************tras de casa***********************************
//	eje origen 0 y30 largo |  0 en origen desde el suelo y 7alto | profundidad*************´por la puerta
		//0.0f, 0.0f,  -40.0f,    0.0f,  0.0f,  -1.0f,     0.0f,  0.0f,//frente cubo
		//30.0f, 0.0f,  -40.0f,     0.0f,  0.0f,  -1.0f,     6.0f,  0.0f,//izq arriba 
		//30.0f,  7.0f,  -40.0f,     0.0f,  0.0f,  -1.0f,     6.0f,  6.0f,//este
		//30.0f,  7.0f,  -40.0f,     0.0f,  0.0f,  -1.0f,  	6.0f,  6.0f,//y este sale el derecho abajo
		//0.0f,  7.0f,  -40.0f,    0.0f,  0.0f,  -1.0f,     0.0f,  6.0f,
		//0.0f, 0.0f,  -40.0f,    0.0f,  0.0f,  -1.0f,     0.0f,  0.0f,//41

//*********************************fin de tras de casa************************
//********************************* TECHO COMPLEMENTARIO izq*********************
//vertice de origen 0.0 , vertice destino 15| vertice donde voy a empezar a trazar 7 en y (por la altura del plano de la entrada mide 7), altura del plano 10 | se hace hacia atrás -10 unidades y de largo desde origen a -40
		//0.0f,  7.0f, -10.0f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,//tapadera
		//0.0f,  7.0f, -40.0f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		//15.0f,  20.0f,  -40.0f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		//15.0f,  20.0f, -40.0f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		//15.0f,  20.0f,  -10.0f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		//0.0f,  7.0f, -10.0f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
//*******************************FIN TECHO COMPLEMENTARIO izq*****************************************

//********************************* TECHO COMPLEMENTARIO der*********************
//vertice de origen 0.0 , vertice destino 15| vertice donde voy a empezar a trazar 7 en y (por la altura del plano de la entrada mide 7), altura del plano 10 | se hace hacia atrás -10 unidades y de largo desde origen a -40
		30.0f,  7.0f, -10.0f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,//tapadera
		30.0f,  7.0f, -40.0f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		15.0f,  20.0f,  -40.0f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		15.0f,  20.0f, -40.0f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		15.0f,  20.0f,  -10.0f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		30.0f,  7.0f, -10.0f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f

		//*******************************FIN TECHO COMPLEMENTARIO der*****************************************


	};

	GLuint indices[] =
	{  // Note that we start from 0!

		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35,
		36,37,38,39,
		40,41,42,43,
		44,45,46,47,
		48,49,50,51,
	};

	 //Positions all containers ////////////la posicion de mis objetos
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		/*glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)*/
	};


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));/**********************/
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;//posiciones de memoria
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	//*****************AQUI VAN LAS LUCES AQUI VAN LAS LUCES AQUI VAN LAS LUCES AQUI VAN LAS LUCES******************************
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	//***********FIN DE AQUI VAN LAS LUCES***************************************
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// Load textures /////////////parte para crear texturas
	GLuint texture1, texture2;//declaro las texturas
	glGenTextures(1, &texture1);//las genero en memoria una difusa
	glGenTextures(1, &texture2);//una especular ******* revisar shader

	int textureWidth, textureHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);//el flip
	unsigned char *image;//caract de repeticion
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//parte de filtrado
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	// Diffuse map************************************************************************
	image = stbi_load("images/casafrente.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)//la parte difusa, liberar memoria, etc etc etc
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);

	//////*********************************************ESPECULAR *****************************
	////// Specular map
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	//image = stbi_load("images/awesomeface.png", &textureWidth, &textureHeight, &nrChannels, 0);
	//glBindTexture(GL_TEXTURE_2D, texture2);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	//glGenerateMipmap(GL_TEXTURE_2D);
	//if (image)
	//{
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else
	//{
	//	std::cout << "Failed to load texture" << std::endl;
	//}
	//stbi_image_free(image);
////**************************************fin ESPECULAR**************************
	glBindTexture(GL_TEXTURE_2D, 0);
	////enlazamos las texturas
			

	// Set texture units
	lightingShader.Use();//cargamos en el shader las texturas, se pueden hasta 20
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.643f, 0.749f, 0.839f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Load Model

		//***********************trabajando aqui***********************************************

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		//*******************FIN DE MIS INTENTOS
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);//brillo que se manda al shader


		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================

		// Directional light, para esto encesito las 4 componentes(ambiente, difusa y especular) y la direccion
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);


		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.007f, 0.027f, 0.352f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.007f, 0.027f, 0.352f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), LightP1.x, LightP1.y, LightP1.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);



		// Point light 2 rosita
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.866f, 0.074f, 0.796f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.866f, 0.074f, 0.796f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		// Point light 3 rojo
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.921f, 0.0f, 0.039f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.921f, 0.0f, 0.039f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		// Point light 4 verde
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.0f, 0.921f, 0.607f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.0f, 0.921f, 0.607f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);

		// SpotLight
		//recibe- donde quiero que esté(ahora está pegada a la camara), direccion (pegada a la camara)
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), spotLight[0].x, spotLight[0].y, spotLight[0].z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);//obtengo el vector forwar de la camara, osea la posicion donde vaya la camara
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), var, var2, var3);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 1.0f, 0.0f);//***se cambia junto con specular mismo color
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 1.0f, 0.0f);//***se cambia junto con diffuse micmo color
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(15.0f)));//que tan abierto está el cono o radio interior
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(20.0f)));//que tan abierto está el cono exterioir

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);//brillo de la iluminacipon

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader ****** lo pasamos al shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Bind diffuse map
		glActiveTexture(GL_TEXTURE0);// textura con la que trabajo texture0
		glBindTexture(GL_TEXTURE_2D, texture1);

		// Bind specular map
		glActiveTexture(GL_TEXTURE1);//textura con la que travajo texture1
		glBindTexture(GL_TEXTURE_2D, texture2);

			   			   
		glBindVertexArray(VAO);
		glm::mat4 model(1);

		// We now draw as many light bulbs as we have point lights.//*/****mandamos a pintar

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


							/*for (GLuint i = 0; i < 10; i++)
							{
								model = glm::mat4(1);
								model = glm::translate(model, cubePositions[i]);
								float angle = 20.0f * i;
								model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
								glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
								glDrawArrays(GL_TRIANGLES, 0, 36);
							}*/

		//Carga de modelo 
	   /* view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f,0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel.Draw(lightingShader);*/

		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		//para saber donde están mis luces
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);
		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	if (keys[GLFW_KEY_1])
	{
		range += 0.1;
		printf("El rango es %f\n", range);
	}

	if (keys[GLFW_KEY_2])
	{
		range -= 0.1;
		printf("El rango es %f\n", range);
	}

	if (keys[GLFW_KEY_3])
	{
		range += 0.1;
		printf("El spotangle es %f\n", range);
	}

	if (keys[GLFW_KEY_4])
	{
		range -= 0.1;
		printf("El spotangle es %f\n", range);
	}

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (keys[GLFW_KEY_T])
	{
		//pointLightPositions[0].x -= 0.1f;
		//pointLightPositions[0].y -= 0.1f;
		pointLightPositions[0].z += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		//pointLightPositions[0].x -= 0.1f;
		//pointLightPositions[0].y -= 0.1f;
		pointLightPositions[0].z -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[1].x += 0.01f;
		pointLightPositions[1].y += 0.01f;
		pointLightPositions[1].z += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[1].x -= 0.01f;
		pointLightPositions[1].y -= 0.01f;
		pointLightPositions[1].z -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[2].x += 0.01f;
		pointLightPositions[2].y += 0.01f;
		pointLightPositions[2].z += 0.01f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[2].x -= 0.01f;
		pointLightPositions[2].y -= 0.01f;
		pointLightPositions[2].z -= 0.01f;
	}
	if (keys[GLFW_KEY_I])
	{
		pointLightPositions[3].x += 0.01f;
		pointLightPositions[3].y += 0.01f;
		pointLightPositions[3].z += 0.01f;
	}
	if (keys[GLFW_KEY_K])
	{
		pointLightPositions[3].x -= 0.01f;
		pointLightPositions[3].y -= 0.01f;
		pointLightPositions[3].z -= 0.01f;
	}

	//*************MOVIMIENTO DE SPOTLIGTH
	if (keys[GLFW_KEY_C])//arriba
	{
		spotLight[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_V])//abajo
	{
		spotLight[0].y -= 0.01f;
	}

	if (keys[GLFW_KEY_N])//izq
	{
		spotLight[0].x += 0.01f;
	}

	if (keys[GLFW_KEY_B])//der
	{
		spotLight[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_M])//cerca
	{
		spotLight[0].z += 0.01f;
	}

	if (keys[GLFW_KEY_L])//lejos
	{
		spotLight[0].z -= 0.01f;
	}

	if (keys[GLFW_KEY_P])//rot en X -
	{
		var -= 0.01;
	}

	if (keys[GLFW_KEY_O])//rot x+
	{
		var += 0.01;
	}

	if (keys[GLFW_KEY_6])//rot y+
	{
		var2 += 0.01;
	}

	if (keys[GLFW_KEY_7])//rot y-
	{
		var2 -= 0.01;
	}

	if (keys[GLFW_KEY_8])//rot z+
	{
		var3 += 0.01;
	}

	if (keys[GLFW_KEY_9])//rot z-
	{
		var3 -= 0.01;
	}

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
			LightP1 = glm::vec3(0.0f, 1.0f, 0.0f);
		else
			LightP1 = glm::vec3(0.0f, 0.0f, 0.0f);
	}


}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}