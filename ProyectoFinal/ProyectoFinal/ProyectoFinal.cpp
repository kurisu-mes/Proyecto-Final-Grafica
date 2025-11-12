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

//objetos de exposicion
Model Lienzo1;
Model Lienzo2;
Model Lienzo3;
Model Lienzo4;
Model Lienzo5;
Model Lienzo6;

Model Tomahawk;
Model MewFossil;
Model BongBong;

//modelos y texturas auxiliares
Model capoLampara;
Model fuegoLampara;

Model PuertaDerE;
Model PuertaIzqE;
Model PilaresE;
Model LetreroM;
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
//SpotLight spotLights[MAX_SPOT_LIGHTS];
SpotLight linterna;                  // Linterna (se activa sola)
SpotLight spotLights_Escenario1[4]; // (Z) 4 Lámparas Galería
SpotLight spotLights_Escenario3[4]; // (C) 4 Antorchas Ring

// (X) 2 Altar + 1 Hawlucha
SpotLight spotLights_Escenario2[4];
PointLight pointLights[MAX_POINT_LIGHTS];

//variables para animaciones
//letras
						//		P	 R		O	Y		E	C		T	O      ESP	C		G	 E		I	  H		C	ESP	
float posicionesLetrasX[] = { 0.6f, 0.0f, 0.4f, 0.2f, 0.8f, 0.4f, 0.4f, 0.4f, 0.6f, 0.4f, 0.2f, 0.8f, 0.6f, 0.4f, 0.4f, 0.6f };
float posicionesLetrasY[] = { 0.4f, 0.2f, 0.4f, 0.0f, 0.8f, 0.8f, 0.2f, 0.4f, 0.0f, 0.8f, 0.6f, 0.8f, 0.6f, 0.6f, 0.8f, 0.0f };
int velCambioLetra = 150; //cada cuando se cambia
int cuentaCambioLetra = 0; //contador para cambio
int pos1 = 0, pos2 = 1, pos3 = 2, pos4 = 3, pos5 = 4; //en que numero vamos
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

//letreros
Texture LetrasRingT;
Texture LetreroEntradaT;


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

//Ave Castigadora
Model AveCuerpo;
Model AveLW;
Model AveRW;

//Angela
Model Angela;
Model AngelaBrazo;
Model AngelaAntebrazo;
Model AngelaMano;

//Eddie
Model Eddie;
Model EddieTapa;
Model EddieAspas;


//Prueba caminata
float anguloMovimiento = 0.0f;  // controla el ciclo de movimiento (sinusoidal)
float velocidadPaso = 0.1f;     // velocidad del ciclo de paso

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

//Posicional Ave
float pos_ini_x_ave = 0.0f;
float pos_ini_z_ave = 0.0f;
float orienta_ave = 0.0f;
float desplazamiento_vuelo = 0.5f;
float tiempovuelo = 0.0f;
float velocidadvuelo = 0.05f;
float radio = 5.0f; //Para la circunferencia de vuelo
float desplazamiento_x = 0.1f;
float desplazamiento_z = 0.1f;
float rotacionAla = 0.0f;
float posAla;
bool animvuelo = true;
bool alasube = true;
float velocidadaleteo = 0.003f;

//Animacion Angela
int faseAnimAngela = 0;
float rotBrazoAng = 0.0f;
float rotAnteBraAng = 0.0f;

//Animacion Eddie
bool animEddie = true;
int estadoEddie = 0;
float posEddieY = 0.0f;
float posEddieX = 0.0f;
float anguloEddieTapa = 0.0f;
float anguloEddieY = 0.0f;
float anguloAspas = 0.0f;
float rotacionAspas = 0.0f;

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
	AngelaMano = Model();
	AngelaMano.LoadModel("Models/AngelaMano.obj");

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
	float attenLin = 0.02f;
	float attenQuad = 0.005f;
	
	//linterna
	spotLights_Escenario1[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	//spotLights_Escenario1[0] = SpotLight(1.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0, 0, 0, 0.0f, -1.0f, 0.0f, attenConst, attenLin, attenQuad, 45.0f);
	spotLights_Escenario1[1] = SpotLight(1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 0, 0, 0, 0.0f, -1.0f, 0.0f, attenConst, attenLin, attenQuad, 65.0f);
	spotLights_Escenario1[2] = SpotLight(1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 0, 0, 0, 0.0f, -1.0f, 0.0f, attenConst, attenLin, attenQuad, 65.0f);
	spotLights_Escenario1[3] = SpotLight(1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 0, 0, 0, 0.0f, -1.0f, 0.0f, attenConst, attenLin, attenQuad, 65.0f);

	spotLights_Escenario2[0] = SpotLight(1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 0, 0, 0, 0.0f, -1.0f, 0.0f, attenConst, attenLin, attenQuad, 65.0f);
	spotLights_Escenario2[1] = SpotLight(1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 0, 0, 0, 0.0f, -1.0f, 0.0f, attenConst, attenLin, attenQuad, 89.0f);
	spotLights_Escenario2[2] = SpotLight(1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 0, 0, 0, 0.0f, -1.0f, 0.0f, attenConst, attenLin, attenQuad, 65.0f);
	spotLights_Escenario2[3] = SpotLight(1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 0, 0, 0, 0.0f, -1.0f, 0.0f, attenConst, attenLin, attenQuad, 65.0f);

	spotLights_Escenario3[0] = SpotLight(1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 0, 0, 0, 0.0f, -1.0f, 0.0f, attenConst, attenLin, attenQuad, 65.0f);
	spotLights_Escenario3[1] = SpotLight(1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 0, 0, 0, 0.0f, -1.0f, 0.0f, attenConst, attenLin, attenQuad, 65.0f);
	spotLights_Escenario3[2] = SpotLight(1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 0, 0, 0, 0.0f, -1.0f, 0.0f, attenConst, attenLin, attenQuad, 65.0f);
	spotLights_Escenario3[3] = SpotLight(1.0f, 1.0f, 0.5f, 0.5f, 2.0f, 0, 0, 0, 0.0f, -1.0f, 0.0f, attenConst, attenLin, attenQuad, 65.0f);

	
	//spotLightCount++;

	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		0.2f, 2.0f, 
		0.0f, 0.0f, 0.0f,
		0.3f, 0.02f, 0.005f);
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
		0.2f, 2.0f, 
		0.0f, 0.0f, 0.0f,
		0.3f, 0.02f, 0.005f);
	pointLights[2] = PointLight(1.0f, 1.0f, 1.0f,
		0.2f, 2.0f,  
		0.0f, 0.0f, 0.0f,
		0.3f, 0.02f, 0.005f); 

	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	//Encender luces nocturnas
	bool lucesNocturnasEncendidas = false;

	printf("Controles:\n");
	//modos de camara
	printf("\nCAMARA:\nWASD - Mover camara\n");
	printf("1 - Modo Principal (Tercera Persona)\n2 - Modo Aereo\n");
	printf("3 - Vista al Ring\n4 - Vista a la Ofrenda\n3 - Vista a la Galeria\n");
	//animaciones
	printf("\nANIMACIONES:\nO - Entrada principal\n");
	printf("I - Entrada al ring\nP - Protoman\n");
	printf("L - Angela\n");
	//luces
	printf("\nILUMINACION:\nSolo de noche se prenden las luces\n");
	printf("Z para luces entrada\n");
	printf("X para luces altar\n");
	printf("C para luces antorchas\n");
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelPiso(1.0);
	glm::mat4 elementos(1.0);
	glm::mat4 elementoLocal(1.0);
	glm::mat4 baseRol(1.0);
	glm::mat4 baseInc(1.0);
	glm::mat4 baseProto(1.0);
	glm::mat4 brazoProto(1.0);
	glm::mat4 baseAve(1.0);
	glm::mat4 baseAngela(1.0);
	glm::mat4 brazoAngela(1.0);
	glm::mat4 antebrazoAngela(1.0);
	glm::mat4 baseEddie(1.0);
	glm::mat4 tapaEddie(1.0);

	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
	glm::vec2 letreroOffset = glm::vec2(0.0f, 0.0f);


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

		//Animacion Protoman
		if (mainWindow.getEstadoProto()) {
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

		//ANIMACION VUELO AVE EN CIRCULOS
		if (animvuelo) {
			tiempovuelo += deltaTime * velocidadvuelo; //Captura tiempo, calcula velocidad anim
			pos_ini_x_ave = desplazamiento_x + (radio * cos(tiempovuelo));
			pos_ini_z_ave = desplazamiento_z + (radio * sin(tiempovuelo));
			//Captura de parametrizacion de circulo (x,y) fuera de origen (h,k)
			// x = h + radio * cos (t)
			// y = k + radio * sin (t)
			orienta_ave = glm::degrees(atan2(-sin(tiempovuelo), cos(tiempovuelo)));
			//OBTIENE GRADOS MEDIANTE TAN(T) = CA/CO
			// T = ATAN (-y / x)
			desplazamiento_vuelo = sin(tiempovuelo * 2.5f) * 1.0f;
			//Empieza declaracion de estados alas
			rotacionAla += 30.0f * deltaTime;
			posAla = sin(glm::radians(rotacionAla));
		}

		//ANIMACION SIMPLE ANGELA
		if (mainWindow.getEstadoAngela()) {
			//se vuelve true, comienza saludo
			if (faseAnimAngela == 0) {
				if (rotBrazoAng > -92.0f) {
					rotBrazoAng -= 2.0f * deltaTime;
					if (rotAnteBraAng < 92.0f) rotAnteBraAng += 2.0f * deltaTime;
				}
				else if (rotAnteBraAng > 0.0f) rotAnteBraAng -= 2.0f * deltaTime;
				else faseAnimAngela = 1;
			}

			//termina saludo bajando el brazo, y reinicializa el bool
			if (faseAnimAngela == 1) {
				if (rotAnteBraAng < 92.0f) rotAnteBraAng += 2.0f * deltaTime;
				else faseAnimAngela = 2;
			}
			if (faseAnimAngela == 2) {
				if (rotBrazoAng < 0.0f) {
					rotBrazoAng += 2.0f * deltaTime;
					if (rotAnteBraAng > 0.0f) rotAnteBraAng -= 2.0f * deltaTime;
				}
				else {
					faseAnimAngela = 0;
					mainWindow.setEstadoAngela(false);
				}
			}
		}
		
		/*
		if(mainWindow.getsKeys()[GLFW_KEY_J])
		{
			animEddie = true;
		}
		*/

		if (animEddie) {
			
			//hacer brinco
			if (estadoEddie == 0) {
				posEddieX += 0.05f;
				if (posEddieY < 7.0f) posEddieY += 0.1f;
				else estadoEddie = 1;
			}
			if (estadoEddie == 1) {
				posEddieX += 0.05f;
				if (posEddieY > 0.0f) posEddieY -= 0.1f;
				else estadoEddie = 2;
			}
			//detenerse, abrir tapa, dar vuelta
			if (estadoEddie == 2) {
				if (anguloEddieTapa > -90.0f) anguloEddieTapa -= 1.5f * deltaTime;
				else estadoEddie = 3;
			}
			if (estadoEddie == 3) {
				if (anguloEddieTapa < 0.0f) anguloEddieTapa += 1.5f * deltaTime;
				else if (anguloEddieY < 180.0f) anguloEddieY += 3.5f * deltaTime;
				else estadoEddie = 4;
			}

			//hacer brinco de regreso
			if (estadoEddie == 4) {
				posEddieX -= 0.05f;
				if (posEddieY < 7.0f) posEddieY += 0.1f;
				else estadoEddie = 5;
			}
			if (estadoEddie == 5) {
				posEddieX -= 0.05f;
				if (posEddieY > 0.0f) posEddieY -= 0.1f;
				else estadoEddie = 6;
			}
			//detenerse, abrir tapa, dar vuelta
			if (estadoEddie == 6) {
				if (anguloEddieTapa > -90.0f) anguloEddieTapa -= 1.5f * deltaTime;
				else estadoEddie = 7;
			}
			if (estadoEddie == 7) {
				if (anguloEddieTapa < 0.0f) anguloEddieTapa += 1.5f * deltaTime;
				else if (anguloEddieY > 0.0f) anguloEddieY -= 3.5f * deltaTime;
				else estadoEddie = 0;
			}
		}

		anguloAspas += 30.0f * deltaTime;
		rotacionAspas = cos(glm::radians(anguloAspas));

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
		uniformTextureOffset = shaderList[0].getOffsetLocation();

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
		spotLights_Escenario1[0].SetFlash(lowerLight, camera.getCameraDirection());
		shaderList[0].SetDirectionalLight(&mainLight);
		int lightMode = mainWindow.getLightMode();
		if (lucesNocturnasEncendidas)
		{
			switch (lightMode)
			{
			case 1: // (Tecla Z)
				shaderList[0].SetSpotLights(spotLights_Escenario1, 4);
				break;
			case 2: // (Tecla X)
				shaderList[0].SetSpotLights(spotLights_Escenario2, 4);
				break;
			case 3: // (Tecla C)
				shaderList[0].SetSpotLights(spotLights_Escenario3, 4);
				break;
			default: // Apagadas
				shaderList[0].SetSpotLights(NULL, 0);
				break;
			}
		}
		else // Es de día
		{
			shaderList[0].SetSpotLights(NULL, 0);
		}


		PointLight activePointLights[MAX_SPOT_LIGHTS];
		unsigned int activePointLightCount = 0;
		

		// Si es de noche, añadir las 2 luces fijas
		if (lucesNocturnasEncendidas)
		{
			activePointLights[activePointLightCount++] = pointLights[0];
			activePointLights[activePointLightCount++] = pointLights[1];
			activePointLights[activePointLightCount++] = pointLights[2];
		}


		shaderList[0].SetPointLights(activePointLights, activePointLightCount);

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
		brazoAngela = glm::rotate(brazoAngela, rotBrazoAng * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(brazoAngela));
		AngelaBrazo.RenderModel();

		//Antebrazo
		antebrazoAngela = brazoAngela;
		antebrazoAngela = glm::translate(antebrazoAngela, glm::vec3(0.28f, -0.65f, 0.07f));
		antebrazoAngela = glm::rotate(antebrazoAngela, rotAnteBraAng * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(antebrazoAngela));
		AngelaAntebrazo.RenderModel();

		//Mano
		modelaux = antebrazoAngela;
		modelaux = glm::translate(modelaux, glm::vec3(-0.45f, -0.055f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		AngelaMano.RenderModel();

		//Ofrenda
		elementos = glm::mat4(1.0);
		//elementos = glm::translate(elementos, glm::vec3(100.0f, -2.0f, -60.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		elementos = glm::translate(elementos, glm::vec3(113.0f, -2.0f, -17.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		pointLights[0].SetPos(glm::vec3(elementos[3]) + glm::vec3(-0.5f, 2.5f, -0.5f));
		pointLights[1].SetPos(glm::vec3(elementos[3]) + glm::vec3(0.5f, 2.5f, 0.5f));
		elementos = glm::rotate(elementos, -75 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementos = glm::scale(elementos, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Ofrenda.RenderModel();

		//Ubicacion central de la piramide
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(158.5f, -2.0f, 39.0f));
		spotLights_Escenario2[1].SetFlash(glm::vec3(elementos[3]) + glm::vec3(0.0f, 4.5f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		
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
		modelaux = baseInc;
		modelaux = glm::translate(modelaux, glm::vec3(0.0f, 19.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		InciCabeza.RenderModel();

		//BD
		modelaux = baseInc;
		modelaux = glm::translate(modelaux, glm::vec3(-42.9f, -14.0f, 2.0f));
		modelaux = glm::rotate(modelaux, glm::radians(rotacionBrazoDerInci), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		InciBD.RenderModel();

		//BI
		modelaux = baseInc;
		modelaux = glm::translate(modelaux, glm::vec3(42.9f, -14.0f, 2.0f));
		modelaux = glm::rotate(modelaux, glm::radians(rotacionBrazoIzqInci), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		InciBI.RenderModel();

		//pD
		modelaux = baseInc;
		modelaux = glm::translate(modelaux, glm::vec3(-12.0f, -73.0f, -2.0f));
		modelaux = glm::rotate(modelaux, glm::radians(rotacionPiernaDerInci), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		InciPD.RenderModel();

		//pI
		modelaux = baseInc;
		modelaux = glm::translate(modelaux, glm::vec3(12.0f, -73.0f, -2.0f));
		modelaux = glm::rotate(modelaux, glm::radians(rotacionPiernaIzqInci), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));

		InciPI.RenderModel();

		//COLA
		modelaux = baseInc;
		modelaux = glm::translate(modelaux, glm::vec3(0.0f, -93.0f, -78.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		InciCola.RenderModel();

		//MegaHawlucha
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(70.0f, -2.0f, 0.0f));
		pointLights[2].SetPos(glm::vec3(elementos[3]) + glm::vec3(0.0f, 2.0f, 0.0f));
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
		elementos = glm::translate(elementos, glm::vec3(90.0f, -2.0f, 17.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		elementos = glm::rotate(elementos, 160 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementoLocal = elementos;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		ParedGaleria.RenderModel();

		//armas megaman
		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(0.0f, 4.0f, 0.5f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Tomahawk.RenderModel();

		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(100.0f, -2.0f, 17.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		elementos = glm::rotate(elementos, 200 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementoLocal = elementos;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		ParedGaleria.RenderModel();

		//mew
		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(0.0f, 5.0f, 0.5f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		MewFossil.RenderModel();

		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(80.0f, -2.0f, 17.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		elementos = glm::rotate(elementos, 200 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementoLocal = elementos;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		ParedGaleria.RenderModel();

		//bong bong
		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(0.0f, 5.0f, 0.5f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
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
		elementos = glm::translate(elementos, glm::vec3(0.0f, 1.5f, 0.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
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
		elementos = glm::translate(elementos, glm::vec3(0.0f, 1.3f, 0.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
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
		elementos = glm::translate(elementos, glm::vec3(0.0f, 1.3f, 0.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
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
		elementos = glm::translate(elementos, glm::vec3(0.0f, 1.5f, 0.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
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
		elementos = glm::translate(elementos, glm::vec3(0.0f, 1.5f, 0.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
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
		elementos = glm::translate(elementos, glm::vec3(0.0f, 1.3f, 0.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Lienzo6.RenderModel();


		//------------------------LAMPARAS------------------------------------------------
		//entrada al ring
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(127.0f, -2.0f, 13.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		spotLights_Escenario3[0].SetFlash(glm::vec3(elementos[3]) + glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f,-1.0f,0.0f));
		elementos = glm::rotate(elementos, -30 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementoLocal = elementos;
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		fuegoLampara.RenderModel();

		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(0.0f, 0.0f, 11.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		spotLights_Escenario3[1].SetFlash(glm::vec3(elementos[3]) + glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));		
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		fuegoLampara.RenderModel();


		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(136.0f, -2.0f, 17.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		spotLights_Escenario3[2].SetFlash(glm::vec3(elementos[3]) + glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));		
		elementos = glm::rotate(elementos, -30 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementoLocal = elementos;
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		fuegoLampara.RenderModel();

		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(0.0f, 0.0f, 11.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		spotLights_Escenario3[3].SetFlash(glm::vec3(elementos[3]) + glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));		
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		fuegoLampara.RenderModel();

		//lamparas de galería
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(36.0f, -2.0f, -5.5f));//Siempre se tiene que tener -1 en Y para estar sobre el piso		
		spotLights_Escenario2[0].SetFlash(glm::vec3(elementos[3]) + glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		capoLampara.RenderModel();

		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(36.0f, -2.0f, 7.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		spotLights_Escenario1[1].SetFlash(glm::vec3(elementos[3]) + glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));		
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		capoLampara.RenderModel();

		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(60.0f, -2.0f, 17.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		spotLights_Escenario1[2].SetFlash(glm::vec3(elementos[3]) + glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));		
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		capoLampara.RenderModel();

		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(50.0f, -2.0f, -23.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		spotLights_Escenario1[3].SetFlash(glm::vec3(elementos[3]) + glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		capoLampara.RenderModel();

		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(90.0f, -2.0f, 22.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		spotLights_Escenario2[3].SetFlash(glm::vec3(elementos[3]) + glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		capoLampara.RenderModel();

		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(80.0f, -2.0f, -26.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		spotLights_Escenario2[2].SetFlash(glm::vec3(elementos[3]) + glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
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

		//letrero Entrada
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 7.5f, 1.0f));
		model = glm::scale(model, glm::vec3(1.7f, 2.5f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		// Vector para el offset del letrero
		// Para el letrero con desplazamiento
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

		//ARCO
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(116.0f, -2.0f, 14.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
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

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(baseRol));
		RolandTorso.RenderModel();

		// ---- DIBUJAR MIEMBROS (animaciones de caminata) ----

		//Brazo Derecho
		modelaux = baseRol;
		modelaux = glm::translate(modelaux, glm::vec3(0.18f, -0.065f, 0.035f));
		modelaux = glm::rotate(modelaux, glm::radians(rotacionBrazoDer), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		RolandBrazoDer.RenderModel();

		//Brazo Izquierdo
		modelaux = baseRol;
		modelaux = glm::translate(modelaux, glm::vec3(-0.18f, -0.067f, 0.017f));
		modelaux = glm::rotate(modelaux, glm::radians(rotacionBrazoIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		RolandBrazoIzq.RenderModel();

		//Pierna Derecha
		modelaux = baseRol;
		modelaux = glm::translate(modelaux, glm::vec3(0.071f, -0.58f, -0.02f));
		modelaux = glm::rotate(modelaux, glm::radians(rotacionPiernaDer), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		RolandPiernaDer.RenderModel();

		//Pierna Izquierda
		modelaux = baseRol;
		modelaux = glm::translate(modelaux, glm::vec3(-0.071f, -0.58f, -0.02f));
		modelaux = glm::rotate(modelaux, glm::radians(rotacionPiernaIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		RolandPiernaIzq.RenderModel();

		//------------------------------------------

		// = PROTOMAN =
		//Posicionar torso
		baseProto = glm::mat4(1.0f);
		baseProto = glm::translate(baseProto, glm::vec3(65.0f, 0.6f, 16.0f - posicionProto));
		baseProto = glm::rotate(baseProto, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
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

		//AVE CASTIGADORA
		baseAve = glm::mat4(1.0f);
		baseAve = glm::translate(baseAve, glm::vec3(20.0f + pos_ini_x_ave, 8.5f + desplazamiento_vuelo, 20.0f + pos_ini_z_ave));
		baseAve = glm::rotate(baseAve, orienta_ave * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		baseAve = glm::scale(baseAve, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(baseAve));
		AveCuerpo.RenderModel();

		//LW
		modelaux = baseAve;//0.083786f,-0.03725f
		modelaux = glm::translate(modelaux, glm::vec3(0.06f, -0.04f, 0.0f));
		modelaux = glm::rotate(modelaux, -posAla, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		AveLW.RenderModel();

		//RW
		modelaux = baseAve;
		modelaux = glm::translate(modelaux, glm::vec3(-0.06f, -0.04f, 0.0f));
		modelaux = glm::rotate(modelaux, posAla, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		AveRW.RenderModel();

		//EDDIE
		baseEddie = glm::mat4(1.0f);
		baseEddie = glm::translate(baseEddie, glm::vec3(15.0f + posEddieX, -2.0f + posEddieY, -10.0f));
		baseEddie = glm::rotate(baseEddie, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		baseEddie = glm::rotate(baseEddie, anguloEddieY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(baseEddie));
		Eddie.RenderModel();

		//tapa
		tapaEddie = baseEddie;
		tapaEddie = glm::translate(tapaEddie, glm::vec3(0.0f, 1.5f, -0.7f));
		tapaEddie = glm::rotate(tapaEddie, anguloEddieTapa * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(tapaEddie));
		EddieTapa.RenderModel();

		//aspas
		modelaux = tapaEddie;
		modelaux = glm::translate(modelaux, glm::vec3(0.0f, 0.2f, 0.7f));
		modelaux = glm::rotate(modelaux, rotacionAspas, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
		EddieAspas.RenderModel();

		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
