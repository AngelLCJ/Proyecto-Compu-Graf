//Proyecto Final Teoria
//Becerril Olivar Axel Daniel
//Rendon Hernandez Roberto Carlos
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float movCocheX;
float movCocheZ;
float movTriX;
float movTriZ;
float movOffset; //bandera para detener el carro 
float movOffset2;
float rotllanta;
float rotllantaOffset; //bandera para detener el carro 
bool avanzaX; //indica si avanza o esta detenido el coche
bool avanzaZ;
bool avanzaX2; //indica si avanza o esta detenido el coche
bool avanzaZ2;
float rotacion2;
int numGiros2;
float rotacion;
int numGiros;

//evolucion
bool transformacion = true;
int tiempo = 0;
int evolucion = 0;

//Variables para la ilumincaión
bool luzdia;
bool luznoche;
float luzgradual = 1.0;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

//TEXTURA DIGIMON
Texture casaDigimonTexture;
Texture veemonTexture;
Texture digiEggTexture;

//#######################		MODELOS

//Ambientación
Model arbol;
Model bancaPicnic;
Model baseBici;
Model carreta;
Model kiosko;
Model lamparaPoste;
Model piedras;
Model triciclo;
Model planeta;

//Fauna
Model paloma_M;
Model ardilla_M;

//ALAMEDA
Model BaseAlamedaCentral;

//UNIVERSO DIGIMON
Model casaDigimon;
Model veemon;
Model digiEgg;
Model Taichi;
Model Exveemon;

//UNIVERSO POKEMON 
Model Charmander;
Model Charizard;

//OXXOS
Model Oxxito;

//UNIVERSO BATMAN
Model batman;
Model mansionBatman;
Model policiaCarro;
Model poliVan;
Model batimovil;
Model batiSenal;

//UNIVERSO FUTURAMA
Model brazoDerecho_M;
Model brazoIzquierdo_M;
Model piernaDerecha_M;
Model piernaIzquierda_M;
Model cuerpoBender;
Model casaFuturama;
Model tiendaFuturama;
Model naveFuturama;
Model maquinaSoda;
Model policiaRobot_M;
Model policiaPatrulla_M;

Skybox skybox;
Skybox skyboxnoche;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = { //PISO
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	casaDigimonTexture = Texture("Textures/farm_build_07.tga");
	casaDigimonTexture.LoadTextureA();



	//-------------------------------------------------------TEXTURA DIGIMON

	//VEEMON
	//veemonTexture = Texture("Textures/Skin_ZSphere_1Tex1.jpg");
	//veemonTexture.LoadTextureA();
	//veemonTexture = Texture("Textures/veemon/Skin_ZSphere_1Tex2.jpg");
	//veemonTexture.LoadTextureA();
	//veemonTexture = Texture("Textures/veemon/Skin_ZSphere_1Tex5.jpg");
	//veemonTexture.LoadTextureA();
	//veemonTexture = Texture("Textures/veemon/Skin_ZSphere_1Tex6.jpg");
	//veemonTexture.LoadTextureA();

	//DIGI-EGG
	digiEggTexture = Texture("Textures/Courage_1a.png");
	digiEggTexture.LoadTextureA();
	digiEggTexture = Texture("Textures/Courage_2.png");
	digiEggTexture.LoadTextureA();

	//-------------------------------------------------------MODELOS-------------------------------------------------------// 

	//------------------------------------------------------AMBIENTACIÓN--------------------------------------------------//

	//Arbol

	arbol = Model();
	arbol.LoadModel("Models/Arbol.obj");

	//Banca Picnic

	bancaPicnic = Model();
	bancaPicnic.LoadModel("Models/BancaPicnic.obj");

	//Base Bici

	baseBici = Model();
	baseBici.LoadModel("Models/BaseBici.obj");

	//Carreta

	carreta = Model();
	carreta.LoadModel("Models/Carreta.obj");

	//Kiosko

	kiosko = Model();
	kiosko.LoadModel("Models/KioskoC.obj");

	//Lampara Poste

	lamparaPoste = Model();
	lamparaPoste.LoadModel("Models/LamparaPoste.obj");

	//Piedras

	piedras = Model();
	piedras.LoadModel("Models/Piedras.obj");

	//Triciclo

	triciclo = Model();
	triciclo.LoadModel("Models/Triciclo.obj");

	//Banca


	//Bote basura

	//Planeta

	planeta = Model();
	planeta.LoadModel("Models/Crystals.fbx");

	//ALAMEDA

	BaseAlamedaCentral = Model();
	BaseAlamedaCentral.LoadModel("Models/BaseAlamedaCentral.obj");

	//Fauna
	paloma_M = Model();
	paloma_M.LoadModel("Models/paloma.obj");

	ardilla_M = Model();
	ardilla_M.LoadModel("Models/ardilla.obj");
	
	//-------------------------------------------------------UNIVERSO DIGIMON-------------------------------------------------------//

	//Exveemon
	Exveemon = Model();
	Exveemon.LoadModel("Models/Exveemon.dae");

	casaDigimon = Model();
	casaDigimon.LoadModel("Models/casaDigimon.obj");

	veemon = Model();
	veemon.LoadModel("Models/veemon.obj");

	digiEgg = Model();
	digiEgg.LoadModel("Models/digiEgg.obj");


	Taichi = Model();
	Taichi.LoadModel("Models/ID00600_00000000out.dae");

	//-------------------------------------------------------UNIVERSO POKÉMON-------------------------------------------------------//
	//Charmander
	Charmander = Model();
	Charmander.LoadModel("Models/UniversoPokemon/Charmander.obj");
	//Charizard
	Charizard = Model();
	Charizard.LoadModel("Models/UniversoPokemon/Charizard.obj");


	//OXXO
	Oxxito = Model();
	Oxxito.LoadModel("Models/OXXOS/oxxito.obj");

	//-------------------------------------------------------UNIVERSO BATMAN-------------------------------------------------------//

	batman = Model();
	batman.LoadModel("Models/batman_obj.obj");

	mansionBatman = Model();
	mansionBatman.LoadModel("Models/mansionBatmanT.obj");

	policiaCarro = Model();
	policiaCarro.LoadModel("Models/policiaCarro.obj");

	poliVan = Model();
	poliVan.LoadModel("Models/PoliVan.obj");

	batimovil = Model();
	batimovil.LoadModel("Models/batimovil_obj.obj");

	batiSenal = Model();
	batiSenal.LoadModel("Models/batiSenal.obj");

	//-------------------------------------------------------UNIVERSO FUTURAMA-------------------------------------------------------//
	cuerpoBender = Model();
	cuerpoBender.LoadModel("Models/cuerpoBender.obj");

	brazoIzquierdo_M = Model();
	brazoIzquierdo_M.LoadModel("Models/brazoIzquierdo.obj");

	brazoDerecho_M = Model();
	brazoDerecho_M.LoadModel("Models/brazoDerecho.obj");

	piernaIzquierda_M = Model();
	piernaIzquierda_M.LoadModel("Models/piernaIzquierda.obj");

	piernaDerecha_M = Model();
	piernaDerecha_M.LoadModel("Models/piernaDerecha.obj");


	casaFuturama = Model();
	casaFuturama.LoadModel("Models/FuturamaCasaText.obj");

	tiendaFuturama = Model();
	tiendaFuturama.LoadModel("Models/tiendafuturama.obj");

	naveFuturama = Model();
	naveFuturama.LoadModel("Models/Navefuturama.obj");

	maquinaSoda = Model();
	maquinaSoda.LoadModel("Models/SodaMachine.obj");

	policiaRobot_M = Model();
	policiaRobot_M.LoadModel("Models/policiaRobot.obj");

	policiaPatrulla_M = Model();
	policiaPatrulla_M.LoadModel("Models/policiaFuturama.obj");

	//Función para atardecer 


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/ciudad2_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/ciudad2_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/ciudad2_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/ciudad2_up.tga");
	skyboxFaces.push_back("Textures/Skybox/ciudad2_ft.tga");
	skyboxFaces.push_back("Textures/Skybox/ciudad2_bk.tga");

	skybox = Skybox(skyboxFaces);

	std::vector<std::string> skyboxFaces2;
	skyboxFaces2.push_back("Textures/Skybox/ciudad2_noche_rt.tga");
	skyboxFaces2.push_back("Textures/Skybox/ciudad2_noche_lf.tga");
	skyboxFaces2.push_back("Textures/Skybox/ciudad2_noche_dn.tga");
	skyboxFaces2.push_back("Textures/Skybox/ciudad2_noche_up.tga");
	skyboxFaces2.push_back("Textures/Skybox/ciudad2_noche_ft.tga");
	skyboxFaces2.push_back("Textures/Skybox/ciudad2_noche_bk.tga");

	skyboxnoche = Skybox(skyboxFaces2);

	Material_brillante = Material(0.3f, 256);
	Material_opaco = Material(4.0f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	//mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
	//	luzgradual, luzgradual,
	//	0.0f, 0.0f, -1.0f);
	//contador de luces puntuales

	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,
		-165.0f, 0.0f, -105.0f,
		0.3f, 0.2f, 0.0f);
	pointLightCount++;

	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,
		-145.0f, 0.0f, -180.0f,
		0.3f, 0.2f, 0.0f);
	pointLightCount++;

	pointLights[2] = PointLight(1.0f, 1.0f, 0.0f,
		0.6f, 0.6f,
		-60.0f, 1.5f, -228.0f,
		0.3f, 0.2f, 0.0f);
	pointLightCount++;

	unsigned int spotLightCount = 0;


	//luz fija
	spotLights[1] = SpotLight(1.0f, 1.0f, 0.0f,
		0.3f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	movCocheX = 2.0f;
	movCocheZ = -120.0f;
	movTriX = -160.0f;
	movTriZ = -100.0f;
	movOffset = 0.3f; //indica que esta iniciado el movimiento
	movOffset2 = 0.5f;
	avanzaX = true;
	avanzaZ = false;
	rotacion = 0.0f;
	avanzaX2 = true;
	numGiros = 0;
	luzdia = true;
	luznoche = false;

	float walkSpeed = 0.1f;  // Velocidad de la caminata
	float picoteoSpeed = 3.0f;  // Reducir la velocidad del picoteo 
	float picoteoAngle = glm::radians(10.0f);  // Ángulo máximo de picoteo
	float deltaTime; // Tiempo transcurrido desde el último frame
	GLfloat lastTime = glfwGetTime(); // Tiempo del último frame
	float timeAngle = 0.0f; // Ángulo de tiempo para la animación
	float hopSpeed = 1.0f; // Velocidad de salto
	float moveDuration = 10.0f; // Duración de cada movimiento
	float moveTimer = 0.0f; // Temporizador para el movimiento
	glm::vec3 direction = glm::vec3(1.0f, 0.0f, 0.0f); // Dirección inicial
	bool isPicoteando = false; // Estado de la paloma (picoteando o no)
	float picoteoDuration = 20.0f; // Duración del picoteo
	float picoteoTimer = 0.0f; // Temporizador para el picoteo

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS; //Maximo 60FPS
		lastTime = now;

		//luz{

		if (luzdia) {

			if (luzgradual <= 1.0 and luzgradual > 0.0 and luznoche == false)
			{

				luzgradual -= 0.0005;
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
				shaderList[0].UseShader();
				uniformModel = shaderList[0].GetModelLocation();
				uniformProjection = shaderList[0].GetProjectionLocation();
				uniformView = shaderList[0].GetViewLocation();
				uniformEyePosition = shaderList[0].GetEyePositionLocation();
				uniformColor = shaderList[0].getColorLocation();

			}if (luzgradual < 0.2)
			{
				luznoche = true;
				luzgradual = 0.2;


			}
			if (luzgradual >= 0.2 and luzgradual < 1.0 and luznoche == true)
			{

				luzgradual += 0.0005;
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				skyboxnoche.DrawSkybox(camera.calculateViewMatrix(), projection);
				shaderList[0].UseShader();
				uniformModel = shaderList[0].GetModelLocation();
				uniformProjection = shaderList[0].GetProjectionLocation();
				uniformView = shaderList[0].GetViewLocation();
				uniformEyePosition = shaderList[0].GetEyePositionLocation();
				uniformColor = shaderList[0].getColorLocation();
			}
			if (luzgradual > 1.0)
			{
				luznoche = false;
				luzgradual = 1.0;


			}

			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
				luzgradual, luzgradual,
				0.0f, 0.0f, -1.0f);

			//if (luzgradual >= 0.0 and luzgradual < 1)
			//{
			//	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
			//		luzgradual, luzgradual,
			//		0.0f, 0.0f, -1.0f);
			//	luzgradual += luzgradual + 0.5;
			//}


		}

		//Movimiento coche
		if (avanzaX) {
			if (movCocheX > 10.0f) {
				movCocheX -= movOffset * deltaTime;
				rotacion = -90;
			}
			else {
				avanzaX = false;
				avanzaZ = true;
				numGiros++;
			}
		}
		else if (avanzaZ) {
			if (movCocheZ < 0.0f) {
				movCocheZ += movOffset * deltaTime;
				rotacion = 0;
			}
			else {
				avanzaZ = false;
				avanzaX = true;
				numGiros++;
			}
		}
		else {
			if (movCocheX < -70.0f) {
				movCocheX += movOffset * deltaTime;
				rotacion = 90;
			}
			else {
				avanzaX = true;
				numGiros++;
			}
		}

		if (numGiros >= 3) {
			avanzaX = false;
			avanzaZ = false;
		}

		// Actualizar el ángulo de tiempo para la animación
		timeAngle += walkSpeed * deltaTime;

		// Limitar el valor de timeAngle para evitar overflow
		if (timeAngle > 2.0f * M_PI) {
			timeAngle -= 2.0f * M_PI;
		}

		// Variables para el movimiento de las piernas y brazos

		float legSwing = sin(timeAngle) * glm::radians(30.0f);  // Aumentar la oscilación de las piernas
		float armSwing = -sin(timeAngle) * glm::radians(30.0f);  // Aumentar la oscilación de los brazos



		// Lógica para determinar si la paloma debe picotear o avanzar
		if (isPicoteando) {
			picoteoTimer += deltaTime * 0.1f;
			if (picoteoTimer > picoteoDuration) {
				isPicoteando = false;
				picoteoTimer = 0.0f;
			}
		}
		else {

			moveTimer += deltaTime;
			if (moveTimer > moveDuration) {
				direction.y = 0.1f;
				direction.x += 0.01f;
				direction.z += 0.01f;

				moveTimer = 0.0f; // Reiniciar el temporizador

				// Si ha avanzado lo suficiente, detenerse y comenzar a picotear
				if (rand() % 100 <= 1) {  // Aproximadamente un 5% de probabilidad de detenerse para picotear
					direction.x = direction.x - 0.01f;
					direction.z = direction.z - 0.01f;
					direction.y = 0.0f;
					isPicoteando = true;
					moveTimer = 0.0f;
				}



				/*if (direction.x >= 5.0f) {
					direction.x = 5.0f;
					direction.y = 0.0f;
					isPicoteando = true;
					moveTimer = 0.0f;
				}*/
			}
		}

		// Si está picoteando, calcular la animación de picoteo
		if (isPicoteando) {
			picoteoTimer += deltaTime * 0.1f;
			if (picoteoTimer > picoteoDuration) {
				isPicoteando = false;
				picoteoTimer = 0.0f;
			}
			// Aquí puedes agregar la animación de picoteo
		}

		// Calcular la posición en el plano XZ y simular los brincos en el eje Y
		float posX = direction.x * hopSpeed;
		float posZ = direction.z * hopSpeed; // Mantener la posición en el plano XZ
		float posY = direction.y * abs(sin(timeAngle + 0.5f));  // Brincos en el eje Y


		// Calcular el ángulo de picoteo basado en una función seno para suavizar el movimiento
		float picoteoTime = picoteoSpeed * timeAngle;
		float headPitch = isPicoteando ? sin(picoteoTime) * picoteoAngle : 0.0f;  // Solo picotear si está en estado de picoteo
		float headPitch1 = sin(picoteoTime) * picoteoAngle;


		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		//shaderList[0].UseShader();
		//uniformModel = shaderList[0].GetModelLocation();
		//uniformProjection = shaderList[0].GetProjectionLocation();
		//uniformView = shaderList[0].GetViewLocation();
		//uniformEyePosition = shaderList[0].GetEyePositionLocation();
		//uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		//// luz ligada a la cámara de tipo flash
		////sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		//	glm::vec3 lowerLight = camera.getCameraPosition();
		//lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		if (mainWindow.getprendido()) {
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		else {
			shaderList[0].SetPointLights(pointLights, pointLightCount - 2);
		}

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//pisoTexture.UseTexture();
		//meshList[2]->RenderMesh();

		//model = glm::mat4(1.0);
		// Casa Digi

		//EVOULCIÓN//

		if (transformacion)
		{

			model = modelaux;
			model = glm::translate(model, glm::vec3(175.0f, 0.0f, 90.0f));
			model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			veemon.RenderModel();
			glDisable(GL_BLEND);

			tiempo += 1;
			//
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 50.0f, 0.0f));
			//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			planeta.RenderModel();

		}
		if (tiempo >= 100 and tiempo < 200)
		{
			model = modelaux;
			model = glm::translate(model, glm::vec3(175.0f, 0.0f, 90.0f));
			model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			veemon.RenderModel();

			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 50.0f, 5.0f));
			//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			planeta.RenderModel();


			transformacion = false;

			tiempo += 1;
		}

		if (tiempo == 200)
		{
			transformacion = true;
			tiempo = 1;
			evolucion += 1;
		}

		if (evolucion > 5)
		{
			transformacion = false;
			tiempo = 0;

			model = modelaux;
			model = glm::translate(model, glm::vec3(170.0f, 0.0f, 95.0f));
			model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Exveemon.RenderModel();


		}


		//if (tiempo > 0 and tiempo <= 10 and transformacion == false)
		//{
		//	model = modelaux;
		//	model = glm::translate(model, glm::vec3(175.0f, 0.0f, 90.0f));
		//	model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//	//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		//	model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		//	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//	veemon.RenderModel();
		//	tiempo -= 0.01;
		//
		//	if (tiempo <= 0)
		//	{
		//		transformacion = true;
		//		tiempo = 0.0;
		//	}
		//}
		model = modelaux;
		model = glm::translate(model, glm::vec3(125.0f, -3.0f, 300.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 235 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		casaDigimon.RenderModel();
		glDisable(GL_BLEND);
		//Veemon


		//digiEgg

		model = modelaux;
		model = glm::translate(model, glm::vec3(-20.0f, 0.0f, 350.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 350.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-60.0f, 0.0f, 350.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-80.0f, 0.0f, 350.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-100.0f, 0.0f, 350.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-120.0f, 0.0f, 350.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-140.0f, 0.0f, 350.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-160.0f, 0.0f, 350.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-20.0f, 0.0f, 325.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 325.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-60.0f, 0.0f, 325.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-80.0f, 0.0f, 325.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-100.0f, 0.0f, 325.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-120.0f, 0.0f, 325.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();


		model = modelaux;
		model = glm::translate(model, glm::vec3(-20.0f, 0.0f, 300.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 300.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-60.0f, 0.0f, 300.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-80.0f, 0.0f, 300.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-100.0f, 0.0f, 300.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-20.0f, 0.0f, 275.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 275.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-60.0f, 0.0f, 275.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();


		model = modelaux;
		model = glm::translate(model, glm::vec3(-180.0f, 0.0f, 325.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-180.0f, 0.0f, 300.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-180.0f, 0.0f, 275.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-180.0f, 0.0f, 250.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-180.0f, 0.0f, 225.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();



		model = modelaux;
		model = glm::translate(model, glm::vec3(-160.0f, 0.0f, 300.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-160.0f, 0.0f, 275.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-160.0f, 0.0f, 250.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-160.0f, 0.0f, 225.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();

		//--

		model = modelaux;
		model = glm::translate(model, glm::vec3(-140.0f, 0.0f, 300.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-140.0f, 0.0f, 275.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-140.0f, 0.0f, 250.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-140.0f, 0.0f, 225.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();


		//--

		model = modelaux;
		model = glm::translate(model, glm::vec3(-120.0f, 0.0f, 275.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 290 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-120.0f, 0.0f, 250.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 290 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-120.0f, 0.0f, 225.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 290 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();


		//--

		model = modelaux;
		model = glm::translate(model, glm::vec3(-100.0f, 0.0f, 250.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 190 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-100.0f, 0.0f, 225.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 190 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();

		//--

		model = modelaux;
		model = glm::translate(model, glm::vec3(-80.0f, 0.0f, 250.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-80.0f, 0.0f, 225.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		digiEgg.RenderModel();


		//Taichi

		model = modelaux;
		model = glm::translate(model, glm::vec3(100.0f, 0.0f, 50.0f));
		//model = glm::translate(model, glm::vec3(5.0 + camera.getCameraPosition().x, -15.0f + camera.getCameraPosition().y, camera.getCameraPosition().z));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Taichi.RenderModel();
		//-------------------------------------------------------UNIVERSO POKÉMON------------------------------------------------------//
		model = modelaux;
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 100.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Charmander.RenderModel();


		model = modelaux;
		model = glm::translate(model, glm::vec3(-70.0f, 11.0f, 70.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Charizard.RenderModel();

		//-------------------------------------------------------OXXOS-----------------------------------------------------------------//
		model = modelaux;
		model = glm::translate(model, glm::vec3(-185.0f, 0.0f, -45.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Oxxito.RenderModel();
		//-------------------------------------------------------UNIVERSO BATMAN-------------------------------------------------------//
		model = modelaux;
		model = glm::translate(model, glm::vec3(-35.0f, 0.0f, -40.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		batman.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-130.0f, 0.0f, -170.0f));
		model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		mansionBatman.RenderModel();
		glDisable(GL_BLEND);


		model = modelaux;
		//model = glm::translate(model, glm::vec3(-165.0f, 0.0f, -100.0f));
		model = glm::translate(model, glm::vec3(-160 + movTriX, 0.0f, -100));
		model = glm::rotate(model, 145 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		policiaCarro.RenderModel();
		

		model = modelaux;
		model = glm::translate(model, glm::vec3(-145.0f, 0.0f, -180.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f, 5.7f, 7.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		poliVan.RenderModel();


		model = modelaux;
		//model = glm::translate(model, glm::vec3(-40.0f, 0.0f, -130.0f));
		model = glm::translate(model, glm::vec3(movCocheX, 0.0f, movCocheZ));
		model = glm::rotate(model, rotacion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		batimovil.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-60.0f, 0.0f, -228.0f));
		model = glm::rotate(model, -25 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		batiSenal.RenderModel();

		//-------------------------------------------------------AMBIENTACIÓN-------------------------------------------------------//

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(20.0f, 0.5f, 20.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BaseAlamedaCentral.RenderModel();


		model = modelaux;
		model = glm::translate(model, glm::vec3(100.0f, 0.0f, 240.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(240.0f, 0.0f, 100.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, -200.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-200.0f, 0.0f, -100.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(50.0f, -0.0f, 300.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bancaPicnic.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-150.0f, -0.0f, 150.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bancaPicnic.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(150.0f, -0.0f, 170.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		baseBici.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(90.0f, -0.0f, -100.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		baseBici.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-150.0f, -0.0f, 100.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		carreta.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -0.0f, 200.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		kiosko.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -210.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		kiosko.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(9.0f, -0.0f, 50.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamparaPoste.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-8.0f, -0.0f, 80.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamparaPoste.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(9.0f, -0.0f, 110.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamparaPoste.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-8.0f, -0.0f, 150.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamparaPoste.RenderModel();


		model = modelaux;
		model = glm::translate(model, glm::vec3(9.0f, -0.0f, -50.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamparaPoste.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-8.0f, -0.0f, -80.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamparaPoste.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(9.0f, -0.0f, -110.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamparaPoste.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-8.0f, -0.0f, -150.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamparaPoste.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(150.0f, -0.0f, 120.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piedras.RenderModel();


		model = modelaux;
		model = glm::translate(model, glm::vec3(5.0f, -0.0f, -160.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		triciclo.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(5.0f, -0.0f, 100.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		triciclo.RenderModel();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(20.0f, 1.0f + posY, 6.0f + posZ * 15.0f)); // Aplicar traslación
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ardilla_M.RenderModel();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(60 + 0.0f, 1.0f + posY, 45.0f - posZ * 15.0f)); // Aplicar traslación
		model = glm::rotate(model, 50.0f, glm::vec3(0.0f, 1.0f, 0.0f)); // Aplicar rotación
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ardilla_M.RenderModel();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-60.0f + 10.0f * cos(posX), posY, -70.0f + 10.0f * sin(posZ))); // Aplicar traslación
		model = glm::rotate(model, -50.0f - direction.x, glm::vec3(0.0f, 1.0f, 0.0f)); // Aplicar rotación
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ardilla_M.RenderModel();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(40.0f + 0.0f, 1.0f, 70.0f)); // Aplicar traslación
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ardilla_M.RenderModel();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-20.0f + 0.0f, 1.0f, 60.0f)); // Aplicar traslación
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ardilla_M.RenderModel();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(40.0f + 10.0f * cos(posX), posY, 40.0f + 10.0f * sin(posZ))); // Aplicar traslación
		model = glm::rotate(model, 180 - direction.x, glm::vec3(0.0f, 1.0f, 0.0f)); // Aplicar rotación
		model = glm::rotate(model, headPitch, glm::vec3(1.0f, 0.0f, 0.0f)); // Aplicar rotación de picoteo en el eje X
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paloma_M.RenderModel();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-60.0f + 10.0f * cos(posX), posY, 40.0f + 10.0f * sin(posZ))); // Aplicar traslación
		model = glm::rotate(model, 180 - direction.x, glm::vec3(0.0f, 1.0f, 0.0f)); // Aplicar rotación
		model = glm::rotate(model, headPitch, glm::vec3(1.0f, 0.0f, 0.0f)); // Aplicar rotación de picoteo en el eje X
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paloma_M.RenderModel();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-60.0f + 10.0f * cos(posX), posY, -35.0f + 10.0f * sin(posZ))); // Aplicar traslación
		model = glm::rotate(model, 180 - direction.x, glm::vec3(0.0f, 1.0f, 0.0f)); // Aplicar rotación
		model = glm::rotate(model, headPitch, glm::vec3(1.0f, 0.0f, 0.0f)); // Aplicar rotación de picoteo en el eje X
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paloma_M.RenderModel();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-60.0f + 10.0f * cos(posX), posY, 70.0f + 10.0f * sin(posZ))); // Aplicar traslación
		model = glm::rotate(model, 180 - direction.x, glm::vec3(0.0f, 1.0f, 0.0f)); // Aplicar rotación
		model = glm::rotate(model, headPitch, glm::vec3(1.0f, 0.0f, 0.0f)); // Aplicar rotación de picoteo en el eje X
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paloma_M.RenderModel();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 50.0f)); // Aplicar traslación
		model = glm::rotate(model, 180 - direction.x, glm::vec3(0.0f, 1.0f, 0.0f)); // Aplicar rotación
		model = glm::rotate(model, headPitch, glm::vec3(1.0f, 0.0f, 0.0f)); // Aplicar rotación de picoteo en el eje X
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paloma_M.RenderModel();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 80.0f)); // Aplicar traslación
		model = glm::rotate(model, 180 - direction.x, glm::vec3(0.0f, 1.0f, 0.0f)); // Aplicar rotación
		model = glm::rotate(model, headPitch, glm::vec3(1.0f, 0.0f, 0.0f)); // Aplicar rotación de picoteo en el eje X
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paloma_M.RenderModel();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-60.0f + 10.0f, posY, -30.0f + 10.0f * sin(posZ))); // Aplicar traslación
		model = glm::rotate(model, 180 - direction.x, glm::vec3(0.0f, 1.0f, 0.0f)); // Aplicar rotación
		model = glm::rotate(model, headPitch, glm::vec3(1.0f, 0.0f, 0.0f)); // Aplicar rotación de picoteo en el eje X
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paloma_M.RenderModel();

		//-------------------------------------------------------UNIVERSO FUTURAMA-------------------------------------------------------//
		// Renderizado de Bender con jerarquia
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(camera.getCameraPosition().x, -15.0f + camera.getCameraPosition().y, 10.0f + camera.getCameraPosition().z));
		//model = glm::rotate(model, camera.getCameraPosition().y, glm::vec3(0.0f, 0.5f, 0.0f));
		//model = glm::translate(model, glm::vec3(10.0f, 4.0f, 0.0f ));

		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		modelaux = model;
		cuerpoBender.RenderModel();

		// Renderizar brazo derecho con oscilación
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.4f, 2.0f, 0.0f));
		model = glm::rotate(model, armSwing, glm::vec3(1.0f, 0.0f, 0.0f));  // Rotación del brazo
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazoDerecho_M.RenderModel();

		// Renderizar brazo izquierdo con oscilación
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.3f, 2.0f, 0.0f));
		model = glm::rotate(model, -armSwing, glm::vec3(1.0f, 0.0f, 0.0f));  // Rotación del brazo
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazoIzquierdo_M.RenderModel();

		// Renderizar pierna derecha con oscilación
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, legSwing, glm::vec3(1.0f, 0.0f, 0.0f));  // Rotación de la pierna
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piernaDerecha_M.RenderModel();

		// Renderizar pierna izquierda con oscilación
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, -legSwing, glm::vec3(1.0f, 0.0f, 0.0f));  // Rotación de la pierna
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piernaIzquierda_M.RenderModel();

		//model = glm::mat4(1.0f);
		////model = modelaux;
		//model = glm::translate(model, glm::vec3(-100.0f, 0.0f, 100.0f));
		////model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//bender.RenderModel();

		//Casa
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(160.0f, 0.0f, -280.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		casaFuturama.RenderModel();
		//xxxxxxxxxxxxxxxxxxxxx
		//Policia robot
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(140.0f, 0.0f, -280.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		policiaRobot_M.RenderModel();


		//Patrulla
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(130.0f, 0.0f, -280.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		policiaPatrulla_M.RenderModel();

		//tienda
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(170.0f, 0.0f, -160.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tiendaFuturama.RenderModel();

		//nave
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(120.0f, 0.0f, -50.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		naveFuturama.RenderModel();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(150.0f, 0.0f, -60.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maquinaSoda.RenderModel();



		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
