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
#include "Animaciones.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
//Implementación librería miniaudio
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
Model Arboles1;

Model Angela_Desk, Angela_Chair;
Model Ofrenda;
Model PiramideRing, Ring;

//GALERIA
//centro
Model CalendarioDoom, PenachoMan, MegaHawlucha;

//alrededores
Model ParedGaleria, ExpositorGaleria;
Model Caballete1, Caballete2;
Model Lienzo1, Lienzo2, Lienzo3;
Model Lienzo4, Lienzo5, Lienzo6;
Model Tomahawk, MewFossil, BongBong;

//modelos y texturas auxiliares
Model capoLampara, fuegoLampara;

Model PuertaDerE, PuertaIzqE, PilaresE, LetreroM;
Model ArcoRing, PuertaDerR, PuertaIzqR;

//animaciones keyframes
Model BaseDig, CabinaDig, BrazoDig;
Model AntebrazoDig, GarraDig;

Model TroncoPoke, BaseCopaPoke, BayaPoke, CopaPoke;

Model MaquinaExpendedora, BotonExpend;
Model BandejaExpend, LataRefresco;
Texture refrescoCherry, refrescoRegular, refrescoGrape;

//PERSONAJES
//Roland
Model RolandTorso, RolandBrazoDer, RolandBrazoIzq;
Model RolandPiernaDer, RolandPiernaIzq;

//Phantump
Model PhantumpCabeza, PhantumpCuerpo;
Model PhantumpBrazos, PhantumpCola;

//Incineroar
Model InciCabeza, InciTorso, InciCola;
Model InciBrazoD, InciAnteD, InciBrazoL, InciAnteL;
Model InciPiernaD, InciPiernaL;

//Protoman
Model ProtoCabeza, ProtoTorso, ProtoEscudo;
Model ProtoIzqBrazo, ProtoDerBrazo;
Model ProtoIzqPierna, ProtoDerPierna;

//Ave Castigadora
Model AveCuerpo, AveLW, AveRW;

//Angela
Model Angela, AngelaBrazo, AngelaAntebrazo;

//Eddie
Model Eddie, EddieTapa, EddieAspas;

//Animaciones anim;
Animaciones anim;


//letreros
Texture LetrasRingT;
Texture LetreroEntradaT;
//coordenadas letreros
//		P	 R		O	Y		E	C		T	O      ESP	C		G	 E		I	  H		C	ESP	
float posicionesLetrasX[] = { 0.6f, 0.0f, 0.4f, 0.2f, 0.8f, 0.4f, 0.4f, 0.4f, 0.6f, 0.4f, 0.2f, 0.8f, 0.6f, 0.4f, 0.4f, 0.6f };
float posicionesLetrasY[] = { 0.4f, 0.2f, 0.4f, 0.0f, 0.8f, 0.8f, 0.2f, 0.4f, 0.0f, 0.8f, 0.6f, 0.8f, 0.6f, 0.6f, 0.8f, 0.0f };
int cuentaCambioLetra = 0; //contador para cambio
//letras
int velCambioLetra = 60; //cada cuando se cambia
int pos1 = 0, pos2 = 1, pos3 = 2, pos4 = 3, pos5 = 4; //en que numero vamos
//Para el letrero------------------------------------------------------
float toffsetLetrero = 0.0f;
float velocidadLetrero = 0.01f;


//Prueba caminata
float anguloMovimiento = 0.0f;  // controla el ciclo de movimiento (sinusoidal)
float velocidadPaso = 0.1f;     // velocidad del ciclo de paso

int currentCameraMode = 1; // Rastreador de modo de cámara

bool estadoActualAngela = false; // Estado actual de la animación de Angela
bool animvuelo = true;
bool animEddie = true;
bool animPhantump = true;
bool animExcavadora = true;

// Posición y rotación guardadas de Roland
glm::vec3 rolandAvatarPos = glm::vec3(0.0f, 1.5f, 2.0f); // Posición inicial (ajusta si es necesario)
float rolandAvatarYaw = M_PI; // Rotación inicial (mirando a -Z)
bool caminarRoland = false;

Skybox skybox, skyboxNoche;

//materiales
Material Material_brillante;
Material Material_opaco;
Material RolandMaterial;



//-----------ILUMINACION----------------
//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
//SpotLight spotLights[MAX_SPOT_LIGHTS];
SpotLight linterna;                  // Linterna (se activa sola)
PointLight pointLights_Escenario1[4];
PointLight pointLights_Escenario2[4];
PointLight pointLights_Escenario3[4];

PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights1[1];// (Z) 4 Lámparas Galería
SpotLight spotLights2[1]; 
SpotLight spotLights3[1];// (C) 4 Antorchas Ring

int spotLightMode = 0;

// Centros de zonas de activación de luces
glm::vec3 centroGaleriaInicio = glm::vec3(25.0f, -2.0f, 0.0f);
glm::vec3 centroGaleriaFondo = glm::vec3(85.0f, -2.0f, 0.0f);
glm::vec3 centroRing = glm::vec3(130.0f, -2.0f, 15.0f);

// Radio de activacion de luces
float radioActivacion = 25.0f;

//Variables miniaudio

ma_engine engine;
ma_sound sonidoFondo;
ma_sound ambientalRing;
ma_sound ambientalgaleria;
ma_sound efectoProtoman;
ma_sound efectoAngela;
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

	unsigned int arcoIndices[] = {
		0, 1, 2,
		0, 2, 3,
	};

	GLfloat arcoVertices[] = {
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,		0.2f, 0.0f,		0.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,		0.2f, 0.2f,		0.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,		0.0f, 0.2f,		0.0f, -1.0f, 0.0f,
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

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(arcoVertices, arcoIndices, 32, 6);
	meshList.push_back(obj5); // letrero ring

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	//calcAverageNormals(arcoIndices, 6, arcoVertices, 32, 8, 5);

}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


void CargarModelos() {
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();

	Piso_M = Model();
	Piso_M.LoadModel("Models/pisoProyecto.obj");

	//arboles
	Arboles1 = Model();
	Arboles1.LoadModel("Models/ArbolesParque1.obj");

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

	Tomahawk = Model();
	Tomahawk.LoadModel("Models/TomahawkMan.obj");
	MewFossil = Model();
	MewFossil.LoadModel("Models/mewFossil.obj");
	BongBong = Model();
	BongBong.LoadModel("Models/bongbong.obj");

	Lienzo1 = Model();
	Lienzo1.LoadModel("Models/Lienzo1.obj");
	Lienzo2 = Model();
	Lienzo2.LoadModel("Models/Lienzo2.obj");
	Lienzo3 = Model();
	Lienzo3.LoadModel("Models/Lienzo3.obj");
	Lienzo4 = Model();
	Lienzo4.LoadModel("Models/Lienzo4.obj");
	Lienzo5 = Model();
	Lienzo5.LoadModel("Models/Lienzo5.obj");
	Lienzo6 = Model();
	Lienzo6.LoadModel("Models/Lienzo6.obj");

	//arcos
	PuertaDerE = Model();
	PuertaDerE.LoadModel("Models/puertaDerEntrada.obj");
	PuertaIzqE = Model();
	PuertaIzqE.LoadModel("Models/puertaIzqEntrada.obj");
	PilaresE = Model();
	PilaresE.LoadModel("Models/pilaresEntrada.obj");
	LetreroM = Model();
	LetreroM.LoadModel("Models/Letrero.obj");

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


	// Phantump
	PhantumpCabeza = Model();
	PhantumpCabeza.LoadModel("Models/PhantumpCabeza.obj");
	PhantumpCuerpo = Model();
	PhantumpCuerpo.LoadModel("Models/PhantumpCuerpo.obj");
	PhantumpBrazos = Model();
	PhantumpBrazos.LoadModel("Models/PhantumpBrazos.obj");
	PhantumpCola = Model();
	PhantumpCola.LoadModel("Models/PhantumpCola.obj");

	//Incineroar
	InciCabeza = Model();
	InciCabeza.LoadModel("Models/InciCabeza.obj");
	InciTorso = Model();
	InciTorso.LoadModel("Models/InciTorso.obj");
	InciCola = Model();
	InciCola.LoadModel("Models/InciCola.obj");
	InciBrazoD = Model();
	InciBrazoD.LoadModel("Models/InciRBrazo.obj");
	InciAnteD = Model();
	InciAnteD.LoadModel("Models/InciRAnte.obj");
	InciBrazoL = Model();
	InciBrazoL.LoadModel("Models/InciLBrazo.obj");
	InciAnteL = Model();
	InciAnteL.LoadModel("Models/InciLAnte.obj");
	InciPiernaD = Model();
	InciPiernaD.LoadModel("Models/InciRPierna.obj");
	InciPiernaL = Model();
	InciPiernaL.LoadModel("Models/InciLPierna.obj");

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

	AveCuerpo = Model();
	AveCuerpo.LoadModel("Models/AveCuerpo.obj");
	AveRW = Model();
	AveRW.LoadModel("Models/AveRW.obj");
	AveLW = Model();
	AveLW.LoadModel("Models/AveLW.obj");

	//Angela
	Angela = Model();
	Angela.LoadModel("Models/AngelaCuerpo.obj");
	AngelaBrazo = Model();
	AngelaBrazo.LoadModel("Models/AngelaBrazo.obj");
	AngelaAntebrazo = Model();
	AngelaAntebrazo.LoadModel("Models/AngelaAntebrazo.obj");

	//Eddie
	Eddie = Model();
	Eddie.LoadModel("Models/cuerpoEddie.obj");
	EddieTapa = Model();
	EddieTapa.LoadModel("Models/tapaEddie.obj");
	EddieAspas = Model();
	EddieAspas.LoadModel("Models/aspaEddie.obj");

	//letreros
	LetrasRingT = Texture("Textures/megamanFont.tga");
	LetrasRingT.LoadTextureA();
	LetreroEntradaT = Texture("Textures/Letrero.png");
	LetreroEntradaT.LoadTextureA();

	//animaciones keyframes
	BaseDig = Model();
	BaseDig.LoadModel("Models/BaseDig.obj");
	CabinaDig = Model();
	CabinaDig.LoadModel("Models/CabinaDig.obj");
	BrazoDig = Model();
	BrazoDig.LoadModel("Models/BrazoDig.obj");
	AntebrazoDig = Model();
	AntebrazoDig.LoadModel("Models/AnteBrazoDig.obj");
	GarraDig = Model();
	GarraDig.LoadModel("Models/GarraDig.obj");

	TroncoPoke = Model();
	TroncoPoke.LoadModel("Models/TroncoPoke.obj");
	BaseCopaPoke = Model();
	BaseCopaPoke.LoadModel("Models/BaseCopaPoke.obj");
	BayaPoke = Model();
	BayaPoke.LoadModel("Models/BayaPoke.obj");
	CopaPoke = Model();
	CopaPoke.LoadModel("Models/CopaPoke.obj");

	MaquinaExpendedora = Model();
	MaquinaExpendedora.LoadModel("Models/expendedora.obj");
	BotonExpend = Model();
	BotonExpend.LoadModel("Models/botonExpend.obj");
	BandejaExpend = Model();
	BandejaExpend.LoadModel("Models/bandejaExpend.obj");
	LataRefresco = Model();
	LataRefresco.LoadModel("Models/lataWellcheers.obj");

	refrescoCherry = Texture("Textures/LataCherryT.png");
	refrescoCherry.LoadTextureA();
	refrescoRegular = Texture("Textures/LataRegularT.png");
	refrescoRegular.LoadTextureA();
	refrescoGrape = Texture("Textures/LataGrapeT.png");
	refrescoGrape.LoadTextureA();


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

	RolandMaterial = Material(0.5f, 32);
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	anim = Animaciones();

	CreateObjects();
	CreateShaders();
	CargarModelos();

	camera = Camera(glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales

	float attenConst = 0.3f;
	float attenLin = 0.02f;
	float attenQuad = 0.005f;
	
	//linterna
	linterna = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);

	//spotLights_Escenario1[0] = SpotLight(1.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0, 0, 0, 0.0f, -1.0f, 0.0f, attenConst, attenLin, attenQuad, 45.0f);
	pointLights_Escenario1[0] = PointLight(1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 0.0f, 0.0f, 0.0f, attenConst, attenLin, attenQuad);
	pointLights_Escenario1[1] = PointLight(1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 0.0f, 0.0f, 0.0f, attenConst, attenLin, attenQuad);
	pointLights_Escenario1[2] = PointLight(1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 0.0f, 0.0f, 0.0f, attenConst, attenLin, attenQuad);
	pointLights_Escenario1[3] = PointLight(1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 0.0f, 0.0f, 0.0f, attenConst, attenLin, attenQuad);

	pointLights_Escenario2[0] = PointLight(1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 0.0f, 0.0f, 0.0f, attenConst, attenLin, attenQuad);
	pointLights_Escenario2[1] = PointLight(1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 0.0f, 0.0f, 0.0f, attenConst, attenLin, attenQuad);
	pointLights_Escenario2[2] = PointLight(1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 0.0f, 0.0f, 0.0f, attenConst, attenLin, attenQuad);
	pointLights_Escenario2[3] = PointLight(1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 0.0f, 0.0f, 0.0f, attenConst, attenLin, attenQuad);

	pointLights_Escenario3[0] = PointLight(1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 0.0f, 0.0f, 0.0f, attenConst, attenLin, attenQuad);
	pointLights_Escenario3[1] = PointLight(1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 0.0f, 0.0f, 0.0f, attenConst, attenLin, attenQuad);
	pointLights_Escenario3[2] = PointLight(1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 0.0f, 0.0f, 0.0f, attenConst, attenLin, attenQuad);
	pointLights_Escenario3[3] = PointLight(1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 0.0f, 0.0f, 0.0f, attenConst, attenLin, attenQuad);

	
	//spotLightCount++;

	spotLights1[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.2f, 2.0f,
		0.0f, 0.0f, 0.0f,    // Posición se actualiza en el while
		0.0f, -1.0f, 0.0f,   // Dirección: Abajo
		0.3f, 0.02f, 0.005f, // Atenuación
		65.0f);              // Ángulo solicitado

	// Ofrenda 2
	spotLights2[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.2f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.3f, 0.02f, 0.005f,
		65.0f);

	// MegaHawlucha
	spotLights3[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.2f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.3f, 0.02f, 0.005f,
		65.0f);

	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	//Encender luces nocturnas
	bool lucesNocturnasEncendidas = false;

	//Se cargan keyframes
	anim.loadKeyframesExc();
	anim.loadKeyframesPoke();
	anim.loadKeyframesCola();

	printf("Controles:\n");
	//modos de camara
	printf("\nCAMARA:\nWASD - Mover camara\n");
	printf("1 - Modo Principal (Tercera Persona)\n2 - Modo Aereo\n");
	printf("3 - Vista al Ring\n4 - Vista a la Ofrenda\n3 - Vista a la Galeria\n");
	//animaciones
	printf("\nANIMACIONES:\nO - Entrada principal\n");
	printf("I - Entrada al ring\nP - Protoman\nU - Incineroar\n");
	printf("L - Saludo de Angela\nK - Pokebayas\nJ - Maquina expendedora\n");
	//luces
	printf("\nILUMINACION:\nSolo de noche se prenden las luces\n");
	/*
	printf("Z para luces entrada\n");
	printf("X para luces altar\n");
	printf("C para luces antorchas\n");
	*/
	//luces tipo spotlight
	printf("V para luz ofrenda\n");
	printf("B para luz megaHawlucha\n");
	printf("M para luz de la piramide\n");
	//audio
	printf("\AUDIO:\n");
	printf("Q para pausar\n");
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelPiso(1.0);
	glm::mat4 elementos(1.0);
	glm::mat4 elementoLocal(1.0);
	glm::mat4 baseRol(1.0);
	glm::mat4 baseInc(1.0);
	glm::mat4 modelBrazo(1.0);
	glm::mat4 basePhant(1.0);
	glm::mat4 baseProto(1.0);
	glm::mat4 brazoProto(1.0);
	glm::mat4 baseAve(1.0);
	glm::mat4 baseAngela(1.0);
	glm::mat4 brazoAngela(1.0);
	glm::mat4 antebrazoAngela(1.0);
	glm::mat4 baseEddie(1.0);
	glm::mat4 tapaEddie(1.0);
	

	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lowerLight(1.0f);

	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
	glm::vec2 letreroOffset = glm::vec2(0.0f, 0.0f);

	//Función miniaudio
	// 
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
	

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//---------- Camaras -----------------
		// adición de control de teclado
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


		//*******************ANIMACIONES*********************************
		//Captura de barra espaciadora para animacion Roland

		// --- Control de inicio/parada de caminado ---
		if (currentCameraMode == 1) {
			if (mainWindow.getsKeys()[GLFW_KEY_W] || mainWindow.getsKeys()[GLFW_KEY_S] || mainWindow.getsKeys()[GLFW_KEY_A] || mainWindow.getsKeys()[GLFW_KEY_D])
				caminarRoland = true;
			else caminarRoland = false;
		}

		anim.CaminataRoland(caminarRoland, deltaTime);

		anim.EntradaPrincipal(mainWindow.getEntradaAbierta(), deltaTime);
		anim.EntradaRing(mainWindow.getEstadoRing(), deltaTime);

		anim.AnimacionProtoman(mainWindow.getEstadoProto(), deltaTime);
		estadoActualAngela = anim.AnimacionAngela(mainWindow.getEstadoAngela(), deltaTime);
		mainWindow.setEstadoAngela(estadoActualAngela);
		anim.AnimacionIncineroar(mainWindow.getEstadoIncineroar(), deltaTime);

		anim.AnimacionPhantump(animPhantump, deltaTime);
		anim.AnimacionPajaro(animvuelo, toRadians, deltaTime);
		anim.AnimacionEddie(animEddie, toRadians, deltaTime);

		anim.playAnimacionExcavadora(animExcavadora);
		anim.playAnimacionPokearbol(mainWindow.getEstadoPokeArbol());
		
		anim.playAnimacionRefresco(mainWindow.getEstadoRefrescos());
		if (anim.playedCola && mainWindow.getEstadoRefrescos()) {
			mainWindow.setEstadoRefrescos(false);
		}
		

		// Clear the window
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		if (lucesNocturnasEncendidas)
			skyboxNoche.DrawSkybox(camera.calculateViewMatrix(), projection);
		else
			skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		shaderList[0].SetDirectionalLight(&mainLight);
		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		
		SpotLight lucesSpotParaShader[2];
		unsigned int spotCount = 0;
	
		if (lucesNocturnasEncendidas)
		{
			int mode = mainWindow.getSpotLightMode();

			lowerLight = camera.getCameraPosition();
			lowerLight.y -= 0.3f;
			linterna.SetFlash(lowerLight, camera.getCameraDirection());

			lucesSpotParaShader[0] = linterna; 
			spotCount++; 

			switch (mode)
			{
			case 1: // Tecla B
				lucesSpotParaShader[1] = spotLights1[0];
				spotCount++; // Ahora hay 2 luces
				break;
			case 2: // Tecla N
				lucesSpotParaShader[1] = spotLights2[0];
				spotCount++; // Ahora hay 2 luces
				break;
			case 3: // Tecla M
				lucesSpotParaShader[1] = spotLights3[0];
				spotCount++; // Ahora hay 2 luces
				break;
			default:
				break;
			}
		}
		shaderList[0].SetSpotLights(lucesSpotParaShader, spotCount);

		/* ANTERIOR LÓGICA DE LUCES PUNTUALES
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
			shaderList[0].SetPointLights(NULL, 0);
		*/

		if (lucesNocturnasEncendidas)
		{
			glm::vec3 camPos = camera.getCameraPosition();

			
			float distRing = glm::distance(camPos, centroRing);
			float distFondo = glm::distance(camPos, centroGaleriaFondo);
			float distInicio = glm::distance(camPos, centroGaleriaInicio);

			// --- COMPROBACION DE EXCLUSIVIDAD ---
			bool lucesAsignadas = false; // Bandera de control

			// Prioridad 1: Ring
			if (!lucesAsignadas && distRing < radioActivacion)
			{
				shaderList[0].SetPointLights(pointLights_Escenario3, 4);
				lucesAsignadas = true; 
			}

			// Prioridad 2: Fondo Galería
			if (!lucesAsignadas && distFondo < radioActivacion)
			{
				shaderList[0].SetPointLights(pointLights_Escenario2, 4);
				lucesAsignadas = true; 
			}

			// Prioridad 3: Inicio Galería
			if (!lucesAsignadas && distInicio < radioActivacion)
			{
				shaderList[0].SetPointLights(pointLights_Escenario1, 4);
				lucesAsignadas = true; 
			}

			
			if (!lucesAsignadas)
			{
				shaderList[0].SetPointLights(NULL, 0);
			}
		}
		else // Es de día
		{
			shaderList[0].SetPointLights(NULL, 0);
		}


		// --- FIN DE LÓGICA DE LUCES ---

		// --- AUDIO SOUNDTRACK Y AMBIENTAL

		// Calculo de distancias para audio ambiental.

		glm::vec3 posRing = glm::vec3(158.5f, 0.0f, 39.0f); //Heredamos la posicion del ring

		glm::vec3 posGaleria = glm::vec3(70.0f, -2.0f, 0.0f); // Heredamos posicion de la galeria

		glm::vec3 posJugador = camera.getCameraPosition();

		// Distancia al ring
		float distRing = glm::distance(posJugador, posRing);

		// Distancia a ofrenda
		float distGaleria = glm::distance(posJugador, posGaleria);

		// Escala de distancia
		float maxDist = 50.0f;

		// Volumen local normalizado
		float volRing = 1.0f - glm::clamp(distRing / maxDist, 0.0f, 1.0f);
		float volGaleria = 1.0f - glm::clamp(distGaleria/ maxDist, 0.0f, 1.0f);

		// Aplicar volumen a sonidos espaciales
		ma_sound_set_volume(&ambientalRing, volRing);
		ma_sound_set_volume(&ambientalgaleria, volGaleria);

		// Reducir soundtrack al acercarse a cualquiera
		float reduccion = (volRing > volGaleria ? volRing : volGaleria);
		float volSoundtrack = 0.9f - 0.9f * reduccion;
		ma_sound_set_volume(&sonidoFondo, volSoundtrack);

		// --------- FIN DE FUNCIONES AUDIO --------------

		//Piso modelado con Blender
		modelPiso = glm::mat4(1.0);
		modelPiso = glm::translate(modelPiso, glm::vec3(40.0f, -2.5f, 0.0f));
		//color que afecta a todos los elementos
		color = glm::vec3(0.6f, 0.6f, 0.6f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPiso));
		Piso_M.RenderModel();

		//arboles
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.0f, -2.0f, 35.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arboles1.RenderModel();

		//Recepcion
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(43.0f, -2.0f, -15.0f));
		elementos = glm::scale(elementos, glm::vec3(1.3f, 1.3f, 1.3f));
		elementos = glm::rotate(elementos, 30 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementoLocal = elementos;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Angela_Desk.RenderModel();

		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(0.0f, 0.0f, -2.0f));
		elementoLocal = elementos;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Angela_Chair.RenderModel();

		// = ANGELA =
		//Posicionar torso
		baseAngela = elementoLocal;
		baseAngela = glm::translate(baseAngela, glm::vec3(0.0f, 1.6f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(baseAngela));
		Angela.RenderModel();

		//Brazo
		brazoAngela = baseAngela;
		brazoAngela = glm::translate(brazoAngela, glm::vec3(0.4f, 1.3f, 0.0f));
		brazoAngela = glm::rotate(brazoAngela, anim.rotBrazoAng * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(brazoAngela));
		AngelaBrazo.RenderModel();

		//Antebrazo
		antebrazoAngela = brazoAngela;
		antebrazoAngela = glm::translate(antebrazoAngela, glm::vec3(0.28f, -0.65f, 0.07f));
		antebrazoAngela = glm::rotate(antebrazoAngela, anim.rotAnteBraAng * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(antebrazoAngela));
		AngelaAntebrazo.RenderModel();


		//Ofrenda
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(113.0f, -2.0f, -17.0f));
		spotLights1[0].SetFlash(glm::vec3(elementos[3]) + glm::vec3(-1.5f, 0.0f, -1.5f),glm::vec3(-0.5f,0.866f,0.3f));
		elementos = glm::rotate(elementos, -75 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementos = glm::scale(elementos, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Ofrenda.RenderModel();

		//Modelo Phantump posicionado alrededor de la ofrenda
		//Cuerpo, torso como base
		basePhant = elementos;
		//basePhant = glm::mat4(1.0);
		basePhant = glm::translate(basePhant, glm::vec3(anim.pos_ini_x_pha + anim.elevacion_cuerpo, 2.0f + anim.elevacion_cuerpo, anim.pos_ini_z_pha));
		basePhant = glm::rotate(basePhant, anim.orientaPhantump * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		basePhant = glm::scale(basePhant, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(basePhant));
		PhantumpCuerpo.RenderModel();

		//Brazos
		modelaux = basePhant;
		modelaux = glm::translate(modelaux, glm::vec3(0.0f, 1.0f, 2.0f));
		modelaux = glm::rotate(modelaux, anim.rotacion_brazos, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		PhantumpBrazos.RenderModel();

		//Cabeza
		modelaux = basePhant;
		modelaux = glm::translate(modelaux, glm::vec3(0.0f, 23.3f, 2.0f));
		modelaux = glm::rotate(modelaux, anim.ladeo_cabeza * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		PhantumpCabeza.RenderModel();

		//Cola
		modelaux = basePhant;
		modelaux = glm::translate(modelaux, glm::vec3(-2.5f, -1.5f, -10.0f));
		modelaux = glm::rotate(modelaux, anim.rotacion_cola, glm::vec3(1.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		PhantumpCola.RenderModel();

		//Ubicacion central de la piramide
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(158.5f, -2.0f, 39.0f));
		spotLights3[0].SetFlash(glm::vec3(elementos[3]) + glm::vec3(0.0f, 4.5f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		
		elementos = glm::rotate(elementos, -125 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementoLocal = elementos;
		elementos = glm::translate(elementos, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		PiramideRing.RenderModel();

		//Ring de lucha
		elementos = elementoLocal;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Ring.RenderModel();


		// INCINEROAR
		// 1. Empezar desde la matriz del ring (que ya está girada y en -2.0f Y)
		baseInc = elementoLocal;
		baseInc = glm::translate(baseInc, glm::vec3(anim.posInciX - 5.9f, anim.posInciY + 8.2f, anim.posInciZ -6.1f));
		baseInc = glm::rotate(baseInc, anim.orientaInci * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		baseInc = glm::scale(baseInc, glm::vec3(0.025f, 0.025f, 0.025f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(baseInc));
		InciTorso.RenderModel();

		//Cabeza
		modelaux = baseInc;
		modelaux = glm::rotate(modelaux, anim.rotCabezaInciX * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = glm::rotate(modelaux, anim.rotCabezaInciY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = glm::translate(modelaux, glm::vec3(0.0f, 7.5f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		InciCabeza.RenderModel();

		//BD
		modelBrazo = baseInc;
		modelBrazo = glm::translate(modelBrazo, glm::vec3(-35.0f, -2.0f, -10.0f));
		modelBrazo = glm::rotate(modelBrazo, anim.rotBrazoInciX * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelBrazo = glm::rotate(modelBrazo, anim.rotBrazoInciZ * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//modelBrazo = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBrazo));
		InciBrazoD.RenderModel();

		modelaux = modelBrazo;
		modelaux = glm::translate(modelaux, glm::vec3(-23.0f, -46.0f, 0.0f));
		modelaux = glm::rotate(modelaux, anim.rotAntebInciX * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = glm::rotate(modelaux, anim.rotAntebInciZ * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		InciAnteD.RenderModel();

		//BI
		modelBrazo = baseInc;
		modelBrazo = glm::translate(modelBrazo, glm::vec3(35.0f, -2.0f, -10.0f));
		modelBrazo = glm::rotate(modelBrazo, anim.rotBrazoInciX * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelBrazo = glm::rotate(modelBrazo, -anim.rotBrazoInciZ * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//modelBrazo = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBrazo));
		InciBrazoL.RenderModel();

		modelaux = modelBrazo;
		modelaux = glm::translate(modelaux, glm::vec3(23.0f, -46.0f, 0.0f));
		modelaux = glm::rotate(modelaux, anim.rotAntebInciX * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = glm::rotate(modelaux, -anim.rotAntebInciZ * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		InciAnteL.RenderModel();

		//pD
		modelaux = baseInc;
		modelaux = glm::translate(modelaux, glm::vec3(-5.0f, -62.0f, 0.0f));
		modelaux = glm::rotate(modelaux, anim.rotPiernaInciX * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = glm::rotate(modelaux, anim.rotPiernaInciZ * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		InciPiernaD.RenderModel();

		//pI
		modelaux = baseInc;
		modelaux = glm::translate(modelaux, glm::vec3(5.0f, -62.0f, 0.0f));
		modelaux = glm::rotate(modelaux, anim.rotPiernaInciX * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = glm::rotate(modelaux, -anim.rotPiernaInciZ * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		InciPiernaL.RenderModel();

		//COLA
		modelaux = baseInc;
		modelaux = glm::translate(modelaux, glm::vec3(0.0f, -60.0f, -20.0f));
		modelaux = glm::rotate(modelaux, anim.rotColaInci * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		InciCola.RenderModel();

		//MegaHawlucha
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(70.0f, -2.0f, 0.0f));
		spotLights2[0].SetFlash(glm::vec3(elementos[3]) + glm::vec3(1.0f,-2.0f, 0.0f), glm::vec3(-0.5f,0.866f,0.0f));
		elementoLocal = elementos;
		elementos = glm::rotate(elementos, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		MegaHawlucha.RenderModel();

		//Calendario
		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(20.0f, 0.0f, -1.0f));
		elementos = glm::rotate(elementos, -75 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		CalendarioDoom.RenderModel();

		//Base penacho
		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(-20.0f, 0.0f, 16.0f));
		elementos = glm::rotate(elementos, 150 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementoLocal = elementos;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		ExpositorGaleria.RenderModel();

		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(0.0f, 3.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		PenachoMan.RenderModel();

		//Paredes exposicion
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(90.0f, -2.0f, 17.0f));
		elementos = glm::rotate(elementos, 160 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementoLocal = elementos;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		ParedGaleria.RenderModel();

		//armas megaman
		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(0.0f, 4.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Tomahawk.RenderModel();

		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(100.0f, -2.0f, 17.0f));
		elementos = glm::rotate(elementos, 200 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementoLocal = elementos;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		ParedGaleria.RenderModel();

		//mew
		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(0.0f, 5.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		MewFossil.RenderModel();

		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(80.0f, -2.0f, 17.0f));
		elementos = glm::rotate(elementos, 200 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementoLocal = elementos;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		ParedGaleria.RenderModel();

		//bong bong
		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(0.0f, 5.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		BongBong.RenderModel();


		//FILA DE caballetes.
		//1
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(54.0f, -2.0f, -21.0f));
		elementos = glm::scale(elementos, glm::vec3(1.0f, 1.3f, 1.0f));
		elementos = glm::rotate(elementos, 15 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementoLocal = elementos;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Caballete1.RenderModel();

		//lienzo 1
		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(0.0f, 1.5f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Lienzo1.RenderModel();

		//2
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(61.0f, -2.0f, -22.0f));
		elementos = glm::scale(elementos, glm::vec3(1.3f, 1.3f, 1.3f));
		elementos = glm::rotate(elementos, -20 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementoLocal = elementos;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Caballete2.RenderModel();

		//lienzo 2
		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(0.0f, 1.3f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Lienzo2.RenderModel();

		//3
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(65.0f, -2.0f, -23.0f));
		elementos = glm::rotate(elementos, 10 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementoLocal = elementos;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Caballete2.RenderModel();

		//lienzo 3
		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(0.0f, 1.3f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Lienzo3.RenderModel();

		//4
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(69.0f, -2.0f, -23.0f));
		elementos = glm::scale(elementos, glm::vec3(0.9f, 0.9f, 0.9f));
		elementos = glm::rotate(elementos, 15 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementoLocal = elementos;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Caballete1.RenderModel();

		//lienzo 4
		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(0.0f, 1.5f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Lienzo4.RenderModel();

		//5
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(74.0f, -2.0f, -23.0f));
		elementos = glm::scale(elementos, glm::vec3(1.5f, 1.5f, 1.5f));
		elementos = glm::rotate(elementos, 10 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementoLocal = elementos;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Caballete1.RenderModel();

		//lienzo 5
		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(0.0f, 1.5f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Lienzo5.RenderModel();


		//6
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(79.0f, -2.0f, -23.0f));
		elementos = glm::scale(elementos, glm::vec3(1.0f, 1.3f, 1.5f));
		elementos = glm::rotate(elementos, -10 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementoLocal = elementos;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Caballete2.RenderModel();

		//lienzo 6
		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(0.0f, 1.3f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Lienzo6.RenderModel();


		//------------------------LAMPARAS------------------------------------------------
		//entrada al ring
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(127.0f, -2.0f, 13.0f));
		pointLights_Escenario3[0].SetPos(glm::vec3(elementos[3]) + glm::vec3(0.0f, 5.0f, 0.0f));
		elementos = glm::rotate(elementos, -30 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementoLocal = elementos;
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		fuegoLampara.RenderModel();

		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(0.0f, 0.0f, 11.0f));
		pointLights_Escenario3[1].SetPos(glm::vec3(elementos[3]) + glm::vec3(0.0f, 5.0f, 0.0f));		
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		fuegoLampara.RenderModel();


		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(136.0f, -2.0f, 17.0f));
		pointLights_Escenario3[2].SetPos(glm::vec3(elementos[3]) + glm::vec3(0.0f, 5.0f, 0.0f));		
		elementos = glm::rotate(elementos, -30 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementoLocal = elementos;
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		fuegoLampara.RenderModel();

		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(0.0f, 0.0f, 11.0f));
		pointLights_Escenario3[3].SetPos(glm::vec3(elementos[3]) + glm::vec3(0.0f, 5.0f, 0.0f));		
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		fuegoLampara.RenderModel();

		//lamparas de galería
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(36.0f, -2.0f, -5.5f));	
		pointLights_Escenario1[0].SetPos(glm::vec3(elementos[3]) + glm::vec3(0.0f, 5.0f, 0.0f));
		pointLights_Escenario2[0].SetPos(glm::vec3(elementos[3]) + glm::vec3(0.0f, 5.0f, 0.0f));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		capoLampara.RenderModel();

		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(36.0f, -2.0f, 7.0f));
		pointLights_Escenario1[1].SetPos(glm::vec3(elementos[3]) + glm::vec3(0.0f, 5.0f, 0.0f));		
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		capoLampara.RenderModel();

		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(60.0f, -2.0f, 17.0f));
		pointLights_Escenario1[2].SetPos(glm::vec3(elementos[3]) + glm::vec3(0.0f, 5.0f, 0.0f));		
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		capoLampara.RenderModel();

		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(50.0f, -2.0f, -23.0f));
		pointLights_Escenario1[3].SetPos(glm::vec3(elementos[3]) + glm::vec3(0.0f, 5.0f, 0.0f));
		pointLights_Escenario2[1].SetPos(glm::vec3(elementos[3]) + glm::vec3(0.0f, 5.0f, 0.0f));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		capoLampara.RenderModel();

		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(90.0f, -2.0f, 22.0f));
		pointLights_Escenario2[3].SetPos(glm::vec3(elementos[3]) + glm::vec3(0.0f, 5.0f, 0.0f));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		capoLampara.RenderModel();

		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(80.0f, -2.0f, -26.0f));
		pointLights_Escenario2[2].SetPos(glm::vec3(elementos[3]) + glm::vec3(0.0f, 5.0f, 0.0f));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		capoLampara.RenderModel();


		//--------------------------------- PUERTAS ----------------------------
		//Pilares entrada
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(33.0f, -2.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PilaresE.RenderModel();

		//Puerta izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.0f, 0.0f, -0.8f));
		model = glm::translate(model, glm::vec3(anim.TpuertaE, 0.0f, anim.TpuertaE_Z));;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PuertaIzqE.RenderModel();

		//Puerta derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.0f, 0.0f, -0.8f));
		model = glm::translate(model, glm::vec3(-anim.TpuertaE, 0.0f, anim.TpuertaE_Z));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PuertaDerE.RenderModel();

		//letrero Entrada
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 7.5f, 1.0f));
		model = glm::scale(model, glm::vec3(1.7f, 2.5f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		// Vector para el offset del letrero con desplazamiento
		toffsetLetrero += velocidadLetrero * deltaTime;
		if (toffsetLetrero > 1.0f)	toffsetLetrero = 0.0f;

		letreroOffset = glm::vec2(toffsetLetrero, 0.0f);
		// Envio del offset al shader
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(letreroOffset));
		LetreroEntradaT.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		LetreroM.RenderModel();

		//para evitar generar offset en otras texturas
		toffset = glm::vec2(0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

		//ARCO RING
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(116.0f, -2.0f, 14.0f));
		elementos = glm::scale(elementos, glm::vec3(0.9f, 0.9f, 0.9f));
		elementos = glm::rotate(elementos, 60 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = elementos;
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		ArcoRing.RenderModel();

		//puerta rotatoria der
		elementos = modelaux;
		elementos = glm::translate(elementos, glm::vec3(6.5f, 0.0f, 2.2f));
		elementos = glm::scale(elementos, glm::vec3(1.15f, 1.3f, 1.0f));
		elementos = glm::rotate(elementos, anim.Rpuerta * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		PuertaDerR.RenderModel();

		//puerta rotatoria izq
		elementos = modelaux;
		elementos = glm::translate(elementos, glm::vec3(-6.0f, 0.0f, 2.2f));
		elementos = glm::scale(elementos, glm::vec3(1.15f, 1.3f, 1.0f));
		elementos = glm::rotate(elementos, -anim.Rpuerta * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		PuertaIzqR.RenderModel();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//cambio de letra
		if (cuentaCambioLetra >= velCambioLetra) {
			pos1++;
			if (pos1 > 15) pos1 = 0;

			pos2++;
			if (pos2 > 15) 	pos2 = 0;

			pos3++;
			if (pos3 > 15) pos3 = 0;

			pos4++;
			if (pos4 > 15) 	pos4 = 0;

			pos5++;
			if (pos5 > 15) 	pos5 = 0;

			cuentaCambioLetra = 0;
		}
		cuentaCambioLetra++;

		//letrero del arco, letra 1
		model = modelaux;
		toffset = glm::vec2(posicionesLetrasX[pos1], posicionesLetrasY[pos1]);
		model = glm::translate(model, glm::vec3(3.5f, 13.0f, -3.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		LetrasRingT.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		//letrero del arco, letra 2
		toffset = glm::vec2(posicionesLetrasX[pos2], posicionesLetrasY[pos2]);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		LetrasRingT.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		//letrero del arco, letra 3
		toffset = glm::vec2(posicionesLetrasX[pos3], posicionesLetrasY[pos3]);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		LetrasRingT.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		//letrero del arco, letra 4
		toffset = glm::vec2(posicionesLetrasX[pos4], posicionesLetrasY[pos4]);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		LetrasRingT.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		//letrero del arco, letra 5
		toffset = glm::vec2(posicionesLetrasX[pos5], posicionesLetrasY[pos5]);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		LetrasRingT.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		toffset = glm::vec2(0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
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
		baseRol = glm::mat4(1.0f);
		baseRol = glm::translate(baseRol, rolandAvatarPos); // Usar la posición guardada
		baseRol = glm::rotate(baseRol, rolandAvatarYaw, glm::vec3(0.0f, 1.0f, 0.0f)); // Usar la rotación guardada
		baseRol = glm::scale(baseRol, glm::vec3(3.0f, 3.0f, 3.0f)); // Aplicar escala

		// 7. Dibujar el Torso (baseRol)
		RolandMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(baseRol));
		RolandTorso.RenderModel();

		//Brazo Derecho
		modelaux = baseRol;
		modelaux = glm::translate(modelaux, glm::vec3(0.18f, -0.065f, 0.035f));
		modelaux = glm::rotate(modelaux, glm::radians(anim.rotacionBrazoDer), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		RolandBrazoDer.RenderModel();

		//Brazo Izquierdo
		modelaux = baseRol;
		modelaux = glm::translate(modelaux, glm::vec3(-0.18f, -0.067f, 0.017f));
		modelaux = glm::rotate(modelaux, glm::radians(anim.rotacionBrazoIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		RolandBrazoIzq.RenderModel();

		//Pierna Derecha
		modelaux = baseRol;
		modelaux = glm::translate(modelaux, glm::vec3(0.071f, -0.58f, -0.02f));
		modelaux = glm::rotate(modelaux, glm::radians(anim.rotacionPiernaDer), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		RolandPiernaDer.RenderModel();

		//Pierna Izquierda
		modelaux = baseRol;
		modelaux = glm::translate(modelaux, glm::vec3(-0.071f, -0.58f, -0.02f));
		modelaux = glm::rotate(modelaux, glm::radians(anim.rotacionPiernaIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		RolandPiernaIzq.RenderModel();

		//------------------------------------------
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		// = PROTOMAN =
		//Posicionar torso
		baseProto = glm::mat4(1.0f);
		baseProto = glm::translate(baseProto, glm::vec3(90.0f - anim.posicionProto, 1.0f, 39.0f));
		baseProto = glm::rotate(baseProto, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		baseProto = glm::scale(baseProto, glm::vec3(1.5f, 1.5f, 1.5f));
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
		modelaux = glm::rotate(modelaux, anim.escudoProto1 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = glm::rotate(modelaux, anim.escudoProto2 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = glm::rotate(modelaux, anim.escudoProto1 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		ProtoEscudo.RenderModel();

		//Brazo Izquierdo
		modelaux = baseProto;
		modelaux = glm::translate(modelaux, glm::vec3(0.5f, 0.4f, -0.16f));
		modelaux = glm::rotate(modelaux, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = glm::rotate(modelaux, anim.brazoProtoR * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		ProtoIzqBrazo.RenderModel();

		//Pierna Derecha
		modelaux = baseProto;
		modelaux = glm::translate(modelaux, glm::vec3(-0.17f, -0.53f, -0.13f));
		modelaux = glm::rotate(modelaux, anim.piernaProto * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		ProtoDerPierna.RenderModel();

		//Pierna Izquierda
		modelaux = baseProto;
		modelaux = glm::translate(modelaux, glm::vec3(0.25f, -0.53f, -0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		ProtoIzqPierna.RenderModel();

		//AVE CASTIGADORA
		baseAve = glm::mat4(1.0f);
		baseAve = glm::translate(baseAve, glm::vec3(50.0f + anim.pos_ini_x_ave, 8.5f + anim.desplazamiento_vuelo, 45.0f + anim.pos_ini_z_ave));
		baseAve = glm::rotate(baseAve, anim.orienta_ave * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		baseAve = glm::scale(baseAve, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(baseAve));
		AveCuerpo.RenderModel();

		//LW
		modelaux = baseAve;//0.083786f,-0.03725f
		modelaux = glm::translate(modelaux, glm::vec3(0.06f, -0.04f, 0.0f));
		modelaux = glm::rotate(modelaux, -anim.posAla, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		AveLW.RenderModel();

		//RW
		modelaux = baseAve;
		modelaux = glm::translate(modelaux, glm::vec3(-0.06f, -0.04f, 0.0f));
		modelaux = glm::rotate(modelaux, anim.posAla, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		AveRW.RenderModel();

		//EDDIE
		baseEddie = glm::mat4(1.0f);
		baseEddie = glm::translate(baseEddie, glm::vec3(150.0f, -2.0f + anim.posEddieY, -25.0f + anim.posEddieX));
		//baseEddie = glm::rotate(baseEddie, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		baseEddie = glm::rotate(baseEddie, anim.anguloEddieY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		baseAve = glm::scale(baseAve, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(baseEddie));
		Eddie.RenderModel();

		//tapa
		tapaEddie = baseEddie;
		tapaEddie = glm::translate(tapaEddie, glm::vec3(0.0f, 1.5f, -0.7f));
		tapaEddie = glm::rotate(tapaEddie, anim.anguloEddieTapa * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(tapaEddie));
		EddieTapa.RenderModel();

		//aspas
		modelaux = tapaEddie;
		modelaux = glm::translate(modelaux, glm::vec3(0.0f, 0.2f, 0.7f));
		modelaux = glm::rotate(modelaux, anim.rotacionAspas, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		EddieAspas.RenderModel();

		//---------- KEYFRAMES: EXCAVADORA --------------
		//BASE DIG, nodo padre
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(anim.movBase_x + 160.0f, -2.0f, anim.movBase_z + (-35.0f)));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BaseDig.RenderModel();

		//CABINA DIG
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
		model = glm::rotate(model, anim.giroCabina * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CabinaDig.RenderModel();

		//BRAZO DIG
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 2.5f));
		model = glm::rotate(model, anim.giroBrazo * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BrazoDig.RenderModel();

		//ANTEBRAZO DIG
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.5f));
		model = glm::rotate(model, anim.giroAnteB * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		AntebrazoDig.RenderModel();

		//GARRA DIG
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 4.3f));
		model = glm::rotate(model, anim.giroGarra * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GarraDig.RenderModel();

		////---------- KEYFRAMES: ARBOL DE POKEBAYAS --------------

		//TRONCO POKE, nodo padre
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(85.0f, -2.0f, -35.0f));
		//model = glm::rotate(model, 10 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, anim.rotTronco * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TroncoPoke.RenderModel();

		//BASE COPA POKE
		model = modelaux;
		//model = glm::translate(model, glm::vec3(-1.0f, 3.5f, 0.0f));
		model = glm::translate(model, glm::vec3(anim.movBase * 2, 3.5f, anim.movBase));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BaseCopaPoke.RenderModel();

		//COPA POKE
		model = modelaux;
		//model = glm::translate(model, glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(anim.movCopa * 2, 1.0f, anim.movCopa));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CopaPoke.RenderModel();

		//BAYA POKE
		model = modelaux;
		//model = glm::translate(model, glm::vec3(0.7f, 0.5f, 2.5f));
		model = glm::translate(model, glm::vec3(0.7f, 0.5f + anim.caidaBaya, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BayaPoke.RenderModel();

		////---------- KEYFRAMES: MAQUINA EXPENDEDORA --------------

		//MAQUINA EXPENDEDORA, nodo padre
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(70.0f, -2.0f, 16.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, anim.rotMaquina * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		baseAve = glm::scale(baseAve, glm::vec3(3.0f, 3.0f, 3.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MaquinaExpendedora.RenderModel();

		//BOTON
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.0f, 2.9f, anim.movBoton + 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BotonExpend.RenderModel();

		//Bandeja
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.4f, 1.5f, anim.movBandeja + -0.1f));
		//model = glm::translate(model, glm::vec3(movCopa * 2, 1.0f, movCopa));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BandejaExpend.RenderModel();

		//Lata Refresco
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, anim.posRefrescoY + 1.3f, anim.posRefrescoZ + 0.0f));
		//model = glm::translate(model, glm::vec3(0.7f, 0.5f + caidaBaya, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		switch (anim.tipoRefresco) {
		case 0:
			refrescoCherry.UseTexture();
			break;
		case 1:
			refrescoRegular.UseTexture();
			break;
		case 2:
			refrescoGrape.UseTexture();
			break;
		}
		LataRefresco.RenderModel();

		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
