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
const float PI = 3.14159265f;
// Variable de control: Duración del ciclo completo en segundos (300s = 5 minutos)
GLfloat cycleDuration = 60.0f;
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

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
Model Piramide_M;

Model Piso_M;

Model Angela_Desk;
Model Angela_Chair;
Texture Angela_Desk_Texture;
Texture Angela_Chair_Texture;

Model Ofrenda;
Texture FloresT;
Texture OneSinT;
Texture OfrendaT;

Model PiramideRing;
Model Ring;
Texture PiramideT;
Texture RingT;

//GALERIA
//centro
Model CalendarioDoom;
Model PenachoMan;
Model MegaHawlucha;
Texture CalendarioT;
Texture PenachoT;
Texture MegaHawluchaT;

//alrededores
Model ParedGaleria;
Model ExpositorGaleria;
Model Caballete1;
Model Caballete2;
Texture ParedGalT;
Texture ExpositorT;
Texture CaballeteT;

//modelos y texturas auxiliares
Model birdlamp;
Texture birdlampTexture;


Skybox skybox;

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

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

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
	PiramideTexture = Texture("Textures/PiramideOfrenda.png");
	PiramideTexture.LoadTextureA();


	Piramide_M = Model();
	Piramide_M.LoadModel("Models/Piramide.obj");

	Ring = Model();
	Ring.LoadModel("Models/ring.obj");
	RingT = Texture("Textures/RingBake.png");
	RingT.LoadTextureA();


	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

	Piso_M = Model();
	Piso_M.LoadModel("Models/Piso.obj");

	//recepcion
	Angela_Desk = Model();
	Angela_Desk.LoadModel("Models/escritorio.obj");
	Angela_Desk_Texture = Texture("Textures/Textura_Desk.png");
	Angela_Desk_Texture.LoadTextureA();
	Angela_Chair = Model();
	Angela_Chair.LoadModel("Models/sillaAngela.obj");
	Angela_Chair_Texture = Texture("Textures/Silla_Texture.png");
	Angela_Chair_Texture.LoadTextureA();

	//ofrenda
	Ofrenda = Model();
	Ofrenda.LoadModel("Models/ofrenda.obj");
	FloresT = Texture("Textures/FloresCempasuchilTextura.png");
	FloresT.LoadTextureA();
	OfrendaT = Texture("Textures/BakeOfrenda.png");
	OfrendaT.LoadTextureA();
	OneSinT = Texture("Textures/OneSinTextura.png");
	OneSinT.LoadTextureA();

	//Mega Hawlucha
	MegaHawlucha = Model();
	MegaHawlucha.LoadModel("Models/MegaHawlucha.obj");
	MegaHawluchaT = Texture("Textures/Estatua.png");
	MegaHawluchaT.LoadTextureA();
	//calendario doom
	CalendarioDoom = Model();
	CalendarioDoom.LoadModel("Models/DoomsdayCalendar.obj");
	CalendarioT = Texture("Textures/DoomsdayCalT.png");
	CalendarioT.LoadTextureA();
	//penacho
	PenachoMan = Model();
	PenachoMan.LoadModel("Models/PenachoMan.obj");
	PenachoT = Texture("Textures/PenachoT.png");
	PenachoT.LoadTextureA();

	//alrededores
	ParedGaleria = Model();
	ParedGaleria.LoadModel("Models/BaseGal1.obj");
	ParedGalT = Texture("Textures/BaseGaleria1T.png");
	ParedGalT.LoadTextureA();

	ExpositorGaleria = Model();
	ExpositorGaleria.LoadModel("Models/BaseGal2.obj");
	ExpositorT = Texture("Textures/BaseGaleria2T.png");
	ExpositorT.LoadTextureA();

	Caballete1 = Model();
	Caballete1.LoadModel("Models/CaballeteGal1.obj");
	Caballete2 = Model();
	Caballete2.LoadModel("Models/CaballeteGal2.obj");
	CaballeteT = Texture("Textures/caballete1T.png");
	CaballeteT.LoadTextureA();

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,
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
	////Loop mientras no se cierra la ventana

	//Encender luces nocturnas
	bool lucesNocturnasEncendidas = false;

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::mat4 modelPiso(1.0);
	glm::mat4 Elementos(1.0);
	glm::mat4 elementos(1.0);
	glm::mat4 elementoLocal(1.0);
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

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
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

		// --- INICIO DE MODIFICACIÓN: CICLO DÍA/NOCHE ---
		// Calcular el factor de iluminación (0.0 noche, 1.0 día)
		GLfloat angle = (now / cycleDuration) * 2.0f * PI;
		GLfloat lightFactor = (cos(angle) + 1.0f) * 0.5f; // Mapea -1..1 a 0..1

		// Interpolar las intensidades
		GLfloat currentAmbient = minAmbient + (maxAmbient - minAmbient) * lightFactor;
		GLfloat currentDiffuse = minDiffuse + (maxDiffuse - minDiffuse) * lightFactor;

		// Re-asignar la mainLight con las nuevas intensidades
		// Se mantienen el color (1,1,1) y la dirección (0,0,-1)
		mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
			currentAmbient, currentDiffuse,
			0.0f, 0.0f, -1.0f);
		// --- FIN DE MODIFICACIÓN ---
		if (lightFactor < 0.3f)
		{
			lucesNocturnasEncendidas = true;
			
		}
		else // Si es de día (lightFactor >= 0.3)
		{
			lucesNocturnasEncendidas = false;
			
		}
		

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
		modelPiso = glm::translate(modelPiso, glm::vec3(0.0f, -2.0f, 0.0f));
		Elementos = modelPiso;
		//model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPiso));
		Piso_M.RenderModel();
		
		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(), 0.5f, -3.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();
	
		//Helicóptero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		//Cubos de posicion
		//Recepcion
		Elementos = glm::mat4(1.0);
		Elementos = glm::translate(Elementos, glm::vec3(-55.0f, -1.0f, 30.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(Elementos));
		meshList[0]->RenderMesh();

		//El papu de rosa
		Elementos = glm::mat4(1.0);
		Elementos = glm::translate(Elementos, glm::vec3(-90.0f, -1.0f, 60.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(Elementos));
		meshList[0]->RenderMesh();

		//Poke estatua (?) Gris
		Elementos = glm::mat4(1.0);
		Elementos = glm::translate(Elementos, glm::vec3(-90.0f, -1.0f, 0.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(Elementos));
		meshList[0]->RenderMesh();

		//Ofrenda
		Elementos = glm::mat4(1.0);
		Elementos = glm::translate(Elementos, glm::vec3(-20.0f, -1.0f, -40.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(Elementos));
		meshList[0]->RenderMesh();
		//Skullman
		Elementos = glm::mat4(1.0);
		Elementos = glm::translate(Elementos, glm::vec3(-40.0f, -1.0f, -40.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(Elementos));
		meshList[0]->RenderMesh();

		//Protoman
		Elementos = glm::mat4(1.0);
		Elementos = glm::translate(Elementos, glm::vec3(10.0f, -1.0f, 70.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(Elementos));
		meshList[0]->RenderMesh();

		//Entrada al ring
		Elementos = glm::mat4(1.0);
		Elementos = glm::translate(Elementos, glm::vec3(100.0f, -1.0f, 50.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(Elementos));
		meshList[0]->RenderMesh();

		//Mercenaria
		Elementos = glm::mat4(1.0);
		Elementos = glm::translate(Elementos, glm::vec3(95.0f, -1.0f, 47.5f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(Elementos));
		meshList[0]->RenderMesh();

		//Ring de lucha/Ubicacion central de la piramide
		Elementos = glm::mat4(1.0);
		Elementos = glm::translate(Elementos, glm::vec3(172.5f, -1.0f, -25.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(Elementos));
		meshList[0]->RenderMesh();

		//Poke arbol
		Elementos = glm::mat4(1.0);
		Elementos = glm::translate(Elementos, glm::vec3(80.0f, -1.0f, -55.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(Elementos));
		meshList[0]->RenderMesh();

		//Ubicaciones centrales de la galeria
		//------------------------------------------ 
		// Limite a la izquierda
		Elementos = glm::mat4(1.0);
		Elementos = glm::translate(Elementos, glm::vec3(-40.0f, -1.0f, 30.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(Elementos));
		meshList[0]->RenderMesh();

		//Centro de la parte central
		Elementos = glm::mat4(1.0);
		Elementos = glm::translate(Elementos, glm::vec3(40.0f, -1.0f, 20.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(Elementos));
		meshList[0]->RenderMesh();

		//Limite a la derecha
		Elementos = glm::mat4(1.0);
		Elementos = glm::translate(Elementos, glm::vec3(110.0f, -1.0f, 20.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(Elementos));
		meshList[0]->RenderMesh();

		//Limite abajo
		Elementos = glm::mat4(1.0);
		Elementos = glm::translate(Elementos, glm::vec3(40.0f, -1.0f, 55.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(Elementos));
		meshList[0]->RenderMesh();
		//Limite arriba
		Elementos = glm::mat4(1.0);
		Elementos = glm::translate(Elementos, glm::vec3(40.0f, -1.0f, -5.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(Elementos));
		meshList[0]->RenderMesh();


		//Recepcion
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(43.0f, -2.0f, -15.0f));
		elementos = glm::scale(elementos, glm::vec3(1.3f, 1.3f, 1.3f));
		elementos = glm::rotate(elementos, 30 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementoLocal = elementos;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Angela_Desk_Texture.UseTexture();
		Angela_Desk.RenderModel();

		elementos = glm::translate(elementos, glm::vec3(0.0f, 0.0f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		Angela_Chair_Texture.UseTexture();
		Angela_Chair.RenderModel();

		//Ofrenda
		elementos = glm::mat4(1.0);
		//elementos = glm::translate(elementos, glm::vec3(100.0f, -2.0f, -60.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		elementos = glm::translate(elementos, glm::vec3(113.0f, -2.0f, -17.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		elementos = glm::rotate(elementos, -75 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		elementos = glm::scale(elementos, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		FloresT.UseTexture();
		OneSinT.UseTexture();
		OfrendaT.UseTexture();
		Ofrenda.RenderModel();

		//Ubicacion central de la piramide
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(175.5f, -2.0f, 50.0f));
		elementos = glm::rotate(elementos, -125 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//elementos = glm::translate(elementos, glm::vec3(125.5f, -2.0f, 25.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		//elementos = glm::rotate(elementos, -125 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		PiramideTexture.UseTexture();
		Piramide_M.RenderModel();

		//Ring de lucha
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		RingT.UseTexture();
		Ring.RenderModel();

		//MegaHawlucha
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(70.0f, -2.0f, 0.0f));
		elementoLocal = elementos;
		elementos = glm::rotate(elementos, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		MegaHawluchaT.UseTexture();
		MegaHawlucha.RenderModel();

		//Calendario
		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(20.0f, 0.0f, -1.0f));
		elementos = glm::rotate(elementos, -75 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		CalendarioT.UseTexture();
		CalendarioDoom.RenderModel();

		//Penacho (con base)
		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(-15.0f, 0.0f, 16.0f));
		elementos = glm::rotate(elementos, 150 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		PenachoT.UseTexture();
		PenachoMan.RenderModel();

		//Paredes exposicion
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(90.0f, -2.0f, 17.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		elementoLocal = elementos;
		elementos = glm::rotate(elementos, 160 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		ParedGalT.UseTexture();
		ParedGaleria.RenderModel();

		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(10.0f, 0.0f, 0.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		elementos = glm::rotate(elementos, 200 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		ParedGalT.UseTexture();
		ParedGaleria.RenderModel();

		elementos = elementoLocal;
		elementos = glm::translate(elementos, glm::vec3(-10.0f, 0.0f, 0.0f));//Siempre se tiene que tener -1 en Y para estar sobre el piso
		elementos = glm::rotate(elementos, 200 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		ParedGalT.UseTexture();
		ParedGaleria.RenderModel();

		//FILA DE caballetes.
		//1
		elementos = glm::mat4(1.0);
		elementos = glm::translate(elementos, glm::vec3(54.0f, -2.0f, -24.0f));
		modelaux = elementos;
		elementos = glm::scale(elementos, glm::vec3(1.0f, 1.3f, 1.0f));
		elementos = glm::rotate(elementos, 15 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		CaballeteT.UseTexture();
		//textura de cuadro en cuestion
		Caballete1.RenderModel();

		//2
		elementos = modelaux;
		elementos = glm::translate(elementos, glm::vec3(5.0f, 0.0f, 0.0f));
		modelaux = elementos;
		elementos = glm::scale(elementos, glm::vec3(1.5f, 1.5f, 1.5f));
		elementos = glm::rotate(elementos, -20 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		CaballeteT.UseTexture();
		//textura de cuadro en cuestion
		Caballete2.RenderModel();

		//3
		elementos = modelaux;
		elementos = glm::translate(elementos, glm::vec3(4.0f, 0.0f, 0.0f));
		modelaux = elementos;
		elementos = glm::scale(elementos, glm::vec3(0.8f, 0.8f, 0.8f));
		elementos = glm::rotate(elementos, 10 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		CaballeteT.UseTexture();
		//textura de cuadro en cuestion
		Caballete2.RenderModel();

		//4
		elementos = modelaux;
		elementos = glm::translate(elementos, glm::vec3(4.0f, 0.0f, 0.0f));
		modelaux = elementos;
		elementos = glm::rotate(elementos, 15 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		CaballeteT.UseTexture();
		//textura de cuadro en cuestion
		Caballete1.RenderModel();

		//5
		elementos = modelaux;
		elementos = glm::translate(elementos, glm::vec3(4.0f, 0.0f, 0.0f));
		modelaux = elementos;
		elementos = glm::scale(elementos, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		CaballeteT.UseTexture();
		//textura de cuadro en cuestion
		Caballete1.RenderModel();

		//6
		elementos = modelaux;
		elementos = glm::translate(elementos, glm::vec3(6.0f, 0.0f, 0.0f));
		modelaux = elementos;
		elementos = glm::scale(elementos, glm::vec3(1.0f, 1.3f, 1.5f));
		elementos = glm::rotate(elementos, -10 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		CaballeteT.UseTexture();
		//textura de cuadro en cuestion
		Caballete2.RenderModel();

		//7
		elementos = modelaux;
		elementos = glm::translate(elementos, glm::vec3(4.0f, 0.0f, 0.0f));
		modelaux = elementos;
		elementos = glm::scale(elementos, glm::vec3(0.8f, 0.8f, 0.8f));
		elementos = glm::rotate(elementos, 10 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		CaballeteT.UseTexture();
		//textura de cuadro en cuestion
		Caballete1.RenderModel();

		//8
		elementos = modelaux;
		elementos = glm::translate(elementos, glm::vec3(5.0f, 0.0f, 0.0f));
		modelaux = elementos;
		elementos = glm::rotate(elementos, 15 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		CaballeteT.UseTexture();
		//textura de cuadro en cuestion
		Caballete2.RenderModel();

		//9
		elementos = modelaux;
		elementos = glm::translate(elementos, glm::vec3(5.0f, 0.0f, 0.0f));
		modelaux = elementos;
		elementos = glm::scale(elementos, glm::vec3(1.0f, 1.4f, 1.0f));
		elementos = glm::rotate(elementos, -20 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(elementos));
		CaballeteT.UseTexture();
		//textura de cuadro en cuestion
		Caballete1.RenderModel();

		//------------------------------------------

		//Agave
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
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
