#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>
#include <cmath>
#include <vector>
#include <math.h>
//PARA GUARDAR KEYS EN .TXT
#include <fstream>   
#include <sstream>   
#include <iostream>  

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
	//animaciones keyframes excavadora
	void loadKeyframesExc();
	void resetElementsExc();
	void interpolationExc();
	void animateExc(bool playExc);
	void playAnimacionExcavadora(bool playExc);
	//animaciones keyframes arbol
	void loadKeyframesPoke();
	void resetElementsPoke();
	void interpolationPoke();
	void animatePoke(bool playPoke);
	void playAnimacionPokearbol(bool playPoke);
	//animaciones keyframes refresco
	void loadKeyframesCola();
	void resetElementsCola();
	void interpolationCola();
	void animateCola(bool playCola);
	void playAnimacionRefresco(bool playCola);
	

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

	// Keys Excavadora
	float movBase_x, movBase_z, giroCabina;
	float giroBrazo, giroAnteB, giroGarra;

	// Keys Arbol
	float rotTronco, movBase, movCopa, caidaBaya;
	bool playedPoke;

	// Keys Refresco
	float rotMaquina, movBoton, movBandeja;
	float posRefrescoY, posRefrescoZ;
	int tipoRefresco;
	bool refrescoAleatorio;
	bool playedCola;

	
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

	// Keyframes 
	#define MAX_FRAMES 50
	const char* keyFramesExcavadora = "keyframesExcava.txt";
	const char* keyFramesArbol = "keyframesPokeArbol.txt";
	const char* keyFramesRefresco = "keyframesExpendedora.txt";
	// EXCAVADORA
	int i_max_stepsExc = 50; //Número de pasos entre cuadros para interpolación, a mayor número , más lento será el movimiento
	int FrameIndexExc, playIndexExc, i_curr_stepsExc;
	// ARBOL
	int i_max_stepsPoke = 10; //Número de pasos entre cuadros para interpolación, a mayor número , más lento será el movimiento
	int FrameIndexPoke, playIndexPoke, i_curr_stepsPoke;
	// EXPENDEDORA
	int i_max_stepsCola = 20; //Número de pasos entre cuadros para interpolación, a mayor número , más lento será el movimiento
	int FrameIndexCola, playIndexCola, i_curr_stepsCola;

};
