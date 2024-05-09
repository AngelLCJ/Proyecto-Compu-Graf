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
#include "Shader_m.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

//Variables de animación
float movCocheX;
float movCocheZ;
float movOffset; //bandera para detener el carro 
float rotllanta;
float rotllantaOffset; //bandera para detener el carro 
bool avanzaX; //indica si avanza o esta detenido el coche
bool avanzaZ;
float rotacionDado;
float rotacion;
float rotacionDadoOffset;
int numGiros;

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

//ALAMEDA
Model BaseAlamedaCentral;

//UNIVERSO DIGIMON
Model casaDigimon;
Model veemon;
Model digiEgg;
Model Taichi;
Model Exveemon;

//UNIVERSO BATMAN
Model batman;
Model mansionBatman;
Model policiaCarro;
Model poliVan;
Model batimovil;
Model batiSenal;

//UNIVERSO FUTURAMA
Model bender;
Model casaFuturama;
Model tiendaFuturama;
Model naveFuturama;
Model maquinaSoda;


Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_texture.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_texture.frag";




//cálculo del promedio de las normales para sombreado de Phong
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

	GLfloat floorVertices[] = {
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
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);


	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
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

	//-------------------------------------------------------MODELOS 
	
	//------------------------------------------------------- AMBIENTACIÓN

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



	//ALAMEDA

	BaseAlamedaCentral = Model();
	BaseAlamedaCentral.LoadModel("Models/BaseAlamedaCentral.obj");
		
	//Exveemon

	Exveemon = Model();
	Exveemon.LoadModel("Models/Exveemon.dae");

	casaDigimon = Model();
	casaDigimon.LoadModel("CasaDigimon/casaDigimon.obj");

	veemon = Model();
	veemon.LoadModel("Models/veemon.obj");

	digiEgg = Model();
	digiEgg.LoadModel("Models/digiEgg.obj");


	Taichi = Model();
	Taichi.LoadModel("Models/ID00600_00000000out.dae");

	//-------------------------------------------------------Universo batman

	batman = Model();
	batman.LoadModel("Models/batman_obj.obj");

	mansionBatman = Model();
	mansionBatman.LoadModel("Models/mansionBatman.obj");
	
	policiaCarro = Model();
	policiaCarro.LoadModel("Models/policiaCarro.obj");
	
	poliVan = Model();
	poliVan.LoadModel("Models/PoliVan.obj");

	batimovil = Model();
	batimovil.LoadModel("Models/batimovil_obj.obj");
	
	batiSenal = Model();
	batiSenal.LoadModel("Models/batiSenal.obj");

	//-------------------------------------------------------Universo futurama
	bender = Model();
	bender.LoadModel("Models/bender.obj");

	casaFuturama = Model();
	casaFuturama.LoadModel("Models/FuturamaCasaText.obj");

	tiendaFuturama = Model();
	tiendaFuturama.LoadModel("Models/tiendafuturama.obj");

	naveFuturama = Model();
	naveFuturama.LoadModel("Models/Navefuturama.obj");

	maquinaSoda = Model();
	maquinaSoda.LoadModel("Models/SodaMachine.obj");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/ciudad2_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/ciudad2_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/ciudad2_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/ciudad2_up.tga");
	skyboxFaces.push_back("Textures/Skybox/ciudad2_ft.tga");
	skyboxFaces.push_back("Textures/Skybox/ciudad2_bk.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	movCocheX = 13.0f;
	movCocheZ = -120.0f;
	movOffset = 0.3f; //indica que esta iniciado el movimiento
	avanzaX = true;
	avanzaZ = false;
	rotacion = 0.0f;
	rotacionDadoOffset = 0.1f;
	numGiros = 0;

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Movimiento coche
		if (avanzaX) {
			if (movCocheX > -100.0f) {
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
			if (movCocheZ < 0.6f) {
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
			if (movCocheX < -75.0f) {
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


		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		//pisoTexture.UseTexture();
		//meshList[2]->RenderMesh();

		//model = glm::mat4(1.0);
		// Casa Digi

		model = modelaux;
		model = glm::translate(model, glm::vec3(100.0f, 0.0f, 150.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		casaDigimon.RenderModel();
		glDisable(GL_BLEND);
		//Veemon

		model = modelaux;
		model = glm::translate(model, glm::vec3(-100.0f, 0.0f, -150.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		veemon.RenderModel();

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
		model = glm::translate(model, glm::vec3(-200.0f, 0.0f, -150.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Taichi.RenderModel();

		//Exveemon


		model = modelaux;
		model = glm::translate(model, glm::vec3(-250.0f, 0.0f, -150.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Exveemon.RenderModel();

		//_________________________________UNIVERSO BATMAN___________________________________
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
		mansionBatman.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-165.0f, 0.0f, -100.0f));
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

		//_________________________________Ambientación___________________________________

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -0.6f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(20.0f, 0.5f, 20.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BaseAlamedaCentral.RenderModel();

		//_________________________________UNIVERSO FUTURAMA___________________________________
		//Bender
		model = modelaux;
		model = glm::translate(model, glm::vec3(300.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bender.RenderModel();

		//Casa
		model = modelaux;
		model = glm::translate(model, glm::vec3(300.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		casaFuturama.RenderModel();

		//tienda
		model = modelaux;
		model = glm::translate(model, glm::vec3(300.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tiendaFuturama.RenderModel();

		//nave
		model = modelaux;
		model = glm::translate(model, glm::vec3(300.0f, 0.0f, -15.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		naveFuturama.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(300.0f, 0.0f, -30.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getanguloCofre()), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maquinaSoda.RenderModel();

		glUseProgram(0);
		//hola
		
		mainWindow.swapBuffers();
	}

	return 0;
}
/*
//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		logofiTexture.UseTexture(); //textura con transparencia o traslucidez
		FIGURA A RENDERIZAR de OpenGL, si es modelo importado no se declara UseTexture
		glDisable(GL_BLEND);
*/