#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	bool getEntradaAbierta() { return estadoEntrada; }
	bool getEstadoRing() { return estadoRing; }
	bool getEstadoProto() { return estadoProto; }
	bool getEstadoAngela() { return estadoAngela; }
	void setEstadoAngela(bool nuevoEstado) { estadoAngela = nuevoEstado; }
	bool getEstadoIncineroar() { return estadoIncineroar; }
	bool getEstadoPokeArbol() { return estadoPokeArbol; }
	bool getEstadoRefrescos() { return estadoRefrescos; }
	void setEstadoRefrescos(bool nuevoEstado) { estadoRefrescos = nuevoEstado; }
	
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	int getLightMode() { return lightMode; }
	int getSpotLightMode() { return spotLightMode; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	int lightMode;
	int spotLightMode;
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	bool mouseFirstMoved, estadoEntrada, estadoRing, estadoProto, estadoAngela;
	bool estadoIncineroar, estadoPokeArbol, estadoRefrescos;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

