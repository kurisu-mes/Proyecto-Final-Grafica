//para cargar imagen
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

GLfloat cycleDuration = 100.0f;
GLfloat minAmbient = 0.1f;
GLfloat maxAmbient = 1.0f; // Valor original
GLfloat minDiffuse = 0.1f;
GLfloat maxDiffuse = 1.0f; // Valor original

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture PiramideTexture;
Model Piso_M;


//modelos y texturas auxiliares
Model PuertaDerE;
Model PuertaIzqE;
Model PilaresE;
Model Letrero_M;
Texture Letrero_T;
Model ArcoRing;
Model PuertaDerR;
Model PuertaIzqR;

Model ProtoCabeza;
Model ProtoTorso;
Model ProtoEscudo;
Model ProtoIzqBrazo;
Model ProtoDerBrazo;
Model ProtoIzqPierna;
Model ProtoDerPierna;

Skybox skybox;
Skybox skyboxNoche;

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

//variables para animaciones
//Para el letrero------------------------------------------------------
float toffsetLetrero = 0.0f;
float velocidadLetrero = 0.01f;
//Para la puerta
float RpuertaDer = 0.0f; // Angulo de rotacion ACTUAL
float TpuertaE = 0.0f; // Traslacion X ACTUAL
float TpuertaE_Z = 0.0f; // Traslacion Z ACTUAL
float AjusteP = -0.5f; // Ajuste para que la puerta no atraviese el pilar

float TpuertaE_Target_X = -3.0f; //Desplazamiento objetivo en X
float RpuertaDer_Target = 90.0f; // Rotacion objetivo de 90 grados
float velocidadPuerta = 0.05f; // Multiplicador para la velocidad de la animacion

//puertas ring
float posXPuertaR = 0.0f;
float movPuerta = 0.2f;

//protoman
float brazoProtoR = -90.0f, escudoProto1 = 0.0f, escudoProto2 = 0.0f, piernaProto = 0.0f, posicionProto = 0.0f;


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


//Sirve para poder hacer interpolacion lineal y calcular los angulos y posicion en cada frame
float lerp(float a, float b, float t)
{
	return a + t * (b - a);
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	Piso_M = Model();
	Piso_M.LoadModel("Models/pisoProyecto.obj");

	//arcos
	PuertaDerE = Model();
	PuertaDerE.LoadModel("Models/puertaDerEntrada.obj");
	PuertaIzqE = Model();
	PuertaIzqE.LoadModel("Models/puertaIzqEntrada.obj");
	PilaresE = Model();
	PilaresE.LoadModel("Models/pilaresEntrada.obj");
	Letrero_M = Model();
	Letrero_M.LoadModel("Models/Letrero.obj");
	Letrero_T = Texture("Textures/Letrero.png");
	Letrero_T.LoadTextureA();

	ArcoRing = Model();
	ArcoRing.LoadModel("Models/stoneArch.obj");
	PuertaDerR = Model();
	PuertaDerR.LoadModel("Models/puertaDerArco.obj");
	PuertaIzqR = Model();
	PuertaIzqR.LoadModel("Models/puertaIzqArco.obj");

	//PROTOMAN
	ProtoCabeza = Model();
	ProtoCabeza.LoadModel("Models/protoHead.obj");
	ProtoTorso = Model();
	ProtoTorso.LoadModel("Models/protoTorso.obj");
	ProtoEscudo = Model();
	ProtoEscudo.LoadModel("Models/protoShield.obj");
	ProtoIzqBrazo = Model();
	ProtoIzqBrazo.LoadModel("Models/protoLArm.obj");
	ProtoDerBrazo = Model();
	ProtoDerBrazo.LoadModel("Models/protoRArm.obj");
	ProtoIzqPierna = Model();
	ProtoIzqPierna.LoadModel("Models/protoLLeg.obj");
	ProtoDerPierna = Model();
	ProtoDerPierna.LoadModel("Models/protoRLeg.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	//skybox de noche
	std::vector<std::string> skyboxFacesNoche;
	skyboxFacesNoche.push_back("Textures/Skybox/cupertin-lake-night_rt.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/cupertin-lake-night_lf.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/cupertin-lake-night_dn.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/cupertin-lake-night_up.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/cupertin-lake-night_bk.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/cupertin-lake-night_ft.tga");

	skyboxNoche = Skybox(skyboxFacesNoche);


	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	//Encender luces nocturnas
	bool lucesNocturnasEncendidas = false;

	printf("Controles:\n");
	printf("WASD - Mover cámara\n");
	//modos de camara
	printf("1 - Modo Principal (Primera Persona)\n");
	printf("2 - Modo Aéreo\n");
	printf("3 - Modo Estático\n");

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::mat4 modelPiso(1.0);
	glm::mat4 elementos(1.0);
	glm::mat4 elementoLocal(1.0);

	glm::mat4 baseProto(1.0);
	glm::mat4 brazoProto(1.0);

	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//*******************ANIMACIONES*********************************
				//Animacion de la puerta
		if (mainWindow.getEntradaAbierta())
		{
			// --- ABRIR PUERTAS ---
			// Interpolar suavemente hacia los valores objetivo
			RpuertaDer = lerp(RpuertaDer, RpuertaDer_Target, velocidadPuerta * deltaTime);
			TpuertaE = lerp(TpuertaE, TpuertaE_Target_X, velocidadPuerta * deltaTime);
			TpuertaE_Z = lerp(TpuertaE_Z, AjusteP, velocidadPuerta * deltaTime);
		}
		else
		{
			// --- CERRAR PUERTAS ---
			// Interpolar suavemente de vuelta a posicion original
			RpuertaDer = lerp(RpuertaDer, 0.0f, velocidadPuerta * deltaTime);
			TpuertaE = lerp(TpuertaE, 0.0f, velocidadPuerta * deltaTime);
			TpuertaE_Z = lerp(TpuertaE_Z, 0.0f, velocidadPuerta * deltaTime);
		}

		//movimiento Puertas
		if (mainWindow.getEntradaAbierta()) {
			//cerradas, se abren
			if (posXPuertaR > -90.0f) posXPuertaR -= 1.0f * deltaTime;
		}
		else {
			//abiertas, se abren
			if (posXPuertaR < 0.0f) posXPuertaR += 1.0f * deltaTime;
		}

		//Animacion Protoman
		if (mainWindow.getEstadoProtoman()) {
			if (brazoProtoR < 0.0f) brazoProtoR += 3.0f * deltaTime;
			else {
				if (escudoProto1 < 90.0f) escudoProto1 += 3.0f * deltaTime;
				if (escudoProto2 < 60.0f) escudoProto2 += 2.0f * deltaTime;
				else {
					if (piernaProto < 45.0f) {
						piernaProto += 1.5f * deltaTime;
						posicionProto -= 0.1f * deltaTime;
					}
				}
			}
		}
		else {
			if (piernaProto > 0.0f) {
				piernaProto -= 1.5f * deltaTime;
				posicionProto += 0.1f * deltaTime;
			}
			else {
				if (escudoProto1 > 0.0f) escudoProto1 -= 3.0f * deltaTime;
				if (escudoProto2 > 0.0f) escudoProto2 -= 2.0f * deltaTime;
				else if (brazoProtoR > -90.0f) brazoProtoR -= 3.0f * deltaTime;
			}
		}



		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//---------- Camaras -----------------
		// aadición de control de teclado
		bool* keys = mainWindow.getsKeys();

		// Control de la cámara
		camera.keyControl(keys, deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Control para cambiar el modo de la cámara
		if (keys[GLFW_KEY_1])
		{
			camera.setCameraMode(1); // Modo Principal
		}
		if (keys[GLFW_KEY_2])
		{
			camera.setCameraMode(2); // Modo Aéreo
		}
		if (keys[GLFW_KEY_3])
		{
			camera.setCameraMode(3); // Modo Estático
		}

		// --- CICLO DÍA/NOCHE ---
		// Calcular el factor de iluminación (0.0 noche, 1.0 día)
		GLfloat angle = (now / cycleDuration) * 2.0f * M_PI;
		GLfloat PorcentajeLuz = (cos(angle) + 1.0f) * 0.5f; // Mapea -1..1 a 0..1

		// Interpolar las intensidades
		GLfloat currentAmbient = minAmbient + (maxAmbient - minAmbient) * PorcentajeLuz;
		GLfloat currentDiffuse = minDiffuse + (maxDiffuse - minDiffuse) * PorcentajeLuz;

		GLfloat lightDirX = 0.0f;
		GLfloat lightDirY = -cos(angle);
		GLfloat lightDirZ = sin(angle);

		mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
			currentAmbient, currentDiffuse,
			lightDirX, lightDirY, lightDirZ);

		if (PorcentajeLuz < 0.3f) lucesNocturnasEncendidas = true;
		else lucesNocturnasEncendidas = false;


		// Clear the window
		glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		if (lucesNocturnasEncendidas)
		{
			skyboxNoche.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		else
		{
			skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		}

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);




		/* Piso simple con textura
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();
		*/

		//Piso modelado con Blender
		modelPiso = glm::mat4(1.0);
		modelPiso = glm::translate(modelPiso, glm::vec3(40.0f, -2.5f, 0.0f));
		//color que afecta a todos los elementos
		color = glm::vec3(0.6f, 0.6f, 0.6f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPiso));
		Piso_M.RenderModel();


		//--------------------------------- PUERTAS ----------------------------
		//Pilares entrada
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(33.0f, -2.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PilaresE.RenderModel();

		//Puerta izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.0f, 0.0f, -0.8f));
		model = glm::translate(model, glm::vec3(TpuertaE, 0.0f, TpuertaE_Z));;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PuertaIzqE.RenderModel();

		//Puerta derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.0f, 0.0f, -0.8f));
		model = glm::translate(model, glm::vec3(-TpuertaE, 0.0f, TpuertaE_Z));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PuertaDerE.RenderModel();

		//letrero
		/*
		* model = glm::mat4(1.0);
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.85f, 2.25f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		// Vector para el offset del letrero
		// Para el letrero con desplazamiento
		toffsetLetrero += velocidadLetrero * deltaTime;
		if (toffsetLetrero > 1.0f)
		{
			toffsetLetrero = 0.0f;
		}
		glm::vec2 letreroOffset = glm::vec2(toffsetLetrero, 0.0f);
		// Envio del offset al shader
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(letreroOffset));
		Letrero_T.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Letrero_M.RenderModel();
		toffset = glm::vec2(0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		*/

		//ARCO
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(116.0f, -2.0f, 14.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		elementos = glm::scale(elementos, glm::vec3(0.9f, 0.9f, 0.9f));
		elementos = glm::rotate(elementos, -120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = elementos;
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		ArcoRing.RenderModel();

		//puerta rotatoria der
		elementos = modelaux;
		elementos = glm::translate(elementos, glm::vec3(6.5f, 0.0f, 2.2f));
		elementos = glm::scale(elementos, glm::vec3(1.15f, 1.3f, 1.0f));
		elementos = glm::rotate(elementos, RpuertaDer * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//elementos = glm::rotate(elementos, posXPuertaR, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		PuertaDerR.RenderModel();

		//puerta rotatoria izq
		elementos = modelaux;
		elementos = glm::translate(elementos, glm::vec3(-6.0f, 0.0f, 2.2f));
		elementos = glm::scale(elementos, glm::vec3(1.15f, 1.3f, 1.0f));
		elementos = glm::rotate(elementos, -RpuertaDer * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//elementos = glm::rotate(elementos, posXPuertaR * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		PuertaIzqR.RenderModel();

		//------------------------------------------

		//Posicionar torso
		baseProto = glm::mat4(1.0f);
		baseProto = glm::translate(baseProto, glm::vec3(0.0f, 0.0f, posicionProto));
		//base = glm::translate(base, glm::vec3(PosRolx, 7.0f, PosRolz - distanciacaminar));
		//base = glm::scale(base, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(baseProto));
		ProtoTorso.RenderModel();

		//Cabeza
		modelaux = baseProto;
		modelaux = glm::translate(modelaux, glm::vec3(0.0f, 0.56f, 0.0f));
		//modelaux = glm::rotate(modelaux, glm::radians(rotacionBrazoDer), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		ProtoCabeza.RenderModel();

		//Brazo Derecho
		brazoProto = baseProto; 
		brazoProto = glm::translate(brazoProto, glm::vec3(-0.3f, 0.325f, -0.16f));
		brazoProto = glm::rotate(brazoProto, 60 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(brazoProto));
		ProtoDerBrazo.RenderModel();

		//escudo
		modelaux = brazoProto;
		modelaux = glm::translate(modelaux, glm::vec3(-0.56f, 0.0f, 0.0f));
		//modelaux = glm::rotate(modelaux, glm::radians(rotacionBrazoDer), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = glm::rotate(modelaux, escudoProto1 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = glm::rotate(modelaux, escudoProto2 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = glm::rotate(modelaux, escudoProto1 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		ProtoEscudo.RenderModel();

		//Brazo Izquierdo
		modelaux = baseProto;
		modelaux = glm::translate(modelaux, glm::vec3(0.5f, 0.4f, -0.16f));
		modelaux = glm::rotate(modelaux, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = glm::rotate(modelaux, brazoProtoR * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//modelaux = glm::rotate(modelaux, glm::radians(rotacionBrazoDer), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		ProtoIzqBrazo.RenderModel();

		//Pierna Derecha
		modelaux = baseProto;
		modelaux = glm::translate(modelaux, glm::vec3(-0.17f, -0.53f, -0.13f));
		modelaux = glm::rotate(modelaux, piernaProto * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//modelaux = glm::rotate(modelaux, glm::radians(rotacionBrazoDer), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		ProtoDerPierna.RenderModel();

		//Pierna Izquierda
		modelaux = baseProto;
		modelaux = glm::translate(modelaux, glm::vec3(0.25f, -0.53f, -0.1f));
		//modelaux = glm::rotate(modelaux, glm::radians(rotacionBrazoDer), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		ProtoIzqPierna.RenderModel();

		// AQUI ACABA ROLAND

		/*
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);
		*/
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
