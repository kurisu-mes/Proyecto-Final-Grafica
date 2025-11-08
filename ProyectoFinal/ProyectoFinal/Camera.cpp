#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	cameraMode = 1; // Empezar en modo principal por defecto

	// camara estática
	staticPosition = glm::vec3(0.0f, 2.0f, -20.0f);   // Posición 
	staticFront = glm::vec3(1.0f, 0.0f, 0.0f); // Dirección 

	// camara aérea
	alturaAerea = 50.0f;

	HPrimeraPersona = startPosition.y;

	update();
}

void Camera::setCameraMode(int mode)
{
	// Simple validación para asegurarnos de que el modo es 1, 2, o 3
	if (mode >= 1 && mode <= 3)
	{
		if (mode == 1)
		{
			position.y = HPrimeraPersona;
		}
		cameraMode = mode;
		if (mode == 2)
		{
			position.y = alturaAerea;
		}
	}
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	// Modo 1: Cámara principal 
	if (cameraMode == 1)
	{
		
		glm::vec3 frontMovement = glm::vec3(front.x, 0.0f, front.z);
		frontMovement = glm::normalize(frontMovement);

		if (keys[GLFW_KEY_W])
		{
			position += frontMovement * velocity;
		}
		if (keys[GLFW_KEY_S])
		{
			position -= frontMovement * velocity;
		}
		if (keys[GLFW_KEY_A])
		{
			position -= right * velocity;
		}
		if (keys[GLFW_KEY_D])
		{
			position += right * velocity;
		}
	}
	//Camara aérea
	else if (cameraMode == 2)
	{

		if (keys[GLFW_KEY_W])
		{
			position.z -= velocity; 
		}
		if (keys[GLFW_KEY_S])
		{
			position.z += velocity;
		}
		if (keys[GLFW_KEY_A])
		{
			position.x -= velocity;
		}
		if (keys[GLFW_KEY_D])
		{
			position.x += velocity; 
		}
	}
	// Modo 3: Cámara estática 
	else if (cameraMode == 3)
	{
		// *No hace nada*
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{

	if (cameraMode == 1)
	{
		xChange *= turnSpeed;
		yChange *= turnSpeed;

		yaw += xChange;
		pitch += yChange;

		if (pitch > 89.0f)
		{
			pitch = 89.0f;
		}

		if (pitch < -89.0f)
		{
			pitch = -89.0f;
		}

		update();
	}

	update();
}

glm::mat4 Camera::calculateViewMatrix()
{
	//return glm::lookAt(position, position + front, up);

	// Modo 1: Cámara principal
	if (cameraMode == 1)
	{
		// Usa la posición y dirección (front) calculadas por el mouse
		return glm::lookAt(position, position + front, up);
	}
	// Modo 2: Cámara aérea
	else if (cameraMode == 2)
	{
		// Usa la 'position' (movida por WASD)
		// Mira siempre hacia abajo (posición actual + vector -Y)
		// El vector "arriba" de la cámara es 'hacia el fondo' del mundo (eje -Z)
		return glm::lookAt(position,
			position + glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, -1.0f));
	}
	// Modo 3: Cámara estática
	else // (cameraMode == 3)
	{
		// Usa la posición y dirección estáticas que definimos en el constructor
		return glm::lookAt(staticPosition, staticPosition + staticFront, worldUp);
	}
}

glm::vec3 Camera::getCameraPosition()
{

	if (cameraMode == 3)
	{
		return staticPosition;
	}
	return position;
}


glm::vec3 Camera::getCameraDirection()
{

	if (cameraMode == 1)
	{
		return glm::normalize(front);
	}
	else if (cameraMode == 2)
	{
		return glm::vec3(0.0f, -1.0f, 0.0f); // Siempre mira hacia abajo
	}
	else // Modo 3
	{
		return glm::normalize(staticFront);
	}
}

void Camera::update()
{
	/*front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));*/

	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}


Camera::~Camera()
{
}
