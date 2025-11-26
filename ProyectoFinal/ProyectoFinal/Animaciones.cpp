#include "Animaciones.h"
#include "miniaudio.h"

extern ma_sound efectoProtoman;
extern ma_sound efectoAngela;
static bool protoplay = false;
static bool angelaplay = false;

Animaciones::Animaciones() //inicializa variables
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

	// Keys Excavadora
	movBase_x = 0.0f;
	movBase_z = 0.0f;
	giroCabina = 0.0f;
	giroBrazo = -60.0f;
	giroAnteB = 120.0f;
	giroGarra = 60.0f;

	FrameIndexExc = 0;
	playIndexExc = 0; //a partir de cual
	i_curr_stepsExc = 0;

	// Keys Arbol
	rotTronco = 0.0f;
	movBase = 0.0f;
	movCopa = 0.0f;
	caidaBaya = 0.0f;
	
	FrameIndexPoke = 0;
	playIndexPoke = 0; //a partir de cual
	i_curr_stepsPoke = 0;
	playedPoke = false;

	// Keys Refresco
	rotMaquina = 0.0f;
	movBoton = 0.0f;
	movBandeja = 0.0f;
	posRefrescoY = 0.0f;
	posRefrescoZ = 0.0f;
	tipoRefresco = 0;
	refrescoAleatorio = false;

	FrameIndexCola = 0;		
	playIndexCola = 0; //a partir de cual
	i_curr_stepsCola = 0;
	playedCola = false;
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

// Keyframes EXCAVADORA
typedef struct _Excavadora
{
	float movBase_x;
	float movBase_z;
	float giroCabina;
	float giroBrazo;
	float giroAnteB;
	float giroGarra;

	float movBase_xInc;
	float movBase_zInc;
	float giroCabinaInc;
	float giroBrazoInc;
	float giroAnteBInc;
	float giroGarraInc;

}EXCAVAR;
EXCAVAR KeyFrameExc[MAX_FRAMES]; //numero de frames a almacenar

//carga las keyframes del archivo
void Animaciones::loadKeyframesExc() {

	std::ifstream file(keyFramesExcavadora); // se abre el .txt para su lectura
	std::string line; //guarda en un string cada linea

	//Verifica si se puede abrir el archivo antes de intentar leerlo
	if (!file.is_open()) {
		printf("No se encontro el archivo %s\n", keyFramesExcavadora);
		return;
	}

	//mientras haya lineas en el archivo, se lee el documento
	while (std::getline(file, line) && FrameIndexExc < MAX_FRAMES) {
		std::istringstream coord(line);
		float movX, movY, cabina, brazo, anteb, garra;

		// lee las 5 coordenadas
		if (coord >> movX >> movY >> cabina >> brazo >> anteb >> garra) {

			KeyFrameExc[FrameIndexExc].movBase_x = movX;
			KeyFrameExc[FrameIndexExc].movBase_z = movY;
			KeyFrameExc[FrameIndexExc].giroCabina = cabina;
			KeyFrameExc[FrameIndexExc].giroBrazo = brazo;
			KeyFrameExc[FrameIndexExc].giroAnteB = anteb;
			KeyFrameExc[FrameIndexExc].giroGarra = garra;
			FrameIndexExc++;
		}
		else {
			std::cerr << "Error al leer la linea del archivo: " << line << "\n";
		}
	}

	file.close();
}

//reinicia a posicion inicial
void Animaciones::resetElementsExc() //Tecla 0
{
	movBase_x = KeyFrameExc[0].movBase_x;
	movBase_z = KeyFrameExc[0].movBase_z;
	giroCabina = KeyFrameExc[0].giroCabina;
	giroBrazo = KeyFrameExc[0].giroBrazo;
	giroAnteB = KeyFrameExc[0].giroAnteB;
	giroGarra = KeyFrameExc[0].giroGarra;
}

//agrega frames entre keys
void Animaciones::interpolationExc()
{
	KeyFrameExc[playIndexExc].movBase_xInc = (KeyFrameExc[playIndexExc + 1].movBase_x - KeyFrameExc[playIndexExc].movBase_x) / i_max_stepsExc;
	KeyFrameExc[playIndexExc].movBase_zInc = (KeyFrameExc[playIndexExc + 1].movBase_z - KeyFrameExc[playIndexExc].movBase_z) / i_max_stepsExc;
	KeyFrameExc[playIndexExc].giroCabinaInc = (KeyFrameExc[playIndexExc + 1].giroCabina - KeyFrameExc[playIndexExc].giroCabina) / i_max_stepsExc;
	KeyFrameExc[playIndexExc].giroBrazoInc = (KeyFrameExc[playIndexExc + 1].giroBrazo - KeyFrameExc[playIndexExc].giroBrazo) / i_max_stepsExc;
	KeyFrameExc[playIndexExc].giroAnteBInc = (KeyFrameExc[playIndexExc + 1].giroAnteB - KeyFrameExc[playIndexExc].giroAnteB) / i_max_stepsExc;
	KeyFrameExc[playIndexExc].giroGarraInc = (KeyFrameExc[playIndexExc + 1].giroGarra - KeyFrameExc[playIndexExc].giroGarra) / i_max_stepsExc;
}

//ejecuta la animacion
void Animaciones::animateExc(bool playExc) {
	//Movimiento del objeto con barra espaciadora
	if (playExc)
	{
		if (i_curr_stepsExc >= i_max_stepsExc) //fin de animación entre frames?
		{
			playIndexExc++;
			//printf("playindexExc : %d\n", playIndexExc);
			if (playIndexExc > FrameIndexExc - 2)	//Fin de toda la animación con último frame?
			{
				//printf("Frame index= %d\n", FrameIndexExc);
				//printf("termino la animacion\n");
				playIndexExc = 0;
				playExc = false;
			}
			else //Interpolación del próximo cuadro
			{

				i_curr_stepsExc = 0; //Resetea contador
				//Interpolar
				interpolationExc();
			}
		}
		else
		{
			//Dibujar Animación
			movBase_x += KeyFrameExc[playIndexExc].movBase_xInc;
			movBase_z += KeyFrameExc[playIndexExc].movBase_zInc;
			giroCabina += KeyFrameExc[playIndexExc].giroCabinaInc;
			giroBrazo += KeyFrameExc[playIndexExc].giroBrazoInc;
			giroAnteB += KeyFrameExc[playIndexExc].giroAnteBInc;
			giroGarra += KeyFrameExc[playIndexExc].giroGarraInc;
			i_curr_stepsExc++;
		}

	}
}

void Animaciones::playAnimacionExcavadora(bool playExc) {
	if (!playExc) {
		resetElementsExc();
		interpolationExc();
		playIndexExc = 0;
		i_curr_stepsExc = 0;
	}
	else
		animateExc(playExc);
}
// Keyframes ARBOL
typedef struct _Arbol
{
	//Variables para GUARDAR Key Frames
	float rotTronco;
	float movBase;
	float movCopa;
	float caidaBaya;

	float rotTroncoInc;
	float movBaseInc;
	float movCopaInc;
	float caidaBayaInc;

}ARBOL;

ARBOL KeyFramePoke[MAX_FRAMES]; //numero de frames a almacenar

//carga las keyframes del archivo
void Animaciones::loadKeyframesPoke() {

	std::ifstream file(keyFramesArbol); // se abre el .txt para su lectura
	std::string line; //guarda en un string cada linea

	//Verifica si se puede abrir el archivo antes de intentar leerlo
	if (!file.is_open()) {
		printf("No se encontro el archivo %s\n", keyFramesArbol);
		return;
	}

	//mientras haya lineas en el archivo, se lee el documento
	while (std::getline(file, line) && FrameIndexPoke < MAX_FRAMES) {
		std::istringstream coord(line);
		float tronco, base, copa, baya;

		// lee las 5 coordenadas
		if (coord >> tronco >> base >> copa >> baya) {

			KeyFramePoke[FrameIndexPoke].rotTronco = tronco;
			KeyFramePoke[FrameIndexPoke].movBase = base;
			KeyFramePoke[FrameIndexPoke].movCopa = copa;
			KeyFramePoke[FrameIndexPoke].caidaBaya = baya;
			FrameIndexPoke++;
		}
		else {
			std::cerr << "Error al leer la linea del archivo: " << line << "\n";
		}
	}

	file.close();
}

//reinicia a posicion inicial
void Animaciones::resetElementsPoke() //Tecla 0
{
	rotTronco = KeyFramePoke[0].rotTronco;
	movBase = KeyFramePoke[0].movBase;
	movCopa = KeyFramePoke[0].movCopa;
	caidaBaya = KeyFramePoke[0].caidaBaya;
}

//agrega frames entre keys
void Animaciones::interpolationPoke()
{
	KeyFramePoke[playIndexPoke].rotTroncoInc = (KeyFramePoke[playIndexPoke + 1].rotTronco - KeyFramePoke[playIndexPoke].rotTronco) / i_max_stepsPoke;
	KeyFramePoke[playIndexPoke].movBaseInc = (KeyFramePoke[playIndexPoke + 1].movBase - KeyFramePoke[playIndexPoke].movBase) / i_max_stepsPoke;
	KeyFramePoke[playIndexPoke].movCopaInc = (KeyFramePoke[playIndexPoke + 1].movCopa - KeyFramePoke[playIndexPoke].movCopa) / i_max_stepsPoke;
	KeyFramePoke[playIndexPoke].caidaBayaInc = (KeyFramePoke[playIndexPoke + 1].caidaBaya - KeyFramePoke[playIndexPoke].caidaBaya) / i_max_stepsPoke;
}

void Animaciones::animatePoke(bool playPoke) {
	//Movimiento del objeto con barra espaciadora
	if (playPoke)
	{
		if (i_curr_stepsPoke >= i_max_stepsPoke) //fin de animación entre frames?
		{
			playIndexPoke++;
			//printf("playIndexPoke : %d\n", playIndexPoke);
			if (playIndexPoke > FrameIndexPoke - 2)	//Fin de toda la animación con último frame?
			{
				//printf("Frame index= %d\n", FrameIndexPoke);
				//printf("termino la animacion\n");
				playIndexPoke = 0;
				playedPoke = true;
			}
			else //Interpolación del próximo cuadro
			{

				i_curr_stepsPoke = 0; //Resetea contador
				//Interpolar
				interpolationPoke();
			}
		}
		else
		{
			//Dibujar Animación
			rotTronco += KeyFramePoke[playIndexPoke].rotTroncoInc;
			movBase += KeyFramePoke[playIndexPoke].movBaseInc;
			movCopa += KeyFramePoke[playIndexPoke].movCopaInc;
			caidaBaya += KeyFramePoke[playIndexPoke].caidaBayaInc;
			i_curr_stepsPoke++;
		}

	}
}

void Animaciones::playAnimacionPokearbol(bool playPoke) {
	if (!playPoke) {
		resetElementsPoke();
		interpolationPoke();
		playIndexPoke = 0;
		i_curr_stepsPoke = 0;
		playedPoke = false;
	}
	if (playPoke && !playedPoke) {
		animatePoke(playPoke);
	}
		
}

// Keyframes EXPENDEDORA
typedef struct _Expendedora
{
	//Variables para GUARDAR Key Frames
	float rotMaquina;
	float movBoton;
	float movBandeja;
	float posRefrescoY;
	float posRefrescoZ;

	float rotMaquinaInc;
	float movBotonInc;
	float movBandejaInc;
	float posRefrescoYInc;
	float posRefrescoZInc;

}EXPEND;

EXPEND KeyFrameCola[MAX_FRAMES]; //numero de frames a almacenar

//carga las keyframes del archivo
void Animaciones::loadKeyframesCola() {

	std::ifstream file(keyFramesRefresco); // se abre el .txt para su lectura
	std::string line; //guarda en un string cada linea

	//Verifica si se puede abrir el archivo antes de intentar leerlo
	if (!file.is_open()) {
		printf("No se encontro el archivo %s\n", keyFramesRefresco);
		return;
	}

	//mientras haya lineas en el archivo, se lee el documento
	while (std::getline(file, line) && FrameIndexCola < MAX_FRAMES) {
		std::istringstream coord(line);
		float maq, boton, bandeja, refY, refZ;

		// lee las 5 coordenadas
		if (coord >> maq >> boton >> bandeja >> refY >> refZ) {

			KeyFrameCola[FrameIndexCola].rotMaquina = maq;
			KeyFrameCola[FrameIndexCola].movBoton = boton;
			KeyFrameCola[FrameIndexCola].movBandeja = bandeja;
			KeyFrameCola[FrameIndexCola].posRefrescoY = refY;
			KeyFrameCola[FrameIndexCola].posRefrescoZ = refZ;
			FrameIndexCola++;
		}
		else {
			std::cerr << "Error al leer la linea del archivo: " << line << "\n";
		}
	}

	file.close();
}

//reinicia a posicion inicial
void Animaciones::resetElementsCola() //Tecla 0
{
	rotMaquina = KeyFrameCola[0].rotMaquina;
	movBoton = KeyFrameCola[0].movBoton;
	movBandeja = KeyFrameCola[0].movBandeja;
	posRefrescoY = KeyFrameCola[0].posRefrescoY;
	posRefrescoZ = KeyFrameCola[0].posRefrescoZ;
}

//agrega frames entre keys
void Animaciones::interpolationCola()
{
	KeyFrameCola[playIndexCola].rotMaquinaInc = (KeyFrameCola[playIndexCola + 1].rotMaquina - KeyFrameCola[playIndexCola].rotMaquina) / i_max_stepsCola;
	KeyFrameCola[playIndexCola].movBotonInc = (KeyFrameCola[playIndexCola + 1].movBoton - KeyFrameCola[playIndexCola].movBoton) / i_max_stepsCola;
	KeyFrameCola[playIndexCola].movBandejaInc = (KeyFrameCola[playIndexCola + 1].movBandeja - KeyFrameCola[playIndexCola].movBandeja) / i_max_stepsCola;
	KeyFrameCola[playIndexCola].posRefrescoYInc = (KeyFrameCola[playIndexCola + 1].posRefrescoY - KeyFrameCola[playIndexCola].posRefrescoY) / i_max_stepsCola;
	KeyFrameCola[playIndexCola].posRefrescoZInc = (KeyFrameCola[playIndexCola + 1].posRefrescoZ - KeyFrameCola[playIndexCola].posRefrescoZ) / i_max_stepsCola;
}

void Animaciones::animateCola(bool playCola) {
	//Movimiento del objeto con barra espaciadora
	if (playCola)
	{
		if (i_curr_stepsCola >= i_max_stepsCola) //fin de animación entre frames?
		{
			playIndexCola++;
			//printf("playIndexCola : %d\n", playIndexCola);
			if (playIndexCola > FrameIndexCola - 2)	//Fin de toda la animación con último frame?
			{
				//printf("Frame index= %d\n", FrameIndexCola);
				//printf("termino la animacion\n");
				playIndexCola = 0;
				playedCola = true;
			}
			else //Interpolación del próximo cuadro
			{

				i_curr_stepsCola = 0; //Resetea contador
				//Interpolar
				interpolationCola();
			}
		}
		else
		{
			//Dibujar Animación
			rotMaquina += KeyFrameCola[playIndexCola].rotMaquinaInc;
			movBoton += KeyFrameCola[playIndexCola].movBotonInc;
			movBandeja += KeyFrameCola[playIndexCola].movBandejaInc;
			posRefrescoY += KeyFrameCola[playIndexCola].posRefrescoYInc;
			posRefrescoZ += KeyFrameCola[playIndexCola].posRefrescoZInc;
			i_curr_stepsCola++;
		}

	}
}

void Animaciones::playAnimacionRefresco(bool playCola){
	if (!playCola) {
		resetElementsCola();
		interpolationCola();
		playIndexCola = 0;
		i_curr_stepsCola = 0;
		if (!refrescoAleatorio) {
			tipoRefresco = (rand() % 3);
			refrescoAleatorio = true;
		}
		playedCola = false;
	}
	if (playCola && !playedCola) {
		animateCola(playCola);
		refrescoAleatorio = false;
	}
}