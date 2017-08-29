/*
 * serealizacion.h
 *
 *  Created on: 29/7/2017
 *      Author: utnso
 */

#ifndef LIBCOMUN_SEREALIZACION_H_
#define LIBCOMUN_SEREALIZACION_H_

#include <commons/collections/list.h>
#include "sockets.h"


//////////////////////////////////////////
// PROTOCOLO     						//
//////////////////////////////////////////
typedef enum t_protocolo {
	tEjemplo=1,
	tFinDeProtocolo //NO SACAR Y DEJAR A LO ULTIMO!!!
} t_protocolo;

//////////////////////////////////////////
// TADS									//
//////////////////////////////////////////

typedef struct string {
	int tamanio;
	char * cadena;
} string;

//////////////////////////////////////////
// PAQUETES       						//
//////////////////////////////////////////
typedef struct paqEjemplo {
	int valor;
	string mensaje;
}__attribute__((packed)) paqEjemplo;

//Funciones
/*--- puntuales ---*/
void* serealizarEjemplo(void* elemento, int* tamanio);
paqEjemplo * deserealizarEjemplo(void* buffer);
/*--- generales ---*/
void * serealizar(int tipoMensaje, void * elemento, int * tamanio);
void * deserealizar(int tipoMensaje, void * mensaje, int tamanio);
int enviarPaquete(int fdCliente, int tipoMensaje, void * mensaje, int* tamanioMensaje);
void * recibirPaquete(int fdCliente, int * tipoMensaje, int * tamanioMensaje);

#endif /* LIBCOMUN_SEREALIZACION_H_ */
