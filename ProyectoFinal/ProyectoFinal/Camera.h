#pragma once

#include <glew.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

#include <glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	// Función para establecer el modo de la cámara (1, 2, o 3)
	void setCameraMode(int mode);

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();
	glm::mat4 calculateViewMatrix();

	~Camera();

private:
	glm::vec3 position;
	glm::vec3 positionAerea;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	int cameraMode; // 1 = Principal, 2 = Aerea, 3 = Estatica

	// Posición y dirección para la cámara estática (Modo 3)
	glm::vec3 staticPosition;
	glm::vec3 staticFront;

	// Posición y dirección para la cámara estática (Modo 4)
	glm::vec3 staticPosition4;
	glm::vec3 staticFront4;

	// Posición y dirección para la cámara estática (Modo 5)
	glm::vec3 staticPosition5;
	glm::vec3 staticFront5;

	GLfloat alturaAerea;
	GLfloat HPrimeraPersona;

	void update();
};

