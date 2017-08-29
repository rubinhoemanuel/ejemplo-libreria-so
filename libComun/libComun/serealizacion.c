/*
 * serealizacion.c
 *
 *  Created on: 29/7/2017
 *      Author: utnso
 */

#include "serealizacion.h"

//Puntuales
void * serealizarEjemplo(void* elemento, int* tamanio){
	paqEjemplo * paquete = (paqEjemplo *) elemento;
	int desplazamiento = 0;
	int tamanioString = (sizeof(char) * paquete->mensaje.tamanio);
	*tamanio = sizeof(paqEjemplo) - sizeof(int) + tamanioString;

	void * buffer = malloc(*tamanio);
	memcpy(buffer + desplazamiento, &(paquete->valor), sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(buffer + desplazamiento, &(paquete->mensaje.tamanio), sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(buffer + desplazamiento, paquete->mensaje.cadena, tamanioString);
	return buffer;
}

paqEjemplo * deserealizarEjemplo(void* buffer){
	int desplazamiento = 0;
	paqEjemplo* paquete = malloc(sizeof(paqEjemplo));
	memcpy(&paquete->valor, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(&paquete->mensaje.tamanio, buffer + desplazamiento, sizeof(int));
	desplazamiento += sizeof(int);
	paquete->mensaje.cadena = malloc(paquete->mensaje.tamanio * sizeof(char)+1);
	memcpy(paquete->mensaje.cadena, buffer + desplazamiento, paquete->mensaje.tamanio);
	paquete->mensaje.cadena[paquete->mensaje.tamanio * sizeof(char)]='\0';
	return paquete;
}

//Generales

void * serealizar(int tipoMensaje, void * elemento, int * tamanio) {
	void * buffer;
	switch (tipoMensaje) {
		case tEjemplo: {
			buffer = serealizarEjemplo(elemento, tamanio);
			break;
		}
		case 0: {
			mensaje_Error("Desconexion");
			break;
		}
		default: {
			mensaje_Error("Se recibio un mensaje que no esta en el protocolo");
			abort();
			break;
		}
	}
	return buffer;
}

void * deserealizar(int tipoMensaje, void * mensaje, int tamanio) {
	void * buffer;
	switch (tipoMensaje) {
		case tEjemplo: {
			buffer = deserealizarEjemplo(mensaje);
			break;
		}
		case 0: {
			mensaje_Error("Desconexion");
			break;
		}
		default: {
			mensaje_Error("Se recibio un mensaje que no esta en el protocolo");
			abort();
			break;
		}
	}
	return buffer;
}

int enviarPaquete(int fdCliente, int tipoMensaje, void * mensaje, int* tamanioMensaje) {

	int desplazamiento = 0;

	void * mensajeSerealizado = serealizar(tipoMensaje, mensaje, tamanioMensaje);

	int tamanioTotal = (sizeof(int) * 2) + *tamanioMensaje;

	void * buffer = malloc(tamanioTotal);
	memcpy(buffer + desplazamiento, &tipoMensaje, sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(buffer + desplazamiento, tamanioMensaje, sizeof(int));
	desplazamiento += sizeof(int);
	memcpy(buffer + desplazamiento, mensajeSerealizado, *tamanioMensaje);

	// Envia la totalidad del paquete
	int respuesta = enviarPorSocket(fdCliente, buffer, tamanioTotal);

	if (respuesta == FAIL){
		mensaje_Error("Paquete no enviado");
	}else{
		char* msj = malloc(100);
		memset(msj, 0, 100);
		sprintf(msj, "Paquete enviado correctamente. %d bytes enviados", respuesta);
		mensaje_OK(msj);
		free(msj);
	}

	free(buffer);

	free(mensajeSerealizado);

	return respuesta;
}

void * recibirPaquete(int fdCliente, int * tipoMensaje, int * tamanioMensaje) {
	// Recibe el tipoMensaje

	int recibido = recibirPorSocket(fdCliente, tipoMensaje, sizeof(int));

	if (*tipoMensaje < 1 || *tipoMensaje > tFinDeProtocolo || recibido <= 0) {
		return NULL;
	}

	// Recibe el tamanioMensaje
	recibirPorSocket(fdCliente, tamanioMensaje, sizeof(int));

	void * mensaje = malloc(*tamanioMensaje);
	recibirPorSocket(fdCliente, mensaje, *tamanioMensaje);
	void * buffer = deserealizar(*tipoMensaje, mensaje, *tamanioMensaje);
	free(mensaje);
	return buffer;

} // Recordar castear
