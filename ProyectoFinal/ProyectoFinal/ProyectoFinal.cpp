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
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <filesystem> //Para captura de errores.
#include <iostream>

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

Texture plainTexture;

Model Piso_M;
Model Angela_Desk;
Model Angela_Chair;
Model Ofrenda;
Model PiramideRing;
Model Ring;

//GALERIA
//centro
Model CalendarioDoom;
Model PenachoMan;
Model MegaHawlucha;

//alrededores
Model ParedGaleria;
Model ExpositorGaleria;
Model Caballete1;
Model Caballete2;

//modelos y texturas auxiliares
Model capoLampara;
Model fuegoLampara;

Model PuertaDerE;
Model PuertaIzqE;
Model PilaresE;
Model Letrero_M;
Texture Letrero_T;
Model ArcoRing;
Model PuertaDerR;
Model PuertaIzqR;

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
SpotLight spotLights[MAX_SPOT_LIGHTS];

PointLight pointLights_Escenario1[4];
PointLight pointLights_Escenario2[4];
PointLight pointLights_Escenario3[4];

//variables para animaciones
//Para el letrero------------------------------------------------------
float toffsetLetrero = 0.0f;
float velocidadLetrero = 0.01f;
//Para la puerta
float Rpuerta = 0.0f; // Angulo de rotacion ACTUAL
float TpuertaE = 0.0f; // Traslacion X ACTUAL
float TpuertaE_Z = 0.0f; // Traslacion Z ACTUAL
float AjusteP = -0.5f; // Ajuste para que la puerta no atraviese el pilar

float TpuertaE_Target_X = -3.0f; //Desplazamiento objetivo en X
float RpuertaDer_Target = 90.0f; // Rotacion objetivo de 90 grados
float velocidadPuerta = 0.05f; // Multiplicador para la velocidad de la animacion

//Modelo Phantump
Model PhantumpCabeza;
Model PhantumpCuerpo;
Model PhantumpBrazos;

//Modelo Mawile
Model MawileCabeza;
Model MawileCuerpo;
Model MawileBI;
Model MawileMI;
Model MawileBD;
Model MawileMD;
Model MawilePI;
Model MawilePD;

//Roland
Model RolandTorso;
Model RolandBrazoDer;
Model RolandBrazoIzq;
Model RolandPiernaDer;
Model RolandPiernaIzq;

//Incineroar
Model InciCabeza;
Model InciTorso;
Model InciBD;
Model InciBI;
Model InciPD;
Model InciPI;
Model InciCola;

//Protoman
Model ProtoCabeza;
Model ProtoTorso;
Model ProtoEscudo;
Model ProtoIzqBrazo;
Model ProtoDerBrazo;
Model ProtoIzqPierna;
Model ProtoDerPierna;

//Prueba caminata
float anguloMovimiento = 0.0f;  // controla el ciclo de movimiento (sinusoidal)
float velocidadPaso = 0.005f;     // velocidad del ciclo de paso

int currentCameraMode = 1; // Rastreador de modo de cámara

// Posición y rotación guardadas de Roland
glm::vec3 rolandAvatarPos = glm::vec3(0.0f, 1.5f, 2.0f); // Posición inicial (ajusta si es necesario)
float rolandAvatarYaw = M_PI; // Rotación inicial (mirando a -Z)

// Animacion compleja incineroar
int recorrido = 1;
float orienta = 0.0f; //Orientacion del cuerpo
float tiempolocal = 0.0f; //temporizador de los giros
float rotacionBrazoDerInci = 0.0f;
float rotacionBrazoIzqInci = 0.0f;
float rotacionPiernaDerInci = 0.0f;
float rotacionPiernaIzqInci = 0.0f;
float rotacioncabezaInci = 0.0f; //Rotaciones miembros del modelo

//PosInicial Incineroar
float pos_ini_x_inci = -30.0f;
float pos_ini_z_inci = -30.0f;
float velocidadpiernas = 5.0f;
float velocidadbrazos = 4.0f;
bool animacionInci = true;

//Variables Movimiento Roland
float rotacionBrazoDer = 0.0f;
float rotacionBrazoIzq = 0.0f;
float rotacionPiernaDer = 0.0f;
float rotacionPiernaIzq = 0.0f;
bool caminarRoland = false;

//Variables Movimiento Protoman
float brazoProtoR = -90.0f;
float escudoProto1 = 0.0f;
float escudoProto2 = 0.0f;
float posicionProto = 0.0f;
float piernaProto = 0.0f;

//Variables animación simple Phantump

float pos_ini_x_pha = 5.0f;
float pos_ini_z_pha = -20.0f;
float pos_ini_y_pha = 0.0f;
float altura_max = 3.0f;
float rotacion_brazos = 0.0f;
float elevacion = 0.0f;
float elevacion_cuerpo = 0.0f;
float ladeo_cabeza = 0.0f;
bool subiendo = true;
bool bajando = true;

//Variables posicionales Mawile y de animacion

float pos_ini_x_ma = 10.0f;
float pos_ini_y_ma = 0.0f;
float pos_ini_z_ma = -30.0f;
float rotacion_cab_si_ma = 0.0f;
float rotacion_cab_no_ma = 0.0f;
float rotacion_brazos_ma = 0.0f;
float rotacion_manos_ma = 0.0f;
float desplazamiento_brazos_z = 0.0f;
float tiempollora = 0.0f;
bool animMa = false;
bool si = false;
bool no = false;
bool pausa = false;
int estadoma = 0;

//Variables miniaudio

ma_engine engine;
ma_sound sonidoFondo;
ma_sound ambientalRing;
ma_sound ambientalgaleria;
ma_sound efectoProtoman;
ma_sound efectoAngela;
ma_sound sonidoEspacialVordt;
bool sonidopausa = false;
bool engineinit = false;
bool ambientinit = false;
bool spatialinit = false;



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

	camera = Camera(glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();

	Piso_M = Model();
	Piso_M.LoadModel("Models/pisoProyecto.obj");

	//recepcion
	Angela_Desk = Model();
	Angela_Desk.LoadModel("Models/escritorio.obj");
	Angela_Chair = Model();
	Angela_Chair.LoadModel("Models/sillaAngela.obj");

	//ofrenda
	Ofrenda = Model();
	Ofrenda.LoadModel("Models/ofrenda.obj");

	//piramide
	PiramideRing = Model();
	PiramideRing.LoadModel("Models/Piramide.obj");
	Ring = Model();
	Ring.LoadModel("Models/ring.obj");

	//Mega Hawlucha
	MegaHawlucha = Model();
	MegaHawlucha.LoadModel("Models/megaHawlucha.obj");
	//calendario doom
	CalendarioDoom = Model();
	CalendarioDoom.LoadModel("Models/DoomsdayCalendar.obj");
	//penacho
	PenachoMan = Model();
	PenachoMan.LoadModel("Models/PenachoMan.obj");

	//alrededores
	ParedGaleria = Model();
	ParedGaleria.LoadModel("Models/BaseGal1.obj");
	ExpositorGaleria = Model();
	ExpositorGaleria.LoadModel("Models/BaseGal2.obj");
	Caballete1 = Model();
	Caballete1.LoadModel("Models/CaballeteGal1.obj");
	Caballete2 = Model();
	Caballete2.LoadModel("Models/CaballeteGal2.obj");

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

	//lamparas
	capoLampara = Model();
	capoLampara.LoadModel("Models/daCapo.obj");
	fuegoLampara = Model();
	fuegoLampara.LoadModel("Models/firelamp.obj");

	//Phantump
	PhantumpCabeza = Model();
	PhantumpCabeza.LoadModel("Models/PhantumpCabeza.obj");
	PhantumpCuerpo = Model();
	PhantumpCuerpo.LoadModel("Models/PhantumpCuerpo.obj");
	PhantumpBrazos = Model();
	PhantumpBrazos.LoadModel("Models/PhantumpBrazos.obj");

	//

	//Roland, carga de modelo
	RolandTorso.LoadModel("Models/RolandTorsoArt.obj");
	RolandBrazoDer = Model();
	RolandBrazoDer.LoadModel("Models/RolandBrazoDerArt.obj");
	RolandPiernaDer = Model();
	RolandPiernaDer.LoadModel("Models/RolandPiernaDerArt.obj");
	RolandBrazoIzq = Model();
	RolandBrazoIzq.LoadModel("Models/RolandBrazoIzqArt.obj");
	RolandPiernaIzq = Model();
	RolandPiernaIzq.LoadModel("Models/RolandPiernaIzqArt.obj");

	//Incineroar
	InciCabeza = Model();
	InciCabeza.LoadModel("Models/InciHead.obj");
	InciTorso = Model();
	InciTorso.LoadModel("Models/InciTorso.obj");
	InciBD = Model();
	InciBD.LoadModel("Models/InciBD.obj");
	InciPD = Model();
	InciPD.LoadModel("Models/InciPD.obj");
	InciBI = Model();
	InciBI.LoadModel("Models/InciBI.obj");
	InciPI = Model();
	InciPI.LoadModel("Models/InciPI.obj");
	InciCola = Model();
	InciCola.LoadModel("Models/InciCola.obj");

	//Protoman
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

	//Mawile
	MawileCabeza = Model();
	MawileCabeza.LoadModel("Models/MawileCabeza.obj");
	MawileCuerpo = Model();
	MawileCuerpo.LoadModel("Models/MawileCuerpo.obj");
	MawileBI = Model();
	MawileBI.LoadModel("Models/MawileBI1.obj");
	MawileMI = Model();
	MawileMI.LoadModel("Models/MawileBI2.obj");
	MawileBD = Model();
	MawileBD.LoadModel("Models/MawileBD1.obj");
	MawileMD = Model();
	MawileMD.LoadModel("Models/MawileBD2.obj");
	MawilePI = Model();
	MawilePI.LoadModel("Models/MawilePI.obj");
	MawilePD = Model();
	MawilePD.LoadModel("Models/MawilePD.obj");

	

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/right_dia.tga");
	skyboxFaces.push_back("Textures/Skybox/left_dia.tga");
	skyboxFaces.push_back("Textures/Skybox/bottom_dia.tga");
	skyboxFaces.push_back("Textures/Skybox/top_dia.tga");
	skyboxFaces.push_back("Textures/Skybox/front_dia.tga");
	skyboxFaces.push_back("Textures/Skybox/back_dia.tga");

	skybox = Skybox(skyboxFaces);

	//skybox de noche
	std::vector<std::string> skyboxFacesNoche;
	skyboxFacesNoche.push_back("Textures/Skybox/right_noche.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/left_noche.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/bottom_noche.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/top_noche.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/front_noche.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/back_noche.tga");

	skyboxNoche = Skybox(skyboxFacesNoche);


	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	
	float attenConst = 0.3f;
	float attenLin = 0.1f;
	float attenQuad = 0.05f;

	// Escenario 1 (Z): 4 Lamparas "capoLampara" (Amarillas)
	pointLights_Escenario1[0] = PointLight(1.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0, 0, 0, attenConst, attenLin, attenQuad);
	pointLights_Escenario1[1] = PointLight(1.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0, 0, 0, attenConst, attenLin, attenQuad);
	pointLights_Escenario1[2] = PointLight(1.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0, 0, 0, attenConst, attenLin, attenQuad);
	pointLights_Escenario1[3] = PointLight(1.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0, 0, 0, attenConst, attenLin, attenQuad);

	// Escenario 2 (X): 2 Altar (Blancas) + 2 Lamparas (Amarillas)
	pointLights_Escenario2[0] = PointLight(1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 0, 0, 0, attenConst, attenLin, attenQuad); // Altar 1
	pointLights_Escenario2[1] = PointLight(1.0f, 1.0f, 1.0f, 0.5f, 1.0f, 0, 0, 0, attenConst, attenLin, attenQuad); // Altar 2
	pointLights_Escenario2[2] = PointLight(1.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0, 0, 0, attenConst, attenLin, attenQuad); // Lampara 5
	pointLights_Escenario2[3] = PointLight(1.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0, 0, 0, attenConst, attenLin, attenQuad); // Lampara 6

	// Escenario 3 (C): 4 Antorchas "fuegoLampara" (Naranjas)
	pointLights_Escenario3[0] = PointLight(1.0f, 0.5f, 0.0f, 0.5f, 1.0f, 0, 0, 0, attenConst, attenLin, attenQuad);
	pointLights_Escenario3[1] = PointLight(1.0f, 0.5f, 0.0f, 0.5f, 1.0f, 0, 0, 0, attenConst, attenLin, attenQuad);
	pointLights_Escenario3[2] = PointLight(1.0f, 0.5f, 0.0f, 0.5f, 1.0f, 0, 0, 0, attenConst, attenLin, attenQuad);
	pointLights_Escenario3[3] = PointLight(1.0f, 0.5f, 0.0f, 0.5f, 1.0f, 0, 0, 0, attenConst, attenLin, attenQuad);

	//unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	//spotLightCount++;

	spotLights[1] = SpotLight(1.0f, 1.0f, 0.0f, // Luz Amarilla 
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f, // Posición (temporal)
		0.0f, -1.0f, 0.0f, // Dirección (temporal)
		0.1f, 0.05f, 0.02f, // Atenuación
		75.0f); // Ángulo

	spotLights[2] = SpotLight(0.0f, 1.0f, 1.0f, // Luz Cyan
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f, // Posición (temporal)
		0.0f, -1.0f, 0.0f, // Dirección (temporal)
		0.1f, 0.05f, 0.02f, // Atenuación
		45.0f); // Ángulo

	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	//Encender luces nocturnas
	bool lucesNocturnasEncendidas = false;

	printf("Controles:\n");
	printf("WASD - Mover cámara\n");
	//modos de camara
	printf("1 - Modo Principal (Primera Persona)\n");
	printf("2 - Modo Aereo\n");
	printf("3 - Modo Estatico\n");
	printf("4 - Modo Estatico 2\n");
	printf("5 - Modo Estatico 3\n");

	//Prender Luces
	printf("Solo de noche se prenden las luces\n");
	printf("Z para luces entrada\n");
	printf("X para luces altar\n");
	printf("C para luces antorchas\n");


	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::mat4 modelPiso(1.0);
	glm::mat4 elementos(1.0);
	glm::mat4 elementoLocal(1.0);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
	glm::mat4 baseInc(1.0);
	glm::mat4 baseProto(1.0);
	glm::mat4 brazoProto(1.0);
	glm::mat4 basePhantump(1.0);
	glm::mat4 baseMawile(1.0);
	glm::mat4 model3(1.0f);
	glm::mat4 model4(1.0f);
	glm::mat4 herenciaMa(1.0f);
	glm::mat4 herenciaMa1(1.0f);

	//Función miniaudio
	// Inicializar motor de audio
	if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
		std::cout << "Error inicializando miniaudio" << std::endl;
		return -1;
	}
	//Carga de sonidos desde carpeta audio
	//Ring ambiental
	if (ma_sound_init_from_file(&engine, "audio/Ring_Ambiental.wav",
		MA_SOUND_FLAG_NO_SPATIALIZATION, NULL, NULL, &ambientalRing) != MA_SUCCESS) {
		std::cout << "Error cargando sonido del ring\n"; //Excepción para capturar error al momento de cargar audio ring
	}
	ma_sound_set_looping(&ambientalRing, MA_TRUE);
	ma_sound_set_volume(&ambientalRing, 0.0f);   // Inicializado en 0 hasta que se acerque el usuario
	ma_sound_start(&ambientalRing);

	// Ofrenda ambiental
	if (ma_sound_init_from_file(&engine, "audio/Galeria_Ambiental.wav",
		MA_SOUND_FLAG_NO_SPATIALIZATION, NULL, NULL, &ambientalgaleria) != MA_SUCCESS) {
		std::cout << "Error cargando sonido de la ofrenda\n"; //Captura de excepción
	}
	ma_sound_set_looping(&ambientalgaleria, MA_TRUE);
	ma_sound_set_volume(&ambientalgaleria, 0.0f); // Volumen inicializado en 0
	ma_sound_start(&ambientalgaleria);
	// Soundtrack
	if (ma_sound_init_from_file(&engine, "audio/Soundtrack.wav", MA_SOUND_FLAG_DECODE, NULL, NULL, &sonidoFondo) != MA_SUCCESS) {
		std::cout << "Error cargando archivo de sonido" << std::endl;
	}
	ma_sound_set_looping(&sonidoFondo, MA_TRUE);   // Música infinita
	ma_sound_start(&sonidoFondo);                  // Reproducir

	// SONIDOS DE EFECTOS
	// Efecto Protoman
	if (ma_sound_init_from_file(&engine, "audio/ProtomanFX.wav",
		MA_SOUND_FLAG_DECODE, NULL, NULL, &efectoProtoman) != MA_SUCCESS) {
		std::cout << "Error cargando sonido ProtoFX\n";
	}

	// Efecto Angela
	if (ma_sound_init_from_file(&engine, "audio/AngelaFX.wav",
		MA_SOUND_FLAG_DECODE, NULL, NULL, &efectoAngela) != MA_SUCCESS) {
		std::cout << "Error cargando sonido AngelaFX\n";
	}
	static bool protoplay = false;

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{	
		
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//*******************ANIMACIONES*********************************
		//*******************ANIMACIONES*********************************
				//Animacion de la puerta
		if (mainWindow.getEntradaAbierta())
		{
			// --- ABRIR PUERTAS ---
			// Interpolar suavemente hacia los valores objetivo
			TpuertaE = lerp(TpuertaE, TpuertaE_Target_X, velocidadPuerta * deltaTime);
			TpuertaE_Z = lerp(TpuertaE_Z, AjusteP, velocidadPuerta * deltaTime);
		}
		else
		{
			// --- CERRAR PUERTAS ---
			// Interpolar suavemente de vuelta a posicion original
			TpuertaE = lerp(TpuertaE, 0.0f, velocidadPuerta * deltaTime);
			TpuertaE_Z = lerp(TpuertaE_Z, 0.0f, velocidadPuerta * deltaTime);
		}

		//Puertas Ring
		if (mainWindow.getEstadoRing()) {
			// --- ABRIR PUERTAS ---
			Rpuerta = lerp(Rpuerta, RpuertaDer_Target, velocidadPuerta * deltaTime);
		}
		else {
			// --- CERRAR PUERTAS ---
			Rpuerta = lerp(Rpuerta, 0.0f, velocidadPuerta * deltaTime);
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
			currentCameraMode = 1;
		}
		if (keys[GLFW_KEY_2])
		{
			camera.setCameraMode(2); // Modo Aéreo
			currentCameraMode = 2;
		}
		if (keys[GLFW_KEY_3])
		{
			camera.setCameraMode(3); // Modo Estático
			currentCameraMode = 3;
		}
		if (keys[GLFW_KEY_4])
		{
			camera.setCameraMode(4); // Modo Estático
			currentCameraMode = 4;
		}
		if (keys[GLFW_KEY_5])
		{
			camera.setCameraMode(5); // Modo Estático
			currentCameraMode = 5;
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



		//Captura de barra espaciadora para animacion Roland

		// --- Control de inicio/parada de caminado ---
		if (mainWindow.getsKeys()[GLFW_KEY_W] || mainWindow.getsKeys()[GLFW_KEY_S])
		{
			caminarRoland = true;
		}
		else
		{
			caminarRoland = false;


		}


		// --- Actualizacion de animacion Roland ---
		if (caminarRoland)
		{
			// Actualiza fase de caminata
			anguloMovimiento += velocidadPaso * deltaTime;

			// Movimiento oscilante tipo seno
			rotacionBrazoDer = 35.0f * sin(anguloMovimiento);
			rotacionBrazoIzq = -35.0f * sin(anguloMovimiento);
			rotacionPiernaDer = -35.0f * sin(anguloMovimiento);
			rotacionPiernaIzq = 35.0f * sin(anguloMovimiento);

		}

		if (animacionInci)
		{
			tiempolocal += deltaTime * 0.01;
			// Aumenté la velocidad para que coincida con el nuevo tamaño del recorrido
			float inciSpeed = 0.0035f * deltaTime * 60.0f;

			// Coordenadas del recorrido local (alrededor del 0,0 del ring)
			float minXZ = -15.0f;
			float maxXZ = 15.0f;

			// Estado 1: avanza +x local
			if (recorrido == 1) {
				pos_ini_x_inci += inciSpeed;
				orienta = 90.0f;

				// Brazos subiendo (fase positiva)
				rotacionBrazoDerInci = -165.0f * 0.5f * (1.0f + sin(tiempolocal * velocidadbrazos));
				rotacionBrazoIzqInci = -165.0f * 0.5f * (1.0f + sin(tiempolocal * velocidadbrazos));

				// Piernas caminan alternadas
				rotacionPiernaDerInci = 30.0f * sin(tiempolocal * velocidadpiernas);
				rotacionPiernaIzqInci = -30.0f * sin(tiempolocal * velocidadpiernas);

				if (pos_ini_x_inci >= maxXZ) {
					pos_ini_x_inci = maxXZ; // Ajustar a la esquina
					recorrido = 2;
					tiempolocal = 0.0f;
				}
			}

			//Estado 2: avanza +z local
			else if (recorrido == 2) {
				pos_ini_z_inci += inciSpeed;
				orienta = 0.0f;

				// Brazos bajando (fase invertida)
				rotacionBrazoDerInci = -165.0f * 0.5f * (1.0f - sin(tiempolocal * velocidadbrazos));
				rotacionBrazoIzqInci = -165.0f * 0.5f * (1.0f - sin(tiempolocal * velocidadbrazos));

				rotacionPiernaDerInci = 30.0f * sin(tiempolocal * velocidadpiernas);
				rotacionPiernaIzqInci = -30.0f * sin(tiempolocal * velocidadpiernas);

				if (pos_ini_z_inci >= maxXZ) {
					pos_ini_z_inci = maxXZ; // Ajustar a la esquina
					recorrido = 3;
					tiempolocal = 0.0f;
				}
			}

			// Estado 3: avanza -x local
			else if (recorrido == 3) {
				pos_ini_x_inci -= inciSpeed;
				orienta = -90.0f;

				// Brazos subiendo de nuevo
				rotacionBrazoDerInci = -165.0f * 0.5f * (1.0f + sin(tiempolocal * velocidadbrazos));
				rotacionBrazoIzqInci = -165.0f * 0.5f * (1.0f + sin(tiempolocal * velocidadbrazos));

				rotacionPiernaDerInci = 30.0f * sin(tiempolocal * velocidadpiernas);
				rotacionPiernaIzqInci = -30.0f * sin(tiempolocal * velocidadpiernas);

				if (pos_ini_x_inci <= minXZ) {
					pos_ini_x_inci = minXZ; // Ajustar a la esquina
					recorrido = 4;
					tiempolocal = 0.0f;
				}
			}

			// Estado 4: avanza -z local
			else if (recorrido == 4) {
				pos_ini_z_inci -= inciSpeed;
				orienta = 180.0f;

				// Brazos bajando (fase invertida)
				rotacionBrazoDerInci = -165.0f * 0.5f * (1.0f - sin(tiempolocal * velocidadbrazos));
				rotacionBrazoIzqInci = -165.0f * 0.5f * (1.0f - sin(tiempolocal * velocidadbrazos));

				rotacionPiernaDerInci = 30.0f * sin(tiempolocal * velocidadpiernas);
				rotacionPiernaIzqInci = -30.0f * sin(tiempolocal * velocidadpiernas);

				if (pos_ini_z_inci <= minXZ) {
					pos_ini_z_inci = minXZ; // Ajustar a la esquina
					recorrido = 1;
					tiempolocal = 0.0f;
				}
			}
		}
		//Animación Phantump
		
		if (mainWindow.getanimPhantump() && !subiendo && !bajando) {
			subiendo = true;
		}

		// Fase ascendente
		if (subiendo) {
			rotacion_brazos -= 2.5f * deltaTime;
			elevacion_cuerpo += 0.1f * deltaTime;
			ladeo_cabeza -= 0.25f * deltaTime;

			if (elevacion_cuerpo >= altura_max) {
				subiendo = false;
				bajando = true;
			}
		}

		// Fase descendente
		if (bajando) {
			ladeo_cabeza += 0.25f * deltaTime;
			rotacion_brazos += 2.5f * deltaTime;
			elevacion_cuerpo -= 0.05f * deltaTime;
			
			if (elevacion_cuerpo <= 0.0f) {
				bajando = false;

				// Detenemos anim
				mainWindow.setanimPhantump(false);
				rotacion_brazos = 0.0f;
				ladeo_cabeza = 0.0f;
				elevacion_cuerpo = 0.0f;
			}
		}



		//Animación Mawile
		
		if (mainWindow.getanimMawile() && estadoma == 0) {
			estadoma = 1;
		}
		if (estadoma == 1) {
		//Levanta sus manos
			rotacion_brazos_ma += 30.0f * deltaTime * 0.04f;
			if (rotacion_brazos_ma >= 90.0f) {
				rotacion_brazos_ma = 90.0f;
				estadoma = 2;
			}
		}
		if (estadoma == 2) {
			rotacion_manos_ma += 40.0f * deltaTime * 0.04f;
			if (rotacion_manos_ma >= 90.0f) {
				rotacion_manos_ma = 90.0f;
				tiempollora = 0.0f;
				estadoma = 3;
			}

		}
		if (estadoma == 3) {
			tiempollora += deltaTime;
			if (si) {
				rotacion_cab_si_ma += 2.0f * deltaTime * 0.04f;
				if (rotacion_cab_si_ma >= 5.0f) {
					si = false;
				}
			else {
					rotacion_cab_si_ma -= 2.0f * deltaTime * 0.04f;
					if (rotacion_cab_si_ma <= -5.0f) {
						si = true;
					}
				}
			}
			if (no) {
				rotacion_cab_no_ma += 1.50f * deltaTime * 0.04f;
				if (rotacion_cab_no_ma >= 5.0f) {
					no = false;
				}
			}
			else {
					rotacion_cab_no_ma -= 1.50f * deltaTime * 0.04f;
					if (rotacion_cab_no_ma <= -5.0f) {
						no = true;
					}
				}
			if (tiempollora >= 9.0f) {
				estadoma = 4;
			
			}
			
		}
		if (estadoma == 4) {
			rotacion_brazos_ma -= 30.0f * deltaTime * 0.04f;
			rotacion_manos_ma -= 40.0f * deltaTime * 0.04f;

			// Enderezar cabeza
			if (rotacion_cab_si_ma > 0)
				rotacion_cab_si_ma -= 10.0f * deltaTime * 0.04f;
			else if (rotacion_cab_si_ma < 0)
				rotacion_cab_si_ma += 10.0f * deltaTime * 0.04f;

			if (rotacion_cab_no_ma > 0)
				rotacion_cab_no_ma -= 1.0f * deltaTime * 0.04f;
			else if (rotacion_cab_no_ma < 0)
				rotacion_cab_no_ma += 1.0f * deltaTime * 0.04f;

			// Cuando vuelve a la pose inicial
			if (rotacion_brazos_ma <= 0.0f && rotacion_manos_ma <= 0.0f) {
				rotacion_brazos_ma = 0.0f;
				rotacion_manos_ma = 0.0f;
				rotacion_cab_si_ma = 0.0f;
				rotacion_cab_no_ma = 0.0f;
				estadoma = 0; // vuelve al estado inactivo
				mainWindow.setanimMawile(false); // apagar trigger
			}
		}

		//Animacion Protoman
		if (mainWindow.getEstadoProto()) {
			//Inicia efecto de sonido
			if (!protoplay) {
				ma_sound_seek_to_pcm_frame(&efectoProtoman, 0);   // Reinicia audio desde el inicio
				ma_sound_start(&efectoProtoman);                  // Reproduce
				protoplay = true;                         // Evitar repetir
			}
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
			protoplay = false; //Apaga sonido, inicializa de nuevo la variable que enciende el audio.
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
		//uniformTextureOffset = shaderList[0].getOffsetLocation();

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
		shaderList[0].SetDirectionalLight(&mainLight);
		int lightMode = mainWindow.getLightMode();
		if (lucesNocturnasEncendidas)
		{
			switch (lightMode)
			{
			case 1: // (Tecla Z)
				shaderList[0].SetPointLights(pointLights_Escenario1, 4);
				break;
			case 2: // (Tecla X)
				shaderList[0].SetPointLights(pointLights_Escenario2, 4);
				break;
			case 3: // (Tecla C)
				shaderList[0].SetPointLights(pointLights_Escenario3, 4);
				break;
			default: // Apagadas
				shaderList[0].SetPointLights(NULL, 0);
				break;
			}
		}
		else // Es de día
		{
			shaderList[0].SetPointLights(NULL, 0);
		}
		//shaderList[0].SetSpotLights(spotLights, spotLightCount); 
		
		SpotLight activeSpotLights[MAX_SPOT_LIGHTS];
		unsigned int activeSpotLightCount = 0;
		activeSpotLights[activeSpotLightCount++] = spotLights[0];

		// Si es de noche, añadir las 2 luces fijas
		if (lucesNocturnasEncendidas)
		{
			activeSpotLights[activeSpotLightCount++] = spotLights[1];
			activeSpotLights[activeSpotLightCount++] = spotLights[2];
		}
		

		shaderList[0].SetSpotLights(activeSpotLights, activeSpotLightCount);

		// Calculo de distancias para audio ambiental.

		glm::vec3 posRing = glm::vec3(158.5f, 0.0f, 39.0f); //Heredamos la posicion del ring

		glm::vec3 posOfrenda = glm::vec3(113.0f, 0.0f, -17.0f); // Heredamos posicion de la galeria


		glm::vec3 posJugador = camera.getCameraPosition();

		// Distancia al ring
		float distRing = glm::distance(posJugador, posRing);

		// Distancia a ofrenda
		float distOfrenda = glm::distance(posJugador, posOfrenda);

		// Escala de distancia
		float maxDist = 50.0f;

		// Volumen local normalizado
		float volRing = 1.0f - glm::clamp(distRing / maxDist, 0.0f, 1.0f);
		float volOfrenda = 1.0f - glm::clamp(distOfrenda / maxDist, 0.0f, 1.0f);

		// Aplicar volumen a sonidos espaciales
		ma_sound_set_volume(&ambientalRing, volRing);
		ma_sound_set_volume(&ambientalgaleria, volOfrenda);

		// Reducir soundtrack al acercarse a cualquiera
		float reduccion = (volRing > volOfrenda ? volRing : volOfrenda);
		float volSoundtrack = 1.0f - 0.6f * reduccion;
		ma_sound_set_volume(&sonidoFondo, volSoundtrack);

		// --- FIN DE LÓGICA DE LUCES ---

		//Piso modelado con Blender
		modelPiso = glm::mat4(1.0);
		modelPiso = glm::translate(modelPiso, glm::vec3(40.0f, -2.5f, 0.0f));
		//color que afecta a todos los elementos
		color = glm::vec3(0.6f, 0.6f, 0.6f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPiso));
		Piso_M.RenderModel();

		//Recepcion
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(43.0f, -2.0f, -15.0f));
		elementos = glm::scale(elementos, glm::vec3(1.3f, 1.3f, 1.3f));
		elementos = glm::rotate(elementos, 30 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementoLocal = elementos;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Angela_Desk.RenderModel();

		elementos = glm::translate(elementos, glm::vec3(0.0f, 0.0f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Angela_Chair.RenderModel();

		//Ofrenda
		elementos = glm::mat4(1.0);
		//elementos = glm::translate(elementos, glm::vec3(100.0f, -2.0f, -60.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		elementos = glm::translate(elementos, glm::vec3(113.0f, -2.0f, -17.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		pointLights_Escenario2[0].SetPos(glm::vec3(elementos[3]) + glm::vec3(-0.5f,0.5f,-0.5f));
		pointLights_Escenario2[1].SetPos(glm::vec3(elementos[3]) + glm::vec3(0.5f, 0.5f, 0.5f));
		elementos = glm::rotate(elementos, -75 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementos = glm::scale(elementos, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Ofrenda.RenderModel();

		//Ubicacion central de la piramide
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(158.5f, -2.0f, 39.0f));
		spotLights[1].SetFlash(glm::vec3(elementos[3]) + glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		elementos = glm::rotate(elementos, -125 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementoLocal = elementos;
		elementos = glm::translate(elementos, glm::vec3(0.0f, -1.0f, 0.0f));
		//elementos = glm::translate(elementos, glm::vec3(125.5f, -2.0f, 25.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		//elementos = glm::rotate(elementos, -125 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		PiramideRing.RenderModel();

		//Ring de lucha
		elementos = elementoLocal;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Ring.RenderModel();

		// INCINEROAR
		// 1. Empezar desde la matriz del ring (que ya está girada y en -2.0f Y)
		baseInc = elementoLocal;

		// 2. Aplicar el movimiento local (X, Z) y ajustar Y para ponerlo en el suelo (-2.5f)
		//    (El ring está en -2.0, así que bajamos -0.5 más)
		baseInc = glm::translate(baseInc, glm::vec3(pos_ini_x_inci, 3.5f, pos_ini_z_inci));

		// 3. Aplicar la orientación local del modelo
		baseInc = glm::rotate(baseInc, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));

		// 4. Aplicar la escala del modelo
		baseInc = glm::scale(baseInc, glm::vec3(0.025f, 0.025f, 0.025f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(baseInc));
		InciTorso.RenderModel();

		//Cabeza
		glm::mat4 model1 = baseInc;
		model1 = glm::translate(model1, glm::vec3(0.0f, 19.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model1));
		InciCabeza.RenderModel();

		//BD
		model1 = baseInc;
		model1 = glm::translate(model1, glm::vec3(-42.9f, -14.0f, 2.0f));
		model1 = glm::rotate(model1, glm::radians(rotacionBrazoDerInci), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model1));
		InciBD.RenderModel();

		//BI
		model1 = baseInc;
		model1 = glm::translate(model1, glm::vec3(42.9f, -14.0f, 2.0f));
		model1 = glm::rotate(model1, glm::radians(rotacionBrazoIzqInci), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model1));
		InciBI.RenderModel();

		//pD
		model1 = baseInc;
		model1 = glm::translate(model1, glm::vec3(-12.0f, -73.0f, -2.0f));
		model1 = glm::rotate(model1, glm::radians(rotacionPiernaDerInci), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model1));
		InciPD.RenderModel();

		//pI
		model1 = baseInc;
		model1 = glm::translate(model1, glm::vec3(12.0f, -73.0f, -2.0f));
		model1 = glm::rotate(model1, glm::radians(rotacionPiernaIzqInci), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model1));

		InciPI.RenderModel();

		//COLA
		model1 = baseInc;
		model1 = glm::translate(model1, glm::vec3(0.0f, -93.0f, -78.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model1));
		InciCola.RenderModel();

		//MegaHawlucha
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(70.0f, -2.0f, 0.0f));
		spotLights[2].SetFlash(glm::vec3(elementos[3]) + glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		elementoLocal = elementos;
		elementos = glm::rotate(elementos, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		MegaHawlucha.RenderModel();

		//Calendario
		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(20.0f, 0.0f, -1.0f));
		elementos = glm::rotate(elementos, -75 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		CalendarioDoom.RenderModel();

		//Penacho (con base)
		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(-20.0f, 0.0f, 16.0f));
		elementos = glm::rotate(elementos, 150 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		PenachoMan.RenderModel();

		//Paredes exposicion
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(90.0f, -2.0f, 17.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		elementoLocal = elementos;
		elementos = glm::rotate(elementos, 160 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		ParedGaleria.RenderModel();

		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(10.0f, 0.0f, 0.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		elementos = glm::rotate(elementos, 200 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		ParedGaleria.RenderModel();

		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(-10.0f, 0.0f, 0.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		elementos = glm::rotate(elementos, 200 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		ParedGaleria.RenderModel();

		//FILA DE caballetes.
		//1
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(54.0f, -2.0f, -24.0f));
		modelaux = elementos;
		elementos = glm::translate(elementos, glm::vec3(0, 0.0f, 3.0f));
		elementos = glm::scale(elementos, glm::vec3(1.0f, 1.3f, 1.0f));
		elementos = glm::rotate(elementos, 15 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		//textura de cuadro en cuestion
		Caballete1.RenderModel();

		//2
		elementos = modelaux;
		elementos = glm::translate(elementos, glm::vec3(5.0f, 0.0f, 0.0f));
		modelaux = elementos;
		elementos = glm::translate(elementos, glm::vec3(0.0f, 0.0f, 2.0f));
		elementos = glm::scale(elementos, glm::vec3(1.5f, 1.5f, 1.5f));
		elementos = glm::rotate(elementos, -20 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		//textura de cuadro en cuestion
		Caballete2.RenderModel();

		//3
		elementos = modelaux;
		elementos = glm::translate(elementos, glm::vec3(4.0f, 0.0f, 0.0f));
		modelaux = elementos;
		elementos = glm::scale(elementos, glm::vec3(0.8f, 0.8f, 0.8f));
		elementos = glm::rotate(elementos, 10 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		//textura de cuadro en cuestion
		Caballete2.RenderModel();

		//4
		elementos = modelaux;
		elementos = glm::translate(elementos, glm::vec3(4.0f, 0.0f, 0.0f));
		modelaux = elementos;
		elementos = glm::rotate(elementos, 15 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		//textura de cuadro en cuestion
		Caballete1.RenderModel();

		//5
		elementos = modelaux;
		elementos = glm::translate(elementos, glm::vec3(4.0f, 0.0f, 0.0f));
		modelaux = elementos;
		elementos = glm::scale(elementos, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		//textura de cuadro en cuestion
		Caballete1.RenderModel();

		//6
		elementos = modelaux;
		elementos = glm::translate(elementos, glm::vec3(6.0f, 0.0f, 0.0f));
		modelaux = elementos;
		elementos = glm::scale(elementos, glm::vec3(1.0f, 1.3f, 1.5f));
		elementos = glm::rotate(elementos, -10 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		//textura de cuadro en cuestion
		Caballete2.RenderModel();

		//7
		elementos = modelaux;
		elementos = glm::translate(elementos, glm::vec3(4.0f, 0.0f, 0.0f));
		modelaux = elementos;
		elementos = glm::scale(elementos, glm::vec3(0.8f, 0.8f, 0.8f));
		elementos = glm::rotate(elementos, 10 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		//textura de cuadro en cuestion
		Caballete1.RenderModel();

		//8
		elementos = modelaux;
		elementos = glm::translate(elementos, glm::vec3(5.0f, 0.0f, 0.0f));
		modelaux = elementos;
		elementos = glm::rotate(elementos, 15 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		//textura de cuadro en cuestion
		Caballete2.RenderModel();

		//9
		elementos = modelaux;
		elementos = glm::translate(elementos, glm::vec3(5.0f, 0.0f, 0.0f));
		modelaux = elementos;
		elementos = glm::scale(elementos, glm::vec3(1.0f, 1.4f, 1.0f));
		elementos = glm::rotate(elementos, -20 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		//textura de cuadro en cuestion
		Caballete1.RenderModel();


		//------------------------LAMPARAS------------------------------------------------
		//entrada al ring
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(127.0f, -2.0f, 13.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		pointLights_Escenario3[0].SetPos(glm::vec3(elementos[3]) + glm::vec3(0.0f, 1.0f, 0.0f));
		elementos = glm::rotate(elementos, -30 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = elementos;
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		fuegoLampara.RenderModel();

		elementos = modelaux;
		elementos = glm::translate(elementos, glm::vec3(0.0f, 0.0f, 11.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		pointLights_Escenario3[1].SetPos(glm::vec3(elementos[3]) + glm::vec3(0.0f, 1.0f, 0.0f));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		fuegoLampara.RenderModel();


		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(136.0f, -2.0f, 17.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		pointLights_Escenario3[2].SetPos(glm::vec3(elementos[3]) + glm::vec3(0.0f, 1.0f, 0.0f));
		elementos = glm::rotate(elementos, -30 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = elementos;
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		fuegoLampara.RenderModel();

		elementos = modelaux;
		elementos = glm::translate(elementos, glm::vec3(0.0f, 0.0f, 11.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		pointLights_Escenario3[3].SetPos(glm::vec3(elementos[3]) + glm::vec3(0.0f, 1.0f, 0.0f));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		fuegoLampara.RenderModel();

		//lamparas de por ahi
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(36.0f, -2.0f, -5.5f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		pointLights_Escenario1[0].SetPos(glm::vec3(elementos[3]) + glm::vec3(0.0f, 1.0f, 0.0f));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		capoLampara.RenderModel();

		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(36.0f, -2.0f, 7.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		pointLights_Escenario1[1].SetPos(glm::vec3(elementos[3]) + glm::vec3(0.0f, 1.0f, 0.0f));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		capoLampara.RenderModel();

		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(60.0f, -2.0f, 17.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		pointLights_Escenario1[2].SetPos(glm::vec3(elementos[3]) + glm::vec3(0.0f, 1.0f, 0.0f));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		capoLampara.RenderModel();

		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(50.0f, -2.0f, -23.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		pointLights_Escenario1[3].SetPos(glm::vec3(elementos[3]) + glm::vec3(0.0f, 1.0f, 0.0f));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		capoLampara.RenderModel();

		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(90.0f, -2.0f, 22.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		pointLights_Escenario2[3].SetPos(glm::vec3(elementos[3]) + glm::vec3(0.0f, 1.0f, 0.0f));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		capoLampara.RenderModel();

		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(80.0f, -2.0f, -26.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		pointLights_Escenario2[2].SetPos(glm::vec3(elementos[3]) + glm::vec3(0.0f, 1.0f, 0.0f));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		capoLampara.RenderModel();


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
		elementos = glm::rotate(elementos, Rpuerta * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//elementos = glm::rotate(elementos, posXPuertaR, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		PuertaDerR.RenderModel();

		//puerta rotatoria izq
		elementos = modelaux;
		elementos = glm::translate(elementos, glm::vec3(-6.0f, 0.0f, 2.2f));
		elementos = glm::scale(elementos, glm::vec3(1.15f, 1.3f, 1.0f));
		elementos = glm::rotate(elementos, -Rpuerta * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//elementos = glm::rotate(elementos, posXPuertaR * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		PuertaIzqR.RenderModel();

		//------------------------------------------

		// === HUMANOIDE ROLAND ===

		if (currentCameraMode == 1)
		{
			// 1. Obtener vectores de la cámara
			glm::vec3 camPos = camera.getCameraPosition();
			glm::vec3 camDir = camera.getCameraDirection();

			// 2. Calcular dirección XZ (plana)
			glm::vec3 avatarForward = glm::vec3(camDir.x, 0.0f, camDir.z);

			// 3. Definir offset y nivel del suelo
			GLfloat avatarDist = 2.0f;
			GLfloat avatarYLevel = 2.0f; // Nivel del suelo

			// 4. Normalizar y calcular rotación (Yaw)
			if (glm::length(avatarForward) > 0.001f)
			{
				avatarForward = glm::normalize(avatarForward);
				rolandAvatarYaw = atan2(avatarForward.x, avatarForward.z) + M_PI;
			}
			// (Si no, rolandAvatarYaw mantiene su último valor)

			// 5. Calcular la posición final del avatar y guardarla
			rolandAvatarPos = camPos + avatarForward * avatarDist;
			rolandAvatarPos.y = avatarYLevel; // Fijar al suelo
		}

		// --- DIBUJADO DE ROLAND ---
		// Dibuja a Roland CADA FRAME usando la última posición/rotación guardada

		// 6. Crear la matriz 'base' (model) para Roland
		glm::mat4 base = glm::mat4(1.0f);
		base = glm::translate(base, rolandAvatarPos); // Usar la posición guardada
		base = glm::rotate(base, rolandAvatarYaw, glm::vec3(0.0f, 1.0f, 0.0f)); // Usar la rotación guardada
		base = glm::scale(base, glm::vec3(3.0f, 3.0f, 3.0f)); // Aplicar escala

		// 7. Dibujar el Torso (base)
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(base));
		RolandTorso.RenderModel();

		// ---- DIBUJAR MIEMBROS (animaciones de caminata) ----

		//Brazo Derecho
		glm::mat4 model0 = base;
		model0 = glm::translate(model0, glm::vec3(0.17694f, -0.064725f, 0.035086f));
		model0 = glm::rotate(model0, glm::radians(rotacionBrazoDer), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model0));
		RolandBrazoDer.RenderModel();

		//Brazo Izquierdo
		model0 = base;
		model0 = glm::translate(model0, glm::vec3(-0.17694f, -0.067725f, 0.017086f));
		model0 = glm::rotate(model0, glm::radians(rotacionBrazoIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model0));
		RolandBrazoIzq.RenderModel();

		//Pierna Derecha
		model0 = base;
		model0 = glm::translate(model0, glm::vec3(0.071255f, -0.582795f, -0.01924f));
		model0 = glm::rotate(model0, glm::radians(rotacionPiernaDer), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model0));
		RolandPiernaDer.RenderModel();

		//Pierna Izquierda
		model0 = base;
		model0 = glm::translate(model0, glm::vec3(-0.071255f, -0.582795f, -0.01924f));
		model0 = glm::rotate(model0, glm::radians(rotacionPiernaIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model0));
		RolandPiernaIzq.RenderModel();

		//------------------------------------------

		// = PROTOMAN =
		//Posicionar torso
		baseProto = glm::mat4(1.0f);
		baseProto = glm::translate(baseProto, glm::vec3(-15.0f, 0.6f, 20.0f + posicionProto));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(baseProto));
		ProtoTorso.RenderModel();

		//Cabeza
		modelaux = baseProto;
		modelaux = glm::translate(modelaux, glm::vec3(0.0f, 0.56f, 0.0f));
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
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		ProtoIzqBrazo.RenderModel();

		//Pierna Derecha
		modelaux = baseProto;
		modelaux = glm::translate(modelaux, glm::vec3(-0.17f, -0.53f, -0.13f));
		modelaux = glm::rotate(modelaux, piernaProto * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		ProtoDerPierna.RenderModel();

		//Pierna Izquierda
		modelaux = baseProto;
		modelaux = glm::translate(modelaux, glm::vec3(0.25f, -0.53f, -0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		ProtoIzqPierna.RenderModel();

		//------------------------------------------------------------------------------------------------
		//Modelo Phantump

		//Cuerpo, torso como base
		basePhantump = glm::mat4(1.0f);
		basePhantump = glm::translate(basePhantump, glm::vec3(pos_ini_x_pha, pos_ini_y_pha + elevacion_cuerpo, pos_ini_z_pha));
		basePhantump = glm::scale(basePhantump, glm::vec3(0.1f,0.1f,0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(basePhantump));
		PhantumpCuerpo.RenderModel();

		//Brazos
		model3 = basePhantump;
		model3= glm::translate(model3, glm::vec3(0.0f, 0.953284f, 14.9372f));
		model3 = glm::rotate(model3, glm::radians(rotacion_brazos), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model3));
		PhantumpBrazos.RenderModel();

		//Cabeza
		model3 = basePhantump;
		model3 = glm::translate(model3, glm::vec3(0.0f, 23.2911 + elevacion, 13.0059f));
		model3 = glm::rotate(model3, glm::radians(ladeo_cabeza), glm::vec3(0.0f,0.0f,1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model3));
		PhantumpCabeza.RenderModel();



		//-------------------------------------------------------------------------------------------------
		//Modelo Mawile
		baseMawile = glm::mat4(1.0f);
		baseMawile = glm::translate(baseMawile, glm::vec3(pos_ini_x_ma, pos_ini_y_ma, pos_ini_z_ma));
		baseMawile = glm::scale(baseMawile, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(baseMawile));
		MawileCuerpo.RenderModel();

		//Brazos Izq
		herenciaMa = baseMawile;
		herenciaMa = glm::translate(herenciaMa, glm::vec3(10.2406f, -1.70517f, -0.732285f));
		herenciaMa = glm::rotate(herenciaMa, glm::radians(-rotacion_brazos_ma), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(herenciaMa));
		MawileBI.RenderModel();

		model4 = herenciaMa;
		model4 = glm::translate(model4, glm::vec3(17.9444f, -10.85983f, 0.05895f));
		model4 = glm::rotate(model4, glm::radians(-rotacion_manos_ma), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model4));
		MawileMI.RenderModel();

		//Cabeza
		model4 = baseMawile;
		model4 = glm::translate(model4, glm::vec3(0.0f, 23.7079f, -30.5368f));
		model4 = glm::rotate(model4, glm::radians(-rotacion_cab_si_ma), glm::vec3(1.0f, 0.0f, 0.0f));
		model4 = glm::rotate(model4, glm::radians(-rotacion_cab_no_ma), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model4));
		MawileCabeza.RenderModel();

		//Brazos Der
		herenciaMa1 = baseMawile;
		herenciaMa1 = glm::translate(herenciaMa1, glm::vec3(-10.2406f, -1.70517f, -0.732285f));
		herenciaMa1 = glm::rotate(herenciaMa1, glm::radians(-rotacion_brazos_ma), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(herenciaMa1));
		MawileBD.RenderModel();

		model4 = herenciaMa1;
		model4 = glm::translate(model4, glm::vec3(-17.9444f, -10.85983f, 0.05895f));
		model4 = glm::rotate(model4, glm::radians(rotacion_manos_ma), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model4));
		MawileMD.RenderModel();

		//Piernas

		model4 = baseMawile;
		model4 = glm::translate(model4, glm::vec3(10.3968f, -31.4319f, -0.196301f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model4));
		MawilePI.RenderModel();

		model4 = baseMawile;
		model4 = glm::translate(model4, glm::vec3(-10.3968f, -31.4319f, -0.196301f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model4));
		MawilePD.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}
	//Llamamos funciones miniaudio para liberar memoria

	//ma_sound_uninit(&sonidoFondo);
	//ma_engine_uninit(&engine);
	return 0;
}
