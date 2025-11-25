#include "Animaciones.h"
#include "miniaudio.h"

extern ma_sound efectoProtoman;
extern ma_sound efectoAngela;
static bool protoplay = false;
static bool angelaplay = false;

Animaciones::Animaciones()
{
	//Para la puerta
	Rpuerta = 0.0f; // Angulo de rotacion ACTUAL
	TpuertaE = 0.0f; // Traslacion X ACTUAL
	TpuertaE_Z = 0.0f; // Traslacion Z ACTUAL
	AjusteP = -0.5f; // Ajuste para que la puerta no atraviese el pilar

	TpuertaE_Target_X = -3.0f; //Desplazamiento objetivo en X
	RpuertaDer_Target = 90.0f; // Rotacion objetivo de 90 grados
	velocidadPuerta = 0.05f; // Multiplicador para la velocidad de la animacion

	//Variables Movimiento Protoman
	brazoProtoR = -90.0f;
	escudoProto1 = 0.0f;
	escudoProto2 = 0.0f;
	posicionProto = 0.0f;
	piernaProto = 0.0f;

	//Variables saludo Angela
	faseAnimAngela = 0;
	rotBrazoAng = 0.0f;
	rotAnteBraAng = 0.0f;

	//variables vuelo pajaro
	pos_ini_x_ave = 0.0f;
	pos_ini_z_ave = 0.0f;
	orienta_ave = 0.0f;
	desplazamiento_vuelo = 0.5f;
	tiempovuelo = 0.0f;
	velocidadvuelo = 0.05f;
	desplazamiento_x = 0.1f;
	desplazamiento_z = 0.1f;
	rotacionAla = 0.0f;
	posAla = 0.0f;

	//variables Eddie
	estadoEddie = 0;
	posEddieY = 0.0f;
	posEddieX = 0.0f;
	anguloEddieTapa = 0.0f;
	anguloEddieY = 0.0f;
	anguloAspas = 0.0f;
	rotacionAspas = 0.0f;

	//Roland
	anguloMovimiento = 0.0f;  // controla el ciclo de movimiento (sinusoidal)
	velocidadPaso = 0.1f;    // velocidad del ciclo de paso
	rotacionBrazoDer = 0.0f;
	rotacionBrazoIzq = 0.0f;
	rotacionPiernaDer = 0.0f;
	rotacionPiernaIzq = 0.0f;

	// Animacion compleja incineroar
	recorrido = 1;
	orienta = 0.0f; //Orientacion del cuerpo
	tiempolocal = 0.0f; //temporizador de los giros
	rotacionBrazoDerInci = 0.0f;
	rotacionBrazoIzqInci = 0.0f;
	rotacionPiernaDerInci = 0.0f;
	rotacionPiernaIzqInci = 0.0f;
	//rotacioncabezaInci = 0.0f; //Rotaciones miembros del modelo

	//PosInicial Incineroar
	pos_ini_x_inci = -30.0f;
	pos_ini_z_inci = -30.0f;
	velocidadpiernas = 5.0f;
	velocidadbrazos = 4.0f;
	minXZ = -15.0f;
	maxXZ = 15.0f;
	inciSpeed = 0.0f;
}

//Sirve para poder hacer interpolacion lineal y calcular los angulos y posicion en cada frame
float Animaciones::lerp(float a, float b, float t)
{
	return a + t * (b - a);
}


void Animaciones::EntradaPrincipal(bool condicion, GLfloat deltaTime)
{
	//Animacion de la puerta
	if (condicion)
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
}

void Animaciones::EntradaRing(bool condicion, GLfloat deltaTime)
{
	//Puertas Ring
	if (condicion) {
		// --- ABRIR PUERTAS ---
		Rpuerta = lerp(Rpuerta, RpuertaDer_Target, velocidadPuerta * deltaTime);
	}
	else {
		// --- CERRAR PUERTAS ---
		Rpuerta = lerp(Rpuerta, 0.0f, velocidadPuerta * deltaTime);
	}
}


//ANIMACION SIMPLE PROTOMAN
void Animaciones::AnimacionProtoman(bool condicion, GLfloat deltaTime) {
	if (condicion) {
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
}

//ANIMACION SIMPLE ANGELA
bool Animaciones::AnimacionAngela(bool condicion, GLfloat deltaTime) {
	if (condicion) {
		if (!angelaplay) {
			ma_sound_seek_to_pcm_frame(&efectoAngela, 0);   // Reinicia audio desde el inicio
			ma_sound_set_volume(&efectoAngela, 1.5f);
			ma_sound_start(&efectoAngela);                  // Reproduce
			angelaplay = true;                         // Evitar repetir
		}
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
				angelaplay = false;
				return(false);
			}
		}
	}
}

//ANIMACION CAMINATA ROLAND
void Animaciones::CaminataRoland(bool condicion, GLfloat deltaTime) {
	if (condicion) {
		// Actualiza fase de caminata
		anguloMovimiento += velocidadPaso * deltaTime;

		// Movimiento oscilante tipo seno
		rotacionBrazoDer = 35.0f * sin(anguloMovimiento);
		rotacionBrazoIzq = -35.0f * sin(anguloMovimiento);
		rotacionPiernaDer = -35.0f * sin(anguloMovimiento);
		rotacionPiernaIzq = 35.0f * sin(anguloMovimiento);

	}
}

//ANIMACION COMPLEJA INCINEROAR
void Animaciones::AnimacionIncineroar(bool condicion, GLfloat deltaTime) {
	if(condicion) {
		tiempolocal += deltaTime * 0.01;
		// Aumenté la velocidad para que coincida con el nuevo tamaño del recorrido
		inciSpeed = 0.0035f * deltaTime * 60.0f;

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
}

//ANIMACION COMPLEJA PAJARO
void Animaciones::AnimacionPajaro(bool condicion, float toRadians, GLfloat deltaTime) {
	if (condicion) {
		tiempovuelo += deltaTime * velocidadvuelo; //Captura tiempo, calcula velocidad anim
		pos_ini_x_ave = desplazamiento_x + (radio * cos(tiempovuelo));
		pos_ini_z_ave = desplazamiento_z + (radio * sin(tiempovuelo));
		//Captura de parametrizacion de circulo (x,y) fuera de origen (h,k)
		// x = h + radio * cos (t)
		// y = k + radio * sin (t)
		orienta_ave = (atan2(-sin(tiempovuelo), cos(tiempovuelo))) * 57.29;
		//orienta_ave = atan2(-sin(tiempovuelo), cos(tiempovuelo));
		//OBTIENE GRADOS MEDIANTE TAN(T) = CA/CO
		// T = ATAN (-y / x)
		desplazamiento_vuelo = sin(tiempovuelo * 2.5f) * 1.0f;
		//Empieza declaracion de estados alas
		rotacionAla += 30.0f * deltaTime;
		posAla = sin(rotacionAla * toRadians);
	}
}

//ANIMACION COMPLEJA EDDIE
void Animaciones::AnimacionEddie(bool condicion, float toRadians, GLfloat deltaTime) {
	if (condicion) {

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
	rotacionAspas = cos(anguloAspas * toRadians);
}

