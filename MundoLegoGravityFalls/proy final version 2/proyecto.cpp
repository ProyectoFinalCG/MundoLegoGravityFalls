//BUENISIMO

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

//Load Models carga de modelos
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
Camera  camera(glm::vec3(15.0f, 10.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float spotAngle = 0.0f;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);// posicion de luz direccional
bool active;


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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "P", nullptr, nullptr);

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
	glewExperimental = GL_TRUE;
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
	glEnable(GL_BLEND);/**/
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);/**/

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");

	//Model Mymodel((char*)"Models/proy/PRUEBAGRANDE.obj");

	// Build and compile our shader program


	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{



		//ordenamiento por vertices


		30.0f,  7.0f, -10.0f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,//tapadera
		30.0f,  7.0f, -40.0f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		15.0f,  25.0f,  -40.0f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		15.0f,  25.0f, -40.0f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		15.0f,  25.0f,  -10.0f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		30.0f,  7.0f, -10.0f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,

		0.0f,  7.0f, -10.0f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,//tapadera
		0.0f,  7.0f, -40.0f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		15.0f,  25.0f,  -40.0f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		15.0f,  25.0f, -40.0f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		15.0f,  25.0f,  -10.0f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		0.0f,  7.0f, -10.0f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,

		0.0f,  0.0f,  -10.0f,    -1.0f,  0.0f,  0.0f,    6.0f,  0.0f,//lado izquierdo
		0.0f,  0.0f, -40.0f,    -1.0f,  0.0f,  0.0f,    6.0f,  6.0f,
		0.0f, 7.0f, -40.0f,    -1.0f,  0.0f,  0.0f,    0.0f,  6.0f,
		0.0f, 7.0f, -40.0f,    -1.0f,  0.0f,  0.0f,    0.0f,  6.0f,
		0.0f, 7.0f,  -10.0f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		0.0f,  0.0f,  -10.0f,    -1.0f,  0.0f,  0.0f,    6.0f,  0.0f,//29

		0.0f,	0.0f,  -10.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,//frente cubo
		30.0f,  0.0f,  -10.0f,     0.0f,  0.0f,  1.0f,     6.0f,  0.0f,//izq arriba 
		30.0f,  7.0f,  -10.0f,     0.0f,  0.0f,  1.0f,     6.0f,  6.0f,
		30.0f,  7.0f,  -10.0f,     0.0f,  0.0f,  1.0f,  	6.0f,  6.0f,//este
		0.0f,   7.0f,  -10.0f,    0.0f,  0.0f,  1.0f,     0.0f,  6.0f,//y este sale el derecho abajo
		0.0f,   0.0f,  -10.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,//11

		30.0f,  0.0f,  -10.0f,    -1.0f,  0.0f,  0.0f,    6.0f,  0.0f,//lado derecho *** aquí reduce en 10 unidades
		30.0f,  0.0f, -40.0f,    -1.0f,  0.0f,  0.0f,    6.0f,  6.0f,
		30.0f, 7.0f, -40.0f,    -1.0f,  0.0f,  0.0f,    0.0f,  6.0f,
		30.0f, 7.0f, -40.0f,    -1.0f,  0.0f,  0.0f,    0.0f,  6.0f,
		30.0f, 7.0f,  -10.0f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,//**reduce en 10 unidades
		30.0f,  0.0f,  -10.0f,    -1.0f,  0.0f,  0.0f,    6.0f,  0.0f,//reduce en 10 unidades profundidad

		0.0f, 0.0f,  -40.0f,    0.0f,  0.0f,  -1.0f,     0.0f,  0.0f,//frente cubo
		30.0f, 0.0f,  -40.0f,     0.0f,  0.0f,  -1.0f,     6.0f,  0.0f,//izq arriba 
		30.0f,  7.0f,  -40.0f,     0.0f,  0.0f,  -1.0f,     6.0f,  6.0f,//este
		30.0f,  7.0f,  -40.0f,     0.0f,  0.0f,  -1.0f,  	6.0f,  6.0f,//y este sale el derecho abajo
		0.0f,  7.0f,  -40.0f,    0.0f,  0.0f,  -1.0f,     0.0f,  6.0f,
		0.0f, 0.0f,  -40.0f,    0.0f,  0.0f,  -1.0f,     0.0f,  0.0f,//5


		0.0f, 0.0f, -10.0f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,//base
		30.0f, 0.0f, -10.0f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		30.0f, 0.0f,  -40.0f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		30.0f, 0.0f,  -40.0f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.0f, 0.0f,  -40.0f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		0.0f, 0.0f, -10.0f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,//35


//*************parte arriba atras izq


		0.0f, 7.0f,  -40.0f,     0.0f,  0.0f,  1.0f,     6.0f,  0.0f,//izq arriba 	
		15.0f,  25.0f, -40.0f,    0.0f,  0.0f,  1.0f,     0.0f,  6.0f,//y este sale el derecho abajo
		15.0f, 7.0f, -40.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,//17		
		0.0f, 25.0f,  -40.0f,     0.0f,  0.0f,  1.0f,     6.0f,  0.0f,//izq arriba 
		0.0f, 7.0f,   -40.0f,     0.0f,  0.0f,  1.0f,     6.0f,  6.0f,
		0.0f, 7.0f,   -40.0f,     0.0f,  0.0f,  1.0f,  	6.0f,  6.0f,//este

//***********fin parte arriba atras izq********************

//*************parte arriba atras der**********************


		30.0f, 7.0f,  -40.0f,     0.0f,  0.0f,  1.0f,     6.0f,  0.0f,//izq arriba 		
		15.0f,  25.0f, -40.0f,    0.0f,  0.0f,  1.0f,     0.0f,  6.0f,//y este sale el derecho abajo
		15.0f, 7.0f, -40.0f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,//17
		30.0f, 7.0f,  -40.0f,     0.0f,  0.0f,  1.0f,     6.0f,  0.0f,//izq arriba 
		30.0f, 25.0f,   -40.0f,     0.0f,  0.0f,  1.0f,     6.0f,  6.0f,
		30.0f, 25.0f,   -40.0f,     0.0f,  0.0f,  1.0f,  	6.0f,  6.0f,//este

//***********fin parte arriba atras der********************

//parte arriba delante izq*********************************



			0.0f, 7.0f, -10.0f, 0.0f, 0.0f, 1.0f, 6.0f, 0.0f,//izq arriba 			
			15.0f, 25.0f, -10.0f, 0.0f, 0.0f, 1.0f, 0.0f, 6.0f,//y este sale el derecho abajo
			15.0f, 7.0f, -10.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,//17
			0.0f, 25.0f, -10.0f, 0.0f, 0.0f, 1.0f, 6.0f, 0.0f,//izq arriba 
			0.0f, 7.0f, -10.0f, 0.0f, 0.0f, 1.0f, 6.0f, 6.0f,
			0.0f, 7.0f, -10.0f, 0.0f, 0.0f, 1.0f, 6.0f, 6.0f,//este

//***********fin parte arriba delante izq********************


//***********parte arriba delatne der********************


			30.0f, 7.0f, -10.0f, 0.0f, 0.0f, 1.0f, 6.0f, 0.0f,//izq arriba 			
			15.0f, 25.0f, -10.0f, 0.0f, 0.0f, 1.0f, 0.0f, 6.0f,//y este sale el derecho abajo
			15.0f, 7.0f, -10.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,//17
			30.0f, 7.0f, -10.0f, 0.0f, 0.0f, 1.0f, 6.0f, 0.0f,//izq arriba 
			30.0f, 25.0f, -10.0f, 0.0f, 0.0f, 1.0f, 6.0f, 6.0f,
			30.0f, 25.0f, -10.0f, 0.0f, 0.0f, 1.0f, 6.0f, 6.0f,//este
//***********fin parte arriba delatne der********************	
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
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// Load textures /////////////parte para crear texturas
	GLuint texture1, texture2;//declaro las texturas
	glGenTextures(1, &texture1);//las genero en memoria una difusa
	//glGenTextures(1, &texture2);//una especular ******* revisar shader

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



	//enlazamos las texturas


	// Set texture units
	lightingShader.Use();//cargamos en el shader las texturas, se pueden hasta 20
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	//glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

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
		glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Load Model

		//***********************trabajando aqui***********************************************

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		//*******************FIN DE MIS INTENTOS
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.0f, 0.0f, 0.0f);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);//brillo que se manda al shader


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

		//// Bind specular map
		//glActiveTexture(GL_TEXTURE1);//textura con la que travajo texture1
		//glBindTexture(GL_TEXTURE_2D, texture2);


		glBindVertexArray(VAO);

		glm::mat4 model(1);

		////*********************************************la casa***************************************
		/*for (GLuint i = 0; i < 10; i++)
		{*/
		model = glm::mat4(1);


		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 66);


		glBindVertexArray(0);


		//}

		//////Carga de modelo  pescado************************************************************************************
		view = camera.GetViewMatrix();
		//model = glm::mat4(1);
		/*model = glm::translate(model, glm::vec3(15.0f, 10.0f, -30.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel.Draw(lightingShader);
*/
//glBindVertexArray(0);


// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		//para saber donde están mis luces
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");



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

	/*if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
			LightP1 = glm::vec3(0.839f, 0.643f, 0.788f);
		else
			LightP1 = glm::vec3(0.0f, 0.0f, 0.0f);
	}
*/

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
