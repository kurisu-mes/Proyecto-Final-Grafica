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

	// camara est�tica (Modo 3, Ring)
	staticPosition = glm::vec3(153.5f, 14.0f, 32.0f);   // Posici�n 
	staticFront = glm::vec3(0.9f, -1.5f, 1.1f); // Direcci�n 

	// Posici�n y direcci�n para la c�mara est�tica (Modo 4, Ofrenda)
	staticPosition4 = glm::vec3(90.0f, 12.0f, -12.0f);
	staticFront4 = glm::vec3(0.8f, -0.4f, -0.2f);

	// Posici�n y direcci�n para la c�mara est�tica (Modo 5, Galer�a)
	staticPosition5 = glm::vec3(38.0f, 15.0f, -20.0f);   // Posici�n 
	staticFront5 = glm::vec3(0.8f, -0.2f, 0.6f); // Direcci�n
	
	// camara a�rea
	alturaAerea = 120.0f;
	positionAerea = glm::vec3(startPosition.x, alturaAerea, startPosition.z);
	HPrimeraPersona = startPosition.y;

	update();
}

void Camera::setCameraMode(int mode)
{
	// Simple validaci�n para asegurarnos de que el modo es 1, 2, 3,4 o 5
	if (mode >= 1 && mode <= 5)
	{
		if (mode == 1)
		{
			position.y = HPrimeraPersona;
		}
		cameraMode = mode;
		if (mode == 2)
		{
			positionAerea.y = alturaAerea;
		}
	}
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	// Modo 1: C�mara principal 
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
	//Camara a�rea
	else if (cameraMode == 2)
	{

		if (keys[GLFW_KEY_W])
		{
			positionAerea.z -= velocity; 
		}
		if (keys[GLFW_KEY_S])
		{
			positionAerea.z += velocity;
		}
		if (keys[GLFW_KEY_A])
		{
			positionAerea.x -= velocity;
		}
		if (keys[GLFW_KEY_D])
		{
			positionAerea.x += velocity; 
		}
	}
	// Modo 3: C�mara est�tica 
	else if (cameraMode == 3)
	{
		// *No hace nada*
	}
	else if (cameraMode == 4)
	{
		// *No hace nada*
	}
	else if (cameraMode == 5)
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

	// Modo 1: C�mara principal
	if (cameraMode == 1)
	{
		// Usa la posici�n y direcci�n (front) calculadas por el mouse
		return glm::lookAt(position, position + front, up);
	}
	// Modo 2: C�mara a�rea
	else if (cameraMode == 2)
	{
		
		return glm::lookAt(positionAerea,
			positionAerea + glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, -1.0f));
	}
	// Modo 3, 4 y 5: C�mara est�tica
	else if (cameraMode == 3)
	{
		// Usa la posici�n y direcci�n est�ticas que definimos en el constructor
		return glm::lookAt(staticPosition, staticPosition + staticFront, worldUp);
	}
	else if (cameraMode == 4)
	{
		// Usa la posici�n y direcci�n est�ticas que definimos en el constructor
		return glm::lookAt(staticPosition4, staticPosition4 + staticFront4, worldUp);
	}
	else if (cameraMode == 5)
	{
		// Usa la posici�n y direcci�n est�ticas que definimos en el constructor
		return glm::lookAt(staticPosition5, staticPosition5 + staticFront5, worldUp);
	}
}

glm::vec3 Camera::getCameraPosition()
{
	if (cameraMode == 2)
	{
		return positionAerea;
	}
	else if (cameraMode == 3)
	{
		return staticPosition;
	}
	else if (cameraMode == 4)
	{
		return staticPosition4;
	}
	if (cameraMode == 5)
	{
		return staticPosition5;
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
	else if (cameraMode == 3)// Modo 3
	{
		return glm::normalize(staticFront);
	}
	else if (cameraMode == 4)// Modo 4
	{
		return glm::normalize(staticFront4);
		}
	else if (cameraMode == 5)// Modo 5
	{
		return glm::normalize(staticFront5);
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
