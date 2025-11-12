
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
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void Animation();
void cargaFrames(string filename);

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

// posiciones de las luces puntuales en el escenario
std::vector<glm::vec3> pointLightPositions;

//posiciones de las luces puntuales en el techo
std::vector<glm::vec3> pointLightpivotsRoof;

//posiciones de la fuente de luz en las paredes (lamparas)
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


//posiciones de la fuente de luz en el techo (lamparas)
glm::vec3 lightPivotsroof[] = {
	glm::vec3(-155.125f, 46.1014f, 79.0f),
	glm::vec3(-155.125f, 46.1014f, 162.923f),
	glm::vec3(-155.125f, 46.1014f, 180.69f),
	glm::vec3(-155.125f, 46.1014f, 269.255f),
	glm::vec3(-155.125f, 46.1014f, 283.631f),
	glm::vec3(-155.125f, 46.1014f, 368.893f),
};

/*
* variables para la animacion de las monedas
* coinAnim: indica si la animacion de las monedas subiendo y bajando esta activa
* coinUp: indica si la moneda esta subiendo o bajando, true para subiendo, false para bajando
* coinRotate: indica si la moneda esta rotando, true para rotando, false para no rotando
* animateCoins: indica si la animacion de las monedas desplazandose esta activa
* animationProgress: progreso de la animacion de las monedas desplazandose el cual va de 0.0 a 1.0
* animationSpeed: velocidad de la animacion de las monedas desplazandose
* coinYOffset = 0.0f: offset en Y para la animacion de las monedas subiendo y bajando
* coinRotateY = 0.0f: angulo de rotacion en Y para la animacion de las monedas rotando
* animationDirectionCoins: direccion de la animacion de las monedas desplazandose 1 es para cuando se construye la figura y -1 para cuando se vuelve a su pos original

*/
float coinAnim = true; 
bool coinUp = true; 
bool coinRotate = true; 
bool animateCoins = false; 
float animationProgress = 0.0f;
float animationSpeed = 0.3f; 
float coinYOffset = 0.0f; 
float coinRotateY = 0.0f; 
int animationDirectionCoins = 1; 


//posiciones iniciales de las monedas
glm::vec3 coinsPos[] = {
	glm::vec3(-190.125f, 0.0f, 87.03f),
	glm::vec3(-170.125f, 2.0f, 150.03f),
	glm::vec3(-180.125f, 10.0f, 200.03f),
	glm::vec3(-130.125f, 4.0f, 140.03f),
	glm::vec3(-140.125f, 1.0, 210.03f),
	glm::vec3(-150.125f, 8.0, 180.03f),
	glm::vec3(-168.125f, 10.0f, 230.03f),
	glm::vec3(-120.125f, 2.0f, 240.03f),
	glm::vec3(-145.125f, 4.0f, 250.03f),
	glm::vec3(-130.125f, 10.0f, 90.03f),
	glm::vec3(-120.125f, 0.0f, 110.03f),
	glm::vec3(-150.125f, 5.0f, 280.03f),
	glm::vec3(-168.125f, 10.0f, 290.03f),
	glm::vec3(-170.125f, 0.0f, 300.03f),
	glm::vec3(-180.125f, 7.0f, 310.03f),
	glm::vec3(-155.125f, 0.0f, 150.03f),
	glm::vec3(-170.125f, 2.0f, 140.03f),
	glm::vec3(-180.125f, 4.0f, 300.03f),
	glm::vec3(-115.125f, 4.0f, 160.03f),
	glm::vec3(-140.125f, 0.0, 340.03f),
	glm::vec3(-210.125f, 6.0, 80.03f),
	glm::vec3(-1.125f, 10.0f, 230.03f),
	glm::vec3(-125.125f, 0.0f, 190.03f),
	glm::vec3(-140.125f, -2.0f, 170.03f),
	glm::vec3(-115.125f, -2.0f, 190.03f),
	glm::vec3(-125.125f, -2.0f, 150.03f),
	glm::vec3(-150.125f, -2.0f, 100.03f),
	glm::vec3(-180.125f, -2.0f, 320.03f),
	glm::vec3(-200.125f, -2.0f, 120.03f),
	glm::vec3(-150.125f, -2.0f, 330.03f),
};

//posiciones en forma de rupia de las monedas

glm::vec3 coinsPosRupia[]{
	glm::vec3(-157.77f, 11.82f, 103.243f),
	glm::vec3(-155.735f,9.55077f,103.243f),
	glm::vec3(-154.026f, 7.71638f, 103.243f),
	glm::vec3(-152.383f, 5.85664f, 103.243f),
	glm::vec3(-151.705f, 3.65661f, 103.243f),
	glm::vec3(-151.705f, 1.24527f, 103.243f),
	glm::vec3(-151.705f, -1.30285f, 103.243f),
	glm::vec3(-151.705f, -3.8088f, 103.243f),
	glm::vec3(-152.268f, -6.15162f, 103.243f),
	glm::vec3(-153.957f, -7.98601f, 103.243f),
	glm::vec3(-155.581f, -9.84576f, 103.243f),
	glm::vec3(-157.774f, -11.182f, 103.243f),
	glm::vec3(-159.675f, -9.84576f, 103.243f),
	glm::vec3(-161.317f, -7.98601f, 103.243f),
	glm::vec3(-163.026f, -6.15162f, 103.243f),
	glm::vec3(-163.842f, -3.80877f, 103.243f),
	glm::vec3(-163.842f, -1.30285f, 103.243f),
	glm::vec3(-163.842f, 1.24527f, 103.243f),
	glm::vec3(-163.842f, 3.65661f, 103.243f),
	glm::vec3(-162.917f, 5.85664f, 103.243f),
	glm::vec3(-161.365f, 7.71638f, 103.243f),
	glm::vec3(-159.75f, 9.55077f, 103.243f),
	glm::vec3(-157.774f, 4.67077f , 103.243f),
	glm::vec3(-155.65f, 2.78606f , 103.243f),
	glm::vec3(-155.65f, 0.290141f , 103.243f),
	glm::vec3(-155.65f, -2.37915f, 103.243f),
	glm::vec3(-157.774f, -4.25789f, 103.243f),
	glm::vec3(-159.898f,-2.37915f , 103.243f),
	glm::vec3(-159.898f, 0.290141f , 103.243f),
	glm::vec3(-159.898f,2.86371f , 103.243f),
};





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



/*
* Variables para la animacion de Mario
* animMario: indica si la animacion de Mario esta activa
* marioState: indica el estado de la animacion de Mario
* vueltasMario: cuenta las vueltas que ha dado Mario en la animacion
* marioRotation: angulo de rotacion de Mario en la animacion
* marioHeight: altura de Mario en la animacion
* marioPos: posicion de Mario
* modelTemp: matriz temporal para guardar la posicion inicial de Mario
*/
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
bool AnimMario = false;
glm::vec3 marioPos(-152.0f, -6.42124f, 387.647f);
glm::mat4 modelTemp = glm::mat4(1.0f);


/*
* Variables para la animacion del sol
* shineRotateAngle: angulo de rotacion del sol
* shineRotate: indica si la rotacion del sol esta activa
* shineUp: indica si el sol esta subiendo
* shineSpinAngle: angulo de spin del sol
*/
float shineRotateAngle = 10.0f;
bool shineRotate = true;
bool shineUp = false;
float shineOrbitAngle = 0.0f;      
bool shineOrbitActive = false;    
float shineSpinAngle = 1.0f;   

/*
* Variables para la animacion del perro
* rotDog  : Rotación del perro (0-360°)
  • dogPosX : Posición en X
  • dogPosY : Posición en Y (altura)
  • dogPosZ : Posición en Z (avance)
  • head    : Rotación cabeza
  • FDLegs  : Pierna Frontal Derecha
  • FILegs  : Pierna Frontal Izquierda
  • TDLegs  : Pierna Trasera Derecha
  • TILegs  : Pierna Trasera Izquierda
  • body    : Rotación del cuerpo
  • tail    : Rotación de la cola
*/
float rotDog = 180.0f;
int dogAnim = 0;
float FDLegs = 0.0f;
float FILegs = 0.0f;
float TDLegs = 0.0f;
float TILegs = 0.0f;
float head = 0.0f;
float body = 0.0f;
float tail = 0.0f;

//KeyFrames
float dogPosX = 230.0f;
float dogPosY = -20.0f;
float dogPosZ = 506.0f;

#define MAX_FRAMES 100
int i_max_steps = 190;
int i_curr_steps = 0;

typedef struct _frame {

	float rotDog;
	float rotDogInc;
	float dogPosX;
	float dogPosY;
	float dogPosZ;
	float incX;
	float incY;
	float incZ;
	float FDLegs;
	float FDLegsInc;
	float FILegs;
	float FILegsInc;
	float TDLegs;
	float TDLegsInc;
	float TILegs;
	float TILegsInc;
	float head;
	float headInc;
	float body;
	float bodyInc;
	float tail;
	float tailInc;


}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].dogPosX = dogPosX;
	KeyFrame[FrameIndex].dogPosY = dogPosY;
	KeyFrame[FrameIndex].dogPosZ = dogPosZ;

	KeyFrame[FrameIndex].rotDog = rotDog;
	KeyFrame[FrameIndex].head = head;

	KeyFrame[FrameIndex].FDLegs = FDLegs;
	KeyFrame[FrameIndex].FILegs = FILegs;
	KeyFrame[FrameIndex].TDLegs = TDLegs;
	KeyFrame[FrameIndex].TILegs = TILegs;
	KeyFrame[FrameIndex].body = body;
	KeyFrame[FrameIndex].tail = tail;


	FrameIndex++;
}

void resetElements(void)
{
	dogPosX = KeyFrame[0].dogPosX;
	dogPosY = KeyFrame[0].dogPosY;
	dogPosZ = KeyFrame[0].dogPosZ;
	head = KeyFrame[0].head;

	rotDog = KeyFrame[0].rotDog;

	FDLegs = KeyFrame[0].FDLegs;
	FILegs = KeyFrame[0].FILegs;
	TDLegs = KeyFrame[0].TDLegs;
	TILegs = KeyFrame[0].TILegs;
	body = KeyFrame[0].body;
	tail = KeyFrame[0].tail;

}
void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].dogPosX - KeyFrame[playIndex].dogPosX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].dogPosY - KeyFrame[playIndex].dogPosY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].dogPosZ - KeyFrame[playIndex].dogPosZ) / i_max_steps;
	KeyFrame[playIndex].headInc = (KeyFrame[playIndex + 1].head - KeyFrame[playIndex].head) / i_max_steps;

	KeyFrame[playIndex].rotDogInc = (KeyFrame[playIndex + 1].rotDog - KeyFrame[playIndex].rotDog) / i_max_steps;

	KeyFrame[playIndex].FDLegsInc = (KeyFrame[playIndex + 1].FDLegs - KeyFrame[playIndex].FDLegs) / i_max_steps;
	KeyFrame[playIndex].FILegsInc = (KeyFrame[playIndex + 1].FILegs - KeyFrame[playIndex].FILegs) / i_max_steps;
	KeyFrame[playIndex].TDLegsInc = (KeyFrame[playIndex + 1].TDLegs - KeyFrame[playIndex].TDLegs) / i_max_steps;
	KeyFrame[playIndex].TILegsInc = (KeyFrame[playIndex + 1].TILegs - KeyFrame[playIndex].TILegs) / i_max_steps;
	KeyFrame[playIndex].bodyInc = (KeyFrame[playIndex + 1].body - KeyFrame[playIndex].body) / i_max_steps;
	KeyFrame[playIndex].tailInc = (KeyFrame[playIndex + 1].tail - KeyFrame[playIndex].tail) / i_max_steps;
}


//Variables para la animacion de la recepcionista
glm::vec3 recepPos(0.0f, 0.0f, 0.0f);
glm::mat4 recepModelTemp = glm::mat4(1.0f);
float recepRot = 0.0f;
// Animación por pieza
float angCabeza = 0.0f;
float angHombroDer = 0.0f;
float angHombroIzq = 0.0f;
float angAntebrazoDer = 0.0f;
float angAntebrazoIzq = 0.0f;
float angManoDer = 0.0f;
float angManoIzq = 0.0f;
float angPiernaDer = 0.0f;
float angPiernaIzq = 0.0f;
float angTibiaDer = 0.0f;
float angTibiaIzq = 0.0f;
float angPieDer = 0.0f;
float angPieIzq = 0.0f;
bool AnimRecep = false;
int recepState = 0;        // estado de la animación
float recepWalkDist = 0.0f;
bool stepRecep = false;    // para animación de piernas


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
	Shader skyboxShader("Shader/SkyBox.vs","Shader/SkyBox.frag");
	
	// Carga de modelos
	Model Proyecto((char*)"models/proyecto.obj");
	
	
	//Modelos de Mario
	Model Shine((char*)"models/monedaMario.obj");
	Model MarioBody((char*)"models/body_mario.obj");
	Model HeadMario((char*)"models/head_mario.obj");
	Model RightLeg((char*)"models/R_leg_mario.obj");
	Model LeftLeg((char*)"models/L_leg_mario.obj");
	Model RightArm((char*)"models/R_arm_mario.obj");
	Model LeftArm((char*)"models/L_arm_mario.obj");
	Model Coin((char*)"models/coin.obj");

	//Modelo de Recepcionista
	Model RecepcionistaCuerpo((char*)"models/Cuerpo.obj");
	Model RecepcionistaCabeza((char*)"models/Cabeza.obj");
	Model RecepcionistaHombroDerecho((char*)"models/HombroDer.obj");
	Model RecepcionistaHombroIzquierdo((char*)"models/HombroIzq.obj");
	Model RecepcionistaAntebrazoDerecho((char*)"models/AntebrazoDer.obj");
	Model RecepcionistaAntebrazoIzquierdo((char*)"models/AntebrazoIzq.obj");
	Model RecepcionistaManoDerecha((char*)"models/ManoDer.obj");
	Model RecepcionistaManoIzquierda((char*)"models/ManoIzq.obj");
	Model RecepcionistaPiernaDerecha((char*)"models/PiernaDer.obj");
	Model RecepcionistaPiernaIzquierda((char*)"models/PiernaIzq.obj");
	Model RecepcionistaTibiaDerecha((char*)"models/TibiaDer.obj");
	Model RecepcionistaTibiaIzquierda((char*)"models/TibiaIzq.obj");
	//Model RecepcionistaPies((char*)"models/Pies.obj");
	Model RecepcionistaPieDerecho((char*)"models/PieDer.obj");
	Model RecepcionistaPieIzquierdo((char*)"models/PieIzq.obj");

	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
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
		32,33,34,35
	};


	//Modelos de Perro
	Model DogBody((char*)"models/PerroTorso.obj");
	Model HeadDog((char*)"models/PerroCabeza.obj");
	Model DogTail((char*)"models/Cola.obj");
	Model F_RightLeg((char*)"models/PataFroder.obj");
	Model F_LeftLeg((char*)"models/PataFroizq.obj");
	Model B_RightLeg((char*)"models/PataTrader.obj");
	Model B_LeftLeg((char*)"models/PataTraizq.obj");

	//KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		if (i == 0) 
		{
			KeyFrame[i].dogPosX = 230.0;
			KeyFrame[i].dogPosY = 20.0;
			KeyFrame[i].dogPosZ = -492.0;
		}
		else
		{
			KeyFrame[i].dogPosX = 0;
			KeyFrame[i].dogPosY = 0;
			KeyFrame[i].dogPosZ = 0;
		}
		
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotDog = 0;
		KeyFrame[i].rotDogInc = 0;
		KeyFrame[i].head = 0;
		KeyFrame[i].headInc = 0;
		KeyFrame[i].FDLegs = 0;
		KeyFrame[i].FDLegsInc = 0;
		KeyFrame[i].FILegs = 0;
		KeyFrame[i].FILegsInc = 0;
		KeyFrame[i].TDLegs = 0;
		KeyFrame[i].TDLegsInc = 0;
		KeyFrame[i].TILegs = 0;
		KeyFrame[i].TILegsInc = 0;
		KeyFrame[i].body = 0;
		KeyFrame[i].bodyInc = 0;
		KeyFrame[i].tail = 0;
		KeyFrame[i].tailInc = 0;
	}

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	//Skybox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1,&skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	//Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right.jpg");
	faces.push_back("SkyBox/left.jpg");
	faces.push_back("SkyBox/top.jpg");
	faces.push_back("SkyBox/bottom.jpg");
	faces.push_back("SkyBox/back.jpg");
	faces.push_back("SkyBox/front.jpg");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 300.0f);


	/*****GENERACION DE LA POSICION DE LUCES PUNTUALES PARA LAS PAREDES*****/

	const int NUM_PIVOTS = sizeof(lightPivots) / sizeof(lightPivots[0]);
	//posiciones de offset en Y para las luces de las paredes
	float yOffsets[] = {
	 0.0f, 3.0f, 6.0f, 9.0f, 12.0f, 16.0f, 23.0f, 26.0f, 29.0f, 32.0f, 35.0f, 38.0f, 41.0f, -3.0f, -6.0f, -9.0f, -12.0f, -15.0f
	};
	//calculo de las posiciones de las luces puntuales
	const int NUM_OFFSETS = sizeof(yOffsets) / sizeof(yOffsets[0]);

	// Generar y almacenar todas las posiciones de luz en el vector global
	for (int i = 0; i < NUM_PIVOTS; ++i) {
		glm::vec3 pivot = lightPivots[i];
		for (int j = 0; j < NUM_OFFSETS; ++j) {
			glm::vec3 finalPosition = glm::vec3(
				pivot.x,
				yOffsets[j],
				pivot.z
			);

			// Almacena la posici�n final 
			pointLightPositions.push_back(finalPosition);
		}
	}


	/*****GENERACI�N DE LAS POSICIONES DE LUCES PUNTUALES PARA EL TECHO*****/
	//posiciones de offset en Z para las luces del techo
	float zOffsetsRoof[] = {
		 0.0f, 5.0f, 10.0f, 20.0f, 25.0f, 30.0f, 35.0f, 40.0f, 45.0f, 50.0f, 55.0f, 60.0f, 65.0f, -5.0f, -10.0f, -15.0f, -20.0f, -25.0f,
		  -30.0f, -35.0f, -40.0f, -45.0f, -50.0f, -55.0f, -60.0f, -65.0f
	};
	// Calcular el n�mero de offsets
	const int NUM_Z_OFFSETS = sizeof(zOffsetsRoof) / sizeof(zOffsetsRoof[0]);

	// Color de las luces del techo (morado)
	glm::vec3 roofLightColor = glm::vec3(1.0f, 0.6f, 1.0f); 

	// Generar y almacenar todas las posiciones de luz en el techo
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

	// Agregar las posiciones del techo al vector principal de posiciones de luces puntuales
	for (int i = 0; i < pointLightpivotsRoof.size(); ++i) {
		pointLightPositions.push_back(pointLightpivotsRoof[i]);
	}


	// Calcula numLights a partir del vector despu�s de que ha sido llenado
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

		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp


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


		// Numero de luces por cada pivote en las paredes
		const int LIGHTS_PER_PIVOT = 18; 
		int numColors = diffuseColors.size();
		glm::vec3 currentDiffuseColor;

		for (int i = 0; i < numLights; ++i) {
			std::string base = "pointLights[" + std::to_string(i) + "]";


			//Calcular el indice del pivote cada 18 puntos es un pivote
			int pivotIndex = i / LIGHTS_PER_PIVOT;

			// Calcular el �ndice del PAR de pivotes
			int pairIndex = pivotIndex / 2;

			// Seleccionar el color de la luz en base al n�mero de luces creadas hasta el momento
			// Si es mayor o igual a 216 se dibujan las luces del techo, usar color morado
			// Si no, alternar entre los colores definidos para las paredes
			if (i >= 216) {
				currentDiffuseColor = glm::vec3(1.0f, 0.0f, 1.0f);
			}

			else {
				currentDiffuseColor = diffuseColors[pairIndex % numColors];
			}
			

			// Dar la posicion de la luz y sus configuraciones
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".position").c_str()),
				pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);

			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".ambient").c_str()),
				currentDiffuseColor.x * 0.05f, currentDiffuseColor.y * 0.05f, currentDiffuseColor.z * 0.05f);

			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".diffuse").c_str()),
				currentDiffuseColor.x, currentDiffuseColor.y, currentDiffuseColor.z);

			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".specular").c_str()),
				currentDiffuseColor.x * 0.5f, currentDiffuseColor.y * 0.5f, currentDiffuseColor.z * 0.5f);

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

	

		//Dibujo de modelo del escenario
        view = camera.GetViewMatrix();	
		model = glm::mat4(1);
		//glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
	    Proyecto.Draw(lightingShader);

		/*
		* Dibujo del modelo del sol y movimiento apartir de la posicion de mario
		*/
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



		/*
		* Dibujo del modelo de Mario dividido en partes por jerarquia
		*/
		glm::mat4 modelMario = glm::mat4(1.0f);
		modelMario = glm::translate(modelMario, marioPos);
		modelMario = glm::rotate(modelMario, glm::radians(marioRotation), glm::vec3(0.0f, 1.0f, 0.0f));
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


		//Recepcionista
		glm::mat4 recepModel = recepModelTemp;
		recepModel = glm::translate(recepModel, recepPos);
		recepModel = glm::rotate(recepModel, glm::radians(recepRot), glm::vec3(0, 1, 0));

		// ---------- CUERPO ----------
		model = recepModel;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RecepcionistaCuerpo.Draw(lightingShader);

		// ---------- CABEZA ----------
		model = recepModel;
		model = glm::rotate(model, glm::radians(angCabeza), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RecepcionistaCabeza.Draw(lightingShader);

		// ---------- HOMBRO DERECHO ----------
		model = recepModel;
		model = glm::rotate(model, glm::radians(angHombroDer), glm::vec3(1, 0, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RecepcionistaHombroDerecho.Draw(lightingShader);

		// ---------- ANTEBRAZO DERECHO ----------
		model = recepModel;
		model = glm::rotate(model, glm::radians(angAntebrazoDer), glm::vec3(1, 0, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RecepcionistaAntebrazoDerecho.Draw(lightingShader);

		// ---------- MANO DERECHA ----------
		model = recepModel;
		model = glm::rotate(model, glm::radians(angManoDer), glm::vec3(1, 0, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RecepcionistaManoDerecha.Draw(lightingShader);

		// ---------- HOMBRO IZQUIERDO ----------
		model = recepModel;
		model = glm::rotate(model, glm::radians(angHombroIzq), glm::vec3(1, 0, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RecepcionistaHombroIzquierdo.Draw(lightingShader);

		// ---------- ANTEBRAZO IZQUIERDO ----------
		model = recepModel;
		model = glm::rotate(model, glm::radians(angAntebrazoIzq), glm::vec3(1, 0, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RecepcionistaAntebrazoIzquierdo.Draw(lightingShader);

		// ---------- MANO IZQUIERDA ----------
		model = recepModel;
		model = glm::rotate(model, glm::radians(angManoIzq), glm::vec3(1, 0, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RecepcionistaManoIzquierda.Draw(lightingShader);

		// ---------- PIERNA DERECHA ----------
		model = recepModel;
		model = glm::rotate(model, glm::radians(angPiernaDer), glm::vec3(1, 0, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RecepcionistaPiernaDerecha.Draw(lightingShader);

		// ---------- TIBIA DERECHA ----------
		model = recepModel;
		model = glm::rotate(model, glm::radians(angTibiaDer), glm::vec3(1, 0, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RecepcionistaTibiaDerecha.Draw(lightingShader);

		// ---------- PIERNA IZQUIERDA ----------
		model = recepModel;
		model = glm::rotate(model, glm::radians(angPiernaIzq), glm::vec3(1, 0, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RecepcionistaPiernaIzquierda.Draw(lightingShader);

		// ---------- TIBIA IZQUIERDA ----------
		model = recepModel;
		model = glm::rotate(model, glm::radians(angTibiaIzq), glm::vec3(1, 0, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RecepcionistaTibiaIzquierda.Draw(lightingShader);

		//// ---------- PIES ----------
		//model = recepModel;
		//model = glm::rotate(model, glm::radians(angPies), glm::vec3(1, 0, 0));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//RecepcionistaPies.Draw(lightingShader);

		// PIE DERECHO
		model = recepModel;
		model = glm::rotate(model, glm::radians(angTibiaDer), glm::vec3(1, 0, 0)); // sigue a la tibia
		model = glm::rotate(model, glm::radians(angPieDer), glm::vec3(1, 0, 0));   // movimiento propio del pie
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RecepcionistaPieDerecho.Draw(lightingShader);

		// PIE IZQUIERDO
		model = recepModel;
		model = glm::rotate(model, glm::radians(angTibiaIzq), glm::vec3(1, 0, 0)); // sigue a la tibia
		model = glm::rotate(model, glm::radians(angPieIzq), glm::vec3(1, 0, 0));   // movimiento propio del pie
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RecepcionistaPieIzquierdo.Draw(lightingShader);


		

		//Dibujo de las monedas con animacion de subida y bajada
		for (int i = 0; i < sizeof(coinsPos) / sizeof(coinsPos[0]); i++) {
			glm::mat4 model = glm::mat4(1.0f);
			if(animateCoins==false)
			{
				if (i % 2 == 0) {
					coinYOffset = sin(glfwGetTime() * 2.0f) * 1.0f;
				}
				else {
					coinYOffset = cos(glfwGetTime() * 2.0f) * 2.0f;
				}
			}
			glm::vec3 currentPos = glm::mix(coinsPos[i], coinsPosRupia[i], animationProgress);
			model = glm::translate(model, glm::vec3(currentPos.x, currentPos.y + coinYOffset, currentPos.z));
			model = glm::rotate(model, glm::radians(coinRotateY), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Coin.Draw(lightingShader);
		}

		/*
		* Dibujo del modelo del Perro dividido en partes por jerarquia
		*/

		//static int frameCount = 0;
		//if (frameCount % 60 == 0) {  // Imprimir cada 60 frames
		//	printf("DEBUG PERRO - X: %.2f, Y: %.2f, Z: %.2f\n", dogPosX, dogPosY, dogPosZ);
		//}
		//frameCount++;

		model = glm::mat4(1);
		
		// Aplicar transformaciones base
		model = glm::translate(model, glm::vec3(dogPosX, dogPosY, dogPosZ));
		model = glm::rotate(model, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.913f, 3.095f, 3.159f));

		// GUARDAR modelTemp DESPUÉS de aplicar escala
		modelTemp = model;

		//Body
		model = modelTemp;
		model = glm::rotate(model, glm::radians(body), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogBody.Draw(lightingShader);

		//Head
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HeadDog.Draw(lightingShader);

		//Tail 
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogTail.Draw(lightingShader);

		//Front Left Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(FILegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_LeftLeg.Draw(lightingShader);

		//Front Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(FDLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_RightLeg.Draw(lightingShader);

		//Back Left Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(TILegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_LeftLeg.Draw(lightingShader);

		//Back Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(TDLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_RightLeg.Draw(lightingShader);

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


		// Dibuja lamparas horizontales del techo
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

		//Draw SkyBox
		glDepthFunc(GL_LEQUAL);
		skyboxShader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVAO);

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

	if (keys[GLFW_KEY_1])
	{
		dogPosZ += 1.0;
	}

	if (keys[GLFW_KEY_2])
	{
		dogPosZ -= 1.0;
	}

	if (keys[GLFW_KEY_3])
	{
		dogPosX -= 1.0;
	}

	if (keys[GLFW_KEY_4])
	{
		dogPosX += 1.0;
	}

	if (keys[GLFW_KEY_5])
	{
		dogPosY += 1.0;
	}

	if (keys[GLFW_KEY_6])
	{
		dogPosY -= 1.0;
	}

	if (keys[GLFW_KEY_7])
	{
		rotDog += 1.0f;
	}

	if (keys[GLFW_KEY_8])
	{
		rotDog -= 1.0f;
	}

	if (keys[GLFW_KEY_9])
	{
		printf("===== POSICION DEL PERRO =====\n");
		printf("X = %.2f\n", dogPosX);
		printf("Y = %.2f\n", dogPosY);
		printf("Z = %.2f\n", dogPosZ);
		printf("rotDog = %.2f\n", rotDog);
		printf("==============================\n");
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

	if (keys[GLFW_KEY_R])
	{
		AnimRecep = true;
		recepState = 0;
		recepWalkDist = 0.0f;
	}


	if (keys[GLFW_KEY_N])
	{
		AnimMario = !AnimMario;
	}

	if (keys[GLFW_KEY_M])
	{
		animateCoins = true;
		animationProgress = 0.0f;
		animationDirectionCoins = 1;
	}

	if (keys[GLFW_KEY_Z])
	{
		animateCoins = true;
		animationDirectionCoins = -1;
	}

	if (keys[GLFW_KEY_P])
	{
		std::cout << "\n******************************************" << std::endl;
		std::cout << "TECLA P PRESIONADA" << std::endl;
		std::cout << "******************************************" << std::endl;

		//reinicio de los valores de los keyframes
		std::cout << "Limpiando keyframes anteriores..." << std::endl;
		for (int i = 0; i < MAX_FRAMES; i++)
		{
			KeyFrame[i].dogPosX = 0;
			KeyFrame[i].dogPosY = 0;
			KeyFrame[i].dogPosZ = 0;
			KeyFrame[i].incX = 0;
			KeyFrame[i].incY = 0;
			KeyFrame[i].incZ = 0;
			KeyFrame[i].rotDog = 0;
			KeyFrame[i].rotDogInc = 0;
			KeyFrame[i].head = 0;
			KeyFrame[i].headInc = 0;
			KeyFrame[i].FDLegs = 0;
			KeyFrame[i].FDLegsInc = 0;
			KeyFrame[i].FILegs = 0;
			KeyFrame[i].FILegsInc = 0;
			KeyFrame[i].TDLegs = 0;
			KeyFrame[i].TDLegsInc = 0;
			KeyFrame[i].TILegs = 0;
			KeyFrame[i].TILegsInc = 0;
			KeyFrame[i].body = 0;
			KeyFrame[i].bodyInc = 0;
			KeyFrame[i].tail = 0;
			KeyFrame[i].tailInc = 0;
		}
		std::cout << "Keyframes limpiados correctamente" << std::endl;

		//carga de valores desde el archivo
		cargaFrames("frames.txt");

		//inicio de la animacion
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "\n******************************************" << std::endl;
			std::cout << "INICIANDO ANIMACION" << std::endl;
			std::cout << "******************************************" << std::endl;
			std::cout << "Frames cargados: " << FrameIndex << std::endl;
			std::cout << "Posicion inicial: (" << KeyFrame[0].dogPosX << ", "
				<< KeyFrame[0].dogPosY << ", " << KeyFrame[0].dogPosZ << ")" << std::endl;
			std::cout << "Posicion final: (" << KeyFrame[FrameIndex - 1].dogPosX << ", "
				<< KeyFrame[FrameIndex - 1].dogPosY << ", " << KeyFrame[FrameIndex - 1].dogPosZ << ")" << std::endl;
			std::cout << "******************************************\n" << std::endl;

			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}

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
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			dogPosX += KeyFrame[playIndex].incX;
			dogPosY += KeyFrame[playIndex].incY;
			dogPosZ += KeyFrame[playIndex].incZ;
			rotDog += KeyFrame[playIndex].rotDogInc;
			head += KeyFrame[playIndex].headInc;
			FDLegs += KeyFrame[playIndex].FDLegsInc;
			FILegs += KeyFrame[playIndex].FILegsInc;
			TDLegs += KeyFrame[playIndex].TDLegsInc;
			TILegs += KeyFrame[playIndex].TILegsInc;
			body += KeyFrame[playIndex].bodyInc;
			tail += KeyFrame[playIndex].tailInc;
			i_curr_steps++;
		}

	}

	// ---------- ANIMACIÓN DE LA RECEPCIONISTA ----------
	if (AnimRecep)
	{
		switch (recepState)
		{
			// 1. Giro hacia la izquierda 135°
		case 0:
			recepRot += 1.0f;
			if (recepRot >= 135.0f) {
				recepRot = 135.0f;
				recepWalkDist = 0.0f;
				recepState = 1;
			}
			break;

			// 2. Camina 50 unidades
		case 1:
			recepPos.x += sin(glm::radians(recepRot)) * 0.05f;
			recepPos.z -= cos(glm::radians(recepRot)) * 0.05f;
			recepWalkDist += 0.05f;

			// animación piernas
			if (!stepRecep) {
				angPiernaDer += 1.0f;
				angTibiaDer += 1.0f;
				angPiernaIzq -= 1.0f;
				angTibiaIzq -= 1.0f;
				if (angPiernaDer > 20.0f) stepRecep = true;
			}
			else {
				angPiernaDer -= 1.0f;
				angTibiaDer -= 1.0f;
				angPiernaIzq += 1.0f;
				angTibiaIzq += 1.0f;
				if (angPiernaDer < -20.0f) stepRecep = false;
			}

			if (recepWalkDist >= 50.0f) {
				recepWalkDist = 0.0f;
				recepState = 2;
			}
			break;

			// 3. Giro 90° a la derecha
		case 2:
			recepRot -= 1.0f;
			if (recepRot <= 45.0f) { // 135 - 90 = 45
				recepRot = 45.0f;
				recepWalkDist = 0.0f;
				recepState = 3;
			}
			break;

			// 4. Camina 40 unidades
		case 3:
			recepPos.x += sin(glm::radians(recepRot)) * 0.05f;
			recepPos.z -= cos(glm::radians(recepRot)) * 0.05f;
			recepWalkDist += 0.05f;

			if (!stepRecep) {
				angPiernaDer += 1.0f;
				angTibiaDer += 1.0f;
				angPiernaIzq -= 1.0f;
				angTibiaIzq -= 1.0f;
				if (angPiernaDer > 20.0f) stepRecep = true;
			}
			else {
				angPiernaDer -= 1.0f;
				angTibiaDer -= 1.0f;
				angPiernaIzq += 1.0f;
				angTibiaIzq += 1.0f;
				if (angPiernaDer < -20.0f) stepRecep = false;
			}

			if (recepWalkDist >= 40.0f) {
				recepWalkDist = 0.0f;
				recepState = 4;
			}
			break;

			// 5. Giro 90° derecha otra vez
		case 4:
			recepRot -= 1.0f;
			if (recepRot <= -45.0f) {  // 45 - 90 = -45
				recepRot = -45.0f;
				recepWalkDist = 0.0f;
				recepState = 5;
			}
			break;

			// 6. Camina 150 unidades
		case 5:
			recepPos.x += sin(glm::radians(recepRot)) * 0.05f;
			recepPos.z -= cos(glm::radians(recepRot)) * 0.05f;
			recepWalkDist += 0.05f;

			if (!stepRecep) {
				angPiernaDer += 1.0f;
				angTibiaDer += 1.0f;
				angPiernaIzq -= 1.0f;
				angTibiaIzq -= 1.0f;
				if (angPiernaDer > 20.0f) stepRecep = true;
			}
			else {
				angPiernaDer -= 1.0f;
				angTibiaDer -= 1.0f;
				angPiernaIzq += 1.0f;
				angTibiaIzq += 1.0f;
				if (angPiernaDer < -20.0f) stepRecep = false;
			}

			if (recepWalkDist >= 150.0f) {
				recepState = 6; // Termina
			}
			break;

			// Animación terminada
		case 6:
			break;
		}
	}

	//animacion de las monedas subida y bajada
	if (coinAnim) {
		//estado 0 de la animacion de las monedas 
		coinRotateY += 2.0f;
		//estado 1 de la animacion de las monedas, la moneda sube
		if (coinYOffset >= 5.0f) {
			coinUp = false;
		}
		//estado 2 de la animacion de las monedas, la moneda baja
		else if (coinYOffset <= -0.0f) {
			coinUp = true;
		}
		//movimiento de la moneda en Y
		if (coinUp) {
			coinYOffset += 0.1f;
		}
		else {
			coinYOffset -= 0.1f;
		}
	}

	if (animateCoins) {

		animationProgress += deltaTime * animationSpeed * animationDirectionCoins;
		//estado 3 de la animacion de las monedas las monedas se mueven hasta formar la rupia
		if (animationProgress >= 1.0f and animationDirectionCoins == 1) {
			animationProgress = 1.0f;
			coinYOffset = 0.0f;
		}

		//estado 4 de la animacion de las monedas las monedas vuelven a su posicion inicial
		if (animationDirectionCoins == -1 && animationProgress <= 0.0f) {
			animationProgress = 0.0f;
		}
	}

	if (!AnimMario)
		return;

	if (shineRotate) {
		shineSpinAngle += 45.0f;
	}
	
	
	switch (marioState)
	{
		//estado 0 de la animacion de mario sube
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
		//estado 1 de la animacion de mario levanta el brazo en X
	case 1:
		if (armMarioAngleX<90.0f) {
			armMarioAngleX += 10.0f;
		}
		else {
			marioState = 2;
		}
		break;
		//estado 2 de la animacion de mario gira el brazo en Y
	case 2:
		if (armMarioAngleY < 90.0f) {
			armMarioAngleY += 10.0f;
		}
		else {
			marioState = 3;
		}
		break;
		//estado 3 de la animacion de mario gira alrededor de su eje Y
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

void cargaFrames(string filename)
{
	//reinicio de los elementos de la animación
	resetElements();
	playIndex = 0;
	i_curr_steps = 0;
	FrameIndex = 0;

	//abrimos el archivo
	std::ifstream file(filename);
	//verificamos que se haya abierto correctamente
	if (!file.is_open()) {
		std::cout << "No se pudo abrir el archivo de frames: " << filename << std::endl;
		return;
	}

	std::cout << "Archivo abierto correctamente " << std::endl;


	//variables para la lectura
	std::string line;
	int lineNumber = 0;

	//mientras haya líneas en el archivo
	while (std::getline(file, line))
	{
		lineNumber++;

		if (line.empty()) continue;

		// Omitir comentarios
		if (line[0] == '#') continue;
		if (line.size() >= 2 && line[0] == '/' && line[1] == '/') continue;

		// Limpiar el frame actual antes de asignar (evita "basura" previa)
		KeyFrame[FrameIndex].rotDog = 0.0f;
		KeyFrame[FrameIndex].dogPosX = 0.0f;
		KeyFrame[FrameIndex].dogPosY = 0.0f;
		KeyFrame[FrameIndex].dogPosZ = 0.0f;
		KeyFrame[FrameIndex].head = 0.0f;
		KeyFrame[FrameIndex].FDLegs = 0.0f;
		KeyFrame[FrameIndex].FILegs = 0.0f;
		KeyFrame[FrameIndex].TDLegs = 0.0f;
		KeyFrame[FrameIndex].TILegs = 0.0f;
		KeyFrame[FrameIndex].body = 0.0f;
		KeyFrame[FrameIndex].tail = 0.0f;

		/*
		* separamos la linea en tokens clave-valor
		*/
		std::istringstream token(line);
		std::string key;
		char equal;
		float value;

		/*
		Asignar los valores de la línea al keyframe actual
		*/
		while (token >> key >> equal >> value)
		{
			if (key == "rotDog") KeyFrame[FrameIndex].rotDog = value;
			else if (key == "dogPosX") KeyFrame[FrameIndex].dogPosX = value;
			else if (key == "dogPosY") KeyFrame[FrameIndex].dogPosY = value;
			else if (key == "dogPosZ") KeyFrame[FrameIndex].dogPosZ = value;
			else if (key == "head") KeyFrame[FrameIndex].head = value;
			else if (key == "FDLegs") KeyFrame[FrameIndex].FDLegs = value;
			else if (key == "FILegs") KeyFrame[FrameIndex].FILegs = value;
			else if (key == "TDLegs") KeyFrame[FrameIndex].TDLegs = value;
			else if (key == "TILegs") KeyFrame[FrameIndex].TILegs = value;
			else if (key == "body") KeyFrame[FrameIndex].body = value;
			else if (key == "tail") KeyFrame[FrameIndex].tail = value;
		}

		//Mostrar el frame cargado
		std::cout << "Frame #" << FrameIndex << " (Linea " << lineNumber << "):" << std::endl;
		std::cout << "  Posicion: (" << KeyFrame[FrameIndex].dogPosX << ", "
			<< KeyFrame[FrameIndex].dogPosY << ", "
			<< KeyFrame[FrameIndex].dogPosZ << ")" << std::endl;
		std::cout << "  Rotacion: " << KeyFrame[FrameIndex].rotDog << std::endl;
		std::cout << "  Cabeza: " << KeyFrame[FrameIndex].head << " | Cola: "
			<< KeyFrame[FrameIndex].tail << std::endl;
		std::cout << "  Patas F: [D:" << KeyFrame[FrameIndex].FDLegs << " I:"
			<< KeyFrame[FrameIndex].FILegs << "]" << std::endl;
		std::cout << "  Patas T: [D:" << KeyFrame[FrameIndex].TDLegs << " I:"
			<< KeyFrame[FrameIndex].TILegs << "]" << std::endl;
		std::cout << std::endl;
		//incrementamos el índice del frame
		FrameIndex++;
	}

	file.close();

	std::cout << "========================================" << std::endl;
	std::cout << "CARGA COMPLETADA" << std::endl;
	std::cout << "Total de frames cargados: " << FrameIndex << std::endl;
	std::cout << "Frames validos para animacion: " << (FrameIndex > 1 ? FrameIndex - 1 : 0) << " transiciones" << std::endl;
	std::cout << "Pasos por transicion: " << i_max_steps << std::endl;
	std::cout << "========================================\n" << std::endl;
}