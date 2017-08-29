/*
 * sockets.h
 *
 *  Created on: 29/7/2017
 *      Author: utnso
 */

#ifndef SOCKETS_H_
#define SOCKETS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>     //memset
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <errno.h>      //perror
#include <arpa/inet.h>  //INADDR_ANY
#include <unistd.h>     //close  usleep
#include <netdb.h> 		//gethostbyname
#include <netinet/in.h>
#include <fcntl.h> //fcntl
#include <pthread.h>
#include "printColor.h"
#include "logs.h"

//////////////////////////////////////////
// RESPUESTA     						//
//////////////////////////////////////////
typedef enum t_respuesta {
	tRespOK=1,
	tRespWarning,
	tRespError
} t_respuesta;

typedef struct nodo_hilo{
	pthread_t id_hilo;
	struct nodo_hilo * siguiente;
} hilo;

#define FAIL -1

extern t_log* vg_logger;

// Servidor
int  newSocket();
struct sockaddr_in asociarSocket(int sockfd, int puerto);
void escucharSocket(int sockfd, int conexionesEntrantesPermitidas);
int levantarServidorParaNConexiones(int puerto, int cant_max_conexiones);
void manejarServidorParaNConexiones(int socketServidor, hilo ** lista_de_hilos);
void servidorNConexiones(int puerto, int cant_max_conexiones, hilo ** lista_de_hilos);
int  aceptarConexionSocket(int sockfd);

// Cliente
int conectarSocket(int sockfd, const char * ipDestino, int puerto);
int conectarCliente(char * ipDestino, int puerto);

// Enviar y Recibir
int enviarPorSocket(int fdCliente, const void * mensaje, int tamanio);
int recibirPorSocket(int fdCliente, void * buffer, int tamanio) ; // retorna -1 si fallo, 0 si se desconecto o los bytes recibidos

// Cerrar File Descriptor
void cerrarSocket(int sockfd);

// Select
int seleccionarSocket(int maxNumDeFD,
					   fd_set * fdListoLectura, fd_set * fdListoEscritura, fd_set * fdListoEjecucion,
					   int* segundos, int* miliSegundos);
int levantarServidorSelect(int puerto, fd_set** ListadoSockets);
void manejarServidorSelect(fd_set** ListadoSockets, int socketServidor);
void servidorSelect(int puerto);
void* atenderCliente(int* fdCliente);
void atenderPedido(int fdCliente, int tipoMensaje, void * mensaje, int tamanioMensaje);

#endif /* SOCKETS_H_ */
