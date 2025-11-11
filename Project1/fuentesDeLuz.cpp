
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


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void Animation();

// Window dimensions
const GLuint WIDTH = 1000, HEIGHT = 800;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;




glm::vec3 lightPivots[] = {
	glm::vec3(-211.63f, 13.0f, 79.0f),
	glm::vec3(-101.54f,13.0f, 79.0f),


	glm::vec3(-211.63f, 13.0f, 162.62f),
	glm::vec3(-101.54f,13.0f, 162.62f),

	glm::vec3(-211.63f, 13.0f, 180.69f),
	glm::vec3(-101.54f,13.0f, 180.69f),

	glm::vec3(-211.63f, 13.0f, 269.26f),
	glm::vec3(-101.54f,13.0f, 269.26f),

	glm::vec3(-211.63f, 13.0f, 283.63f),
	glm::vec3(-101.54f,13.0f, 283.63f),

	glm::vec3(-211.63f, 13.0f, 368.893f),
	glm::vec3(-101.54f,13.0f, 368.893f),
};

glm::vec3 lightPivotsroof[] = {
	glm::vec3(-155.125f, 46.1014f, 79.0f),
	glm::vec3(-155.125f, 46.1014f, 162.923f),
	glm::vec3(-155.125f, 46.1014f, 180.69f),
	glm::vec3(-155.125f, 46.1014f, 269.255f),
	glm::vec3(-155.125f, 46.1014f, 283.631f),
	glm::vec3(-155.125f, 46.1014f, 368.893f),
};

std::vector<glm::vec3> pointLightPositions;



// Colores Difusos a alternar (R, G, B)
std::vector<glm::vec3> diffuseColors = {

	glm::vec3(0.0f, 0.3f, 1.0f),
	glm::vec3(1.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.3f, 1.0f),
	glm::vec3(1.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.3f, 1.0f),
	glm::vec3(1.0f, 1.0f, 0.0f),
	

};

// 18 luces por pivote, como lo especificaste
const int LIGHTS_PER_PIVOT = 18;


float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};


//configuracion de cada una de las luces
glm::vec3 Light1 = glm::vec3(0);


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame


//AmimacionMario
float headMario = 0.0f;
float bodyMario = 0.0f;
float armRightMario = 0.0f;
float armLeftMario = 0.0f;
float legRightMario = 0.0f;
float legLeftMario = 0.0f;
int marioState = 0;
float marioRotation = 0.0f;
float marioHeight = 0.0f;
float marioArmAngle = 50.0f;
float armMarioAngle = 50.0f;
float armMarioAngleX = 0.0f;
float armMarioAngleY = 0.0f;
float armMarioPos = 1.0f;
float legMarioAngle = 0.0f;
float vueltasMario = 0.0f;

//animacion shine
float shineRotateAngle = 10.0f;
bool shineRotate = true;
bool shineUp = false;
float shineOrbitAngle = 0.0f;      
bool shineOrbitActive = false;    
float shineSpinAngle = 1.0f;       


bool AnimMario = false;
glm::vec3 marioPos(-152.0f, -6.42124f, 387.647f);
//glm::vec3 marioPos(0.0f, 0.0f, 0.0f);
glm::mat4 modelTemp = glm::mat4(1.0f); //Temp





int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto.Equipo5", nullptr, nullptr);

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
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}


	// Imprimimos informacin de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);





	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	
	Model Proyecto((char*)"models/proyecto.obj");
	
	
	//Modelos de Mario
	Model Shine((char*)"models/monedaMario.obj");
	Model MarioBody((char*)"models/body_mario.obj");
	Model HeadMario((char*)"models/head_mario.obj");
	Model RightLeg((char*)"models/R_leg_mario.obj");
	Model LeftLeg((char*)"models/L_leg_mario.obj");
	Model RightArm((char*)"models/R_arm_mario.obj");
	Model LeftArm((char*)"models/L_arm_mario.obj");


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 300.0f);


	const int NUM_PIVOTS = sizeof(lightPivots) / sizeof(lightPivots[0]);

	float yOffsets[] = {
	 0.0f, 3.0f, 6.0f, 9.0f, 12.0f, 16.0f, 23.0f, 26.0f, 29.0f, 32.0f, 35.0f, 38.0f, 41.0f, -3.0f, -6.0f, -9.0f, -12.0f, -15.0f
	};
	const int NUM_OFFSETS = sizeof(yOffsets) / sizeof(yOffsets[0]);

	// Generar y almacenar todas las posiciones de luz en el vector global
	for (int i = 0; i < NUM_PIVOTS; ++i) {
		glm::vec3 pivot = lightPivots[i];
		// Iterar sobre los desplazamientos en Y
		for (int j = 0; j < NUM_OFFSETS; ++j) {
			glm::vec3 finalPosition = glm::vec3(
				pivot.x,
				yOffsets[j],
				pivot.z
			);

			// Almacena la posición final en el vector global
			pointLightPositions.push_back(finalPosition);
		}
	}



	//luces en el techo

	float zOffsetsRoof[] = {
		 0.0f, 5.0f, 10.0f, 20.0f, 25.0f, 30.0f, 35.0f, 40.0f, 45.0f, 50.0f, 55.0f, 60.0f, 65.0f, -5.0f, -10.0f, -15.0f, -20.0f, -25.0f,
		  -30.0f, -35.0f, -40.0f, -45.0f, -50.0f, -55.0f, -60.0f, -65.0f
	};
	const int NUM_Z_OFFSETS = sizeof(zOffsetsRoof) / sizeof(zOffsetsRoof[0]);
	std::vector<glm::vec3> pointLightpivotsRoof;
	glm::vec3 roofLightColor = glm::vec3(1.0f, 0.6f, 1.0f); 


	const int NUM_PIVOTS_ROOF = sizeof(lightPivotsroof) / sizeof(lightPivotsroof[0]);
	for (int i = 0; i < NUM_PIVOTS_ROOF; ++i) {
		glm::vec3 pivot = lightPivotsroof[i];
		for (int j = 0; j < NUM_Z_OFFSETS; ++j) {
			glm::vec3 finalPosition = glm::vec3(
				pivot.x + zOffsetsRoof[j],
				pivot.y,           
				pivot.z 
			);
			pointLightpivotsRoof.push_back(finalPosition);
		}
	}


	for (int i = 0; i < pointLightpivotsRoof.size(); ++i) {
		pointLightPositions.push_back(pointLightpivotsRoof[i]);
	}


	// Calcula numLights a partir del vector después de que ha sido llenado
	const int numLights = pointLightPositions.size();

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
		Animation();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);



		//Load Model


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.8f, 0.8f, 0.8f);


		const int LIGHTS_PER_PIVOT = 18; // Definido aquí para mayor claridad o como constante global
		int numColors = diffuseColors.size();
		glm::vec3 currentDiffuseColor;

		for (int i = 0; i < numLights; ++i) {
			std::string base = "pointLights[" + std::to_string(i) + "]";

			// 1. Calcular el índice del pivote al que pertenece esta luz
			// Ej: Luz 0-17 -> Pivote 0; Luz 18-35 -> Pivote 1; etc.
			int pivotIndex = i / LIGHTS_PER_PIVOT;

			// 2. Calcular el índice del PAR de pivotes
			// Ej: Pivotes 0 y 1 -> Par 0; Pivotes 2 y 3 -> Par 1; etc.
			int pairIndex = pivotIndex / 2;

			// 3. Seleccionar el color usando el índice del par y el módulo de la cantidad de colores
			if (i >= 216) {
				currentDiffuseColor = glm::vec3(1.0f, 0.0f, 1.0f);
			}
			else {
				currentDiffuseColor = diffuseColors[pairIndex % numColors];
			}
			

			// 4. Enviar Posición
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".position").c_str()),
				pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);

			// 5. Enviar Colores (Difuso es el que cambia)

			// Color Ambiental: Tenue, basado en el difuso para mantener el tono
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".ambient").c_str()),
				currentDiffuseColor.x * 0.05f, currentDiffuseColor.y * 0.05f, currentDiffuseColor.z * 0.05f);

			// Color Difuso: El color principal del par de pivotes
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".diffuse").c_str()),
				currentDiffuseColor.x, currentDiffuseColor.y, currentDiffuseColor.z);

			// Color Especular: Reflejo suave (opcionalmente puedes usar el color difuso también)
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".specular").c_str()),
				currentDiffuseColor.x * 0.5f, currentDiffuseColor.y * 0.5f, currentDiffuseColor.z * 0.5f);

			// 6. Enviar Atenuación (Se mantiene constante)
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".constant").c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".linear").c_str()), 0.09f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".quadratic").c_str()), 0.032f);
		}

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), 0.0f, -1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"),0.0f, 2.0f, 10.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		//angulo interno y externo del foco
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(20.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(40.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);

	

		//Carga de modelo 
        view = camera.GetViewMatrix();	
		model = glm::mat4(1);
		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);


		//esqueleto del proyecto
	    Proyecto.Draw(lightingShader);

		//sol
		if (shineOrbitActive) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(marioPos.x, marioPos.y, marioPos.z)); 
			model = glm::rotate(model, glm::radians(shineOrbitAngle), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(0.0f, 2.0f, -15.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		}
		else {
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(marioPos.x, marioPos.y, marioPos.z - 15.0f));
			model = glm::rotate(model, glm::radians(shineSpinAngle), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		}

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Shine.Draw(lightingShader);



		// Mario
		glm::mat4 modelMario = glm::mat4(1.0f);
		modelMario = glm::translate(modelMario, marioPos);
		modelMario = glm::rotate(modelMario, glm::radians(marioRotation), glm::vec3(0.0f, 1.0f, 0.0f));

		// Guarda esta base para las partes del cuerpo
		glm::mat4 modelBase = modelMario;

		// Body
		model = modelBase;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		MarioBody.Draw(lightingShader);

		// Head
		model = modelBase;
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HeadMario.Draw(lightingShader);

		// Right Arm
		model = modelBase;
		model = glm::translate(model, glm::vec3(1.2f, armMarioPos, 0.0f));
		model = glm::rotate(model, glm::radians(-armMarioAngle), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RightArm.Draw(lightingShader);

		// Left Arm
		model = modelBase;
		model = glm::translate(model, glm::vec3(-1.2f, armMarioPos, 0.0f));
		model = glm::rotate(model, glm::radians(armMarioAngle), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(armMarioAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(armMarioAngleY), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LeftArm.Draw(lightingShader);

		// Right Leg
		model = modelBase;
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f)); 
		model = glm::rotate(model, glm::radians(legMarioAngle), glm::vec3(1.0f, 0.0, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RightLeg.Draw(lightingShader);

		//Left Leg
		model = modelBase;
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::rotate(model, glm::radians(legMarioAngle), glm::vec3(1.0f, 0.0, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LeftLeg.Draw(lightingShader);


		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
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
		// dibujar lampara verticales
		for (GLuint i = 0; i < 2; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, lightPivots[i]);
			model = glm::scale(model, glm::vec3(1.8f, 56.0f, 2.5f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		// Dibuja lámparas horizontales del techo
		for (GLuint i = 0; i < NUM_PIVOTS_ROOF; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, lightPivotsroof[i]);
			model = glm::scale(model, glm::vec3(100.0f, 1.8f, 2.5f)); // Alargada en Z
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

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
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}
	
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
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
		{
			Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}

	if (keys[GLFW_KEY_N])
	{
		AnimMario = !AnimMario;


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



void Animation()
{
	if (!AnimMario)
		return;

	// Actualiza el giro propio (siempre girando cuando shineRotate está true)
	// Uso deltaTime para velocidad independientemente del frame rate.
	if (shineRotate) {
		shineSpinAngle += 45.0f;
	}
	
	
	switch (marioState)
	{
	case 0:
		if (marioHeight < 8.0f) {
			marioHeight += 0.05f;
			marioPos.y += 0.05f;
			armMarioAngle -= 0.38f;
			armMarioPos -= 0.01f;
			legMarioAngle += 0.05f;
		}
		else {
			marioState = 1;
		}
		break;
	case 1:
		if (armMarioAngleX<90.0f) {
			armMarioAngleX += 10.0f;
		}
		else {
			marioState = 2;
		}
		break;
	case 2:
		if (armMarioAngleY < 90.0f) {
			armMarioAngleY += 10.0f;
		}
		else {
			marioState = 3;
		}
		break;
	case 3:
		if (vueltasMario < 2.0f) {
			marioRotation += 20.0f;
			shineOrbitAngle += 20.0f;
			if(marioRotation >= 360.0f) {
				vueltasMario += 1.0f;
				marioRotation = 0.0f;
			}
		}
		shineOrbitActive = true;
		shineRotate = false;
		break;
		
		break;
	default:
		break;
	}
}