#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>
#include <cmath>
#include <vector>
#include <math.h>

class Animaciones
{
public:
	Animaciones();
	float lerp(float a, float b, float t);
	//animaciones puertas y letreros
	void EntradaPrincipal(bool condicion, GLfloat deltaTime);
	void EntradaRing(bool condicion, GLfloat deltaTime);
	//void LetreroEntrada(GLfloat deltaTime);
	//int LetreroRing(int cuentaCambioLetra);
	//animaciones simples
	void AnimacionProtoman(bool condicion, GLfloat deltaTime);
	bool AnimacionAngela(bool condicion, GLfloat deltaTime);
	//animaciones complejas
	void AnimacionIncineroar(bool condicion, GLfloat deltaTime);
	void AnimacionPajaro(bool condicion, float toRadians, GLfloat deltaTime);
	void AnimacionEddie(bool condicion, float toRadians, GLfloat deltaTime);
	void CaminataRoland(bool condicion, GLfloat deltaTime);
	

	//GLfloat deltaTime;

	//Para la puerta
	float Rpuerta, TpuertaE, TpuertaE_Z;
	//Variables Movimiento Roland
	float anguloMovimiento, velocidadPaso;
	float rotacionBrazoDer, rotacionBrazoIzq;
	float rotacionPiernaDer, rotacionPiernaIzq;

	// Animacion compleja incineroar
	float orienta, tiempolocal;
	float rotacionBrazoDerInci, rotacionBrazoIzqInci;
	float rotacionPiernaDerInci, rotacionPiernaIzqInci;
	//float rotacioncabezaInci;
	float pos_ini_x_inci, pos_ini_z_inci;
	
	//Variables Movimiento Protoman
	float brazoProtoR, escudoProto1, escudoProto2;
	float posicionProto, piernaProto;
	//Animacion Angela
	float rotBrazoAng, rotAnteBraAng;

	//Animacion Eddie
	float posEddieY, posEddieX, rotacionAspas;
	float anguloEddieTapa, anguloEddieY, anguloAspas;

	//Posicional Ave
	float pos_ini_x_ave, pos_ini_z_ave, posAla;
	float desplazamiento_vuelo, orienta_ave;
	
private:
	float AjusteP, velocidadPuerta;
	float TpuertaE_Target_X, RpuertaDer_Target;
	int recorrido;
	float velocidadpiernas = 5.0f;
	float velocidadbrazos = 4.0f;
	int faseAnimAngela;

	//PosInicial Incineroar
	float minXZ, maxXZ;
	float inciSpeed;

	float tiempovuelo, velocidadvuelo, rotacionAla;
	float desplazamiento_x, desplazamiento_z;
	float radio = 5.0f; //Para la circunferencia de vuelo

	int estadoEddie;

};
