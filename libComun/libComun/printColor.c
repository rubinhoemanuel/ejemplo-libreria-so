/*
 * printColor.c
 *
 *  Created on: 29/7/2017
 *      Author: utnso
 */

#include "printColor.h"

void mostrar(char* mensaje){
	printf(mensaje);
}

void letrasRojas(){
	mostrar(COLOR_ROJO);
}

void letrasVerde(){
	mostrar(COLOR_VERDE);
}

void letrasAmarilla(){
	mostrar(COLOR_AMARILLO);
}

void letrasAzul(){
	mostrar(COLOR_AZUL);
}

void letrasMagenta(){
	mostrar(COLOR_MAGENTA);
}

void letrasCyan(){
	mostrar(COLOR_CYAN);
}

void letrasNormal(){
	mostrar(COLOR_RESET);
}

void inicializar_color(){
	letrasNormal();
}

void mensaje_Error(char * mensaje){
	letrasRojas();
	mostrar("[ERROR] ");
	mostrar(mensaje);
	mostrar("\n");
	letrasNormal();
}

void mensaje_Info(char * mensaje){
	letrasNormal();
	mostrar("[INFO] ");
	mostrar(mensaje);
	mostrar("\n");
	letrasNormal();
}

void mensaje_OK(char * mensaje){
	letrasVerde();
	mostrar("[OK] ");
	mostrar(mensaje);
	mostrar("\n");
	letrasNormal();
}

void mensaje_Color(char* color, char* mensaje){
	mostrar(color);
	mostrar(mensaje);
	mostrar("\n");
	letrasNormal();
}
