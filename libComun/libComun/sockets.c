/*
 * sockets.c
 *
 *  Created on: 29/7/2017
 *      Author: utnso
 */

#include "sockets.h"

// *******************************
// *           Servidor			 *
// *******************************
int newSocket() {
	int fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
		if (fileDescriptor == FAIL) mensaje_Error("Funcion socket");
	return fileDescriptor;
}

struct sockaddr_in asociarSocket(int sockfd, int puerto) {
	struct sockaddr_in miDireccion;
	miDireccion.sin_family = AF_INET;
	miDireccion.sin_port = htons(puerto);
	miDireccion.sin_addr.s_addr = 0; // htonl(INADDR_ANY); // Usa mi direccion IP
	memset(&(miDireccion.sin_zero), '\0', 8);

	// Si el puerto esta en uso, lanzamos error
	int enUso = 1;
	int estaEnUso = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*) &enUso, sizeof(enUso));
		if (estaEnUso == FAIL)	mensaje_Error("No es posible reutilizar el socket");

	// Funcion bind
	int funcionBind = bind(sockfd, (struct sockaddr *) &miDireccion,sizeof(struct sockaddr));
		if ( funcionBind == FAIL) mensaje_Error("Funcion BIND: No se pudo asociar con el puerto");

	return miDireccion;
}

void escucharSocket(int sockfd, int conexionesEntrantesPermitidas) {
	int funcionListen = listen(sockfd, conexionesEntrantesPermitidas);
		if ( funcionListen == FAIL) mensaje_Error("Funcion listen");
}

//Retorna el socket servidor creado
int levantarServidorParaNConexiones(int puerto, int cant_max_conexiones){
	int socket;

	socket = newSocket();
	asociarSocket(socket, puerto);
	escucharSocket(socket, cant_max_conexiones);

	mensaje_OK("Servidor levantado");

	return socket;
}

/**
* @NAME: agregarHilo
* @DESC: Agrega un nuevo hilo a la estructura de hilos del administrador de memoria
* @PARAMS: el ID del hilo creado
*
*/
void agregarHilo(pthread_t id_hilo, hilo ** lista_de_hilos){

	hilo * nuevo_hilo = NULL;

	nuevo_hilo = (hilo *) malloc(sizeof(hilo));

	nuevo_hilo->id_hilo = id_hilo;
	nuevo_hilo->siguiente = NULL;

	nuevo_hilo->siguiente = *lista_de_hilos;
	*lista_de_hilos = nuevo_hilo;

	mensaje_OK("Se creo un nuevo hilo");
}

void manejarServidorParaNConexiones(int socketServidor, hilo ** lista_de_hilos){
	int cliente = aceptarConexionSocket(socketServidor);

	while(cliente != FAIL){

		pthread_t id_hilo;
		pthread_create(&id_hilo, NULL, (void *)atenderCliente, &cliente);

		agregarHilo(id_hilo, lista_de_hilos);

		sleep(2);

		cliente = aceptarConexionSocket(socketServidor);
	}

	cerrarSocket(socketServidor);
}

/**
* @NAME: eliminarListaDeHilos
* @DESC: Elimina toda la lista de hilos del administrador de memoria
* @PARAMS:
*
*/
void eliminarListaDeHilos(hilo ** lista_de_hilos){

	mensaje_Info("Eliminando hilos...");

	hilo * id_hilo = NULL;

	for(id_hilo = *lista_de_hilos; *lista_de_hilos ; *lista_de_hilos = id_hilo->siguiente, free(id_hilo), id_hilo = *lista_de_hilos);
}

/**
* @NAME: esperarHilos
* @DESC: Realiza un pthread_join por cada hilo creado en el administrador de memoria
* @PARAMS:
*
*/
void esperarHilos(hilo ** lista_de_hilos){

	mensaje_Info("Esperando hilos...");

	hilo * hilo = NULL;

	for(hilo = *lista_de_hilos ; hilo ; pthread_join(hilo->id_hilo, NULL), hilo = hilo->siguiente);
}

void servidorNConexiones(int puerto, int cant_max_conexiones, hilo ** lista_de_hilos){

	int socketServidor = levantarServidorParaNConexiones(puerto, cant_max_conexiones);

	manejarServidorParaNConexiones(socketServidor, lista_de_hilos);

	esperarHilos(lista_de_hilos);

	eliminarListaDeHilos(lista_de_hilos);
}

int aceptarConexionSocket(int sockfd) {
	struct sockaddr_storage cliente;
	unsigned int addres_size = sizeof(cliente);

	int fdCliente = accept(sockfd, (struct sockaddr*) &cliente, &addres_size);
		if(fdCliente == FAIL) mensaje_Error("Funcion accept");

	return fdCliente;
}

// *******************************
// *           Cliente			 *
// *******************************
int conectarSocket(int sockfd, const char * ipDestino, int puerto){
	struct sockaddr_in datosServidor;

	datosServidor.sin_family = AF_INET;
	datosServidor.sin_port = htons(puerto);
	datosServidor.sin_addr.s_addr = inet_addr(ipDestino);
	memset(&(datosServidor.sin_zero), '\0', 8);

	int funcionConnect = connect(sockfd, (struct sockaddr *) &datosServidor, sizeof(struct sockaddr));
		if ( funcionConnect == FAIL) return FAIL;
	return 0;
}

int conectarCliente(char * ipDestino, int puerto){
	int socket;

	socket = newSocket();
	conectarSocket(socket, ipDestino, puerto);

	return socket;
}
// *******************************
// *       Enviar y Recibir		 *
// *******************************

int enviarPorSocket(int fdCliente, const void * mensaje, int tamanio) {
	int bytes_enviados;
	int total = 0;

	while (total < tamanio) {
		bytes_enviados = send(fdCliente, mensaje + total, tamanio, 0);
		if (bytes_enviados == FAIL) {
			break;
		}
		total += bytes_enviados;
		tamanio -= bytes_enviados;
	}
	if (bytes_enviados == FAIL) mensaje_Error ("Funcion send");

	return bytes_enviados;
}

int recibirPorSocket(int fdCliente, void * buffer, int tamanio) {
	int total = 0;
	int bytesRecibidos;

	while (total < tamanio) {
		bytesRecibidos = recv(fdCliente, buffer + total, tamanio, 0);
		if (bytesRecibidos == FAIL) {
			// Error
			mensaje_Error("Funcion recv");
			break;
		}
		if (bytesRecibidos == 0) {
			// Desconexion
			break;
		}
		total += bytesRecibidos;
		tamanio -= bytesRecibidos;
	}
	return bytesRecibidos;
}	// retorna -1 si fallo, 0 si se desconecto o los bytes recibidos

// *******************************
// *    Cerrar File Descriptor	 *
// *******************************
void cerrarSocket(int sockfd) {
	int funcionClose = close(sockfd);
	if (funcionClose == FAIL) mensaje_Error("Funcion CLOSE: Error al cerrar el file descriptor");
	char* mensaje = malloc(50);
	sprintf(mensaje, "Socket %d cerrado", sockfd);
	mensaje_OK(mensaje);
	free(mensaje);
}

// *******************************
// *       	  Select			 *
// *******************************
int seleccionarSocket(int maxNumDeFD,
					   fd_set *fdListoLectura, fd_set *fdListoEscritura, fd_set *fdListoEjecucion,
					   int* segundos, int* milisegundos) {

	if(segundos == NULL || milisegundos == NULL){
		// Si segundos = NULL o miliSegundos = NULL => espera infinitamente
		int funcionSelect = select((maxNumDeFD + 1), fdListoLectura, fdListoEscritura, fdListoEjecucion, NULL);
		if(funcionSelect == FAIL && errno != EINTR) mensaje_Error("Funcion select");
		return funcionSelect;

	}else{
		struct timeval tv;
		// Si segundos y milisegundos apuntan a 0 => select regresa inmediatamente despues de interrogar a todos sus fd
		int sec = *segundos;
		int mic = (*milisegundos) * 1000;
		tv.tv_sec  = sec;
		tv.tv_usec = mic;

		int selectConTiempo = select((maxNumDeFD + 1), fdListoLectura,
								  fdListoEscritura, fdListoEjecucion, &tv);
		if(selectConTiempo == FAIL && errno != EINTR) mensaje_Error("Funcion select");
		return selectConTiempo;
	}
}

int levantarServidorSelect(int puerto, fd_set** ListadoSockets){
	int socket;

	socket = newSocket();
	asociarSocket(socket, puerto);
	fd_set* fdListadoSockets= *ListadoSockets;
	FD_ZERO(fdListadoSockets);
	FD_SET(socket, fdListadoSockets);
	int returnSelect = seleccionarSocket(socket + 1, fdListadoSockets, NULL, NULL, NULL, NULL);

	if (returnSelect < 0){
		socket = -1;
		mensaje_Error("Servidor NO levantado");
	}else{
		mensaje_OK("Servidor levantado");
	}

	return socket;
}

void manejarServidorSelect(fd_set** ListadoSockets, int socketServidor){

	fd_set* fdListadoSockets = *ListadoSockets;
	int returnSelect;
	int clienteNuevo;
	int maxSelect = socketServidor + 1;

	while(returnSelect != FAIL){
		int i;
		for(i = 0; i < maxSelect;){
			if (i == socketServidor && FD_ISSET(socketServidor, fdListadoSockets)){
				clienteNuevo = aceptarConexionSocket(socketServidor);
				if (clienteNuevo != FAIL){
					FD_SET(clienteNuevo, fdListadoSockets);
					if (clienteNuevo > maxSelect) maxSelect = clienteNuevo;
					char* mostrar = malloc(100);
					sprintf(mostrar, "Se conecto un nuevo cliente n°%d", clienteNuevo);
					mensaje_OK(mostrar);
					free(mostrar);
				}
			}

			if (i != socketServidor && FD_ISSET(i, fdListadoSockets)) {
				int respuesta = atenderClienteSocket(i);
				if (respuesta == tRespError){
					FD_CLR(i, fdListadoSockets);
					char* texto = malloc(100);
					sprintf(texto, "[INFO] Se desconecto el Cliente: %d", i);
					log_info(vg_logger, texto);
					mensaje_Color(COLOR_AMARILLO, texto);
					free(texto);
					cerrarSocket(i);
				}
			}
		}

		returnSelect = seleccionarSocket(maxSelect, fdListadoSockets, NULL, NULL, NULL, NULL);
	}

	cerrarSocket(socketServidor);
}

void servidorSelect(int puerto){

	fd_set* listadoSockets;

	int socketServidor = levantarServidorSelect(puerto, &listadoSockets);

	manejarServidorSelect(&listadoSockets, socketServidor);
}

/**
 * @NAME: atenderCliente
 * @DESC: Se encarga de atender el cliente que se pasa por parametro, al momento
 *        de utilizar se debe declarar la funcion "atenderPedido"
 * @PARAMS: el descriptor del cliente que se quiere atender
 *
 */
void* atenderCliente(int* fdClienteNuevo){
	int fdCliente = *(int *) fdClienteNuevo;
	int tipoMensaje = 0;
	int tamanioMensaje = 0;
	void * mensaje = NULL;
	mensaje = recibirPaquete(fdCliente, &tipoMensaje, &tamanioMensaje);
	while (mensaje != NULL) {
		//recibi un pedido de CPU lo tratamos aca abajo
		atenderPedido(fdCliente, tipoMensaje, mensaje, tamanioMensaje);
		free(mensaje);
		mensaje = recibirPaquete(fdCliente, &tipoMensaje, &tamanioMensaje);
	}

	char* texto = malloc(100);
	sprintf(texto, "[INFO] Se desconecto el Cliente: %d", fdCliente);
	log_info(vg_logger, texto);
	mensaje_Color(COLOR_AMARILLO, texto);
	free(texto);
	cerrarSocket(fdCliente);
	return NULL;
}


/**
 * @NAME: atenderClienteSocket
 * @DESC: Se encarga de atender el cliente que se pasa por parametro, al momento
 *        de utilizar se debe declarar la funcion "atenderPedido"
 * @PARAMS: el descriptor del cliente que se quiere atender
 *
 */
int atenderClienteSocket(int fdClienteNuevo){
	int respuesta = tRespOK;
	int tipoMensaje = 0;
	int tamanioMensaje = 0;
	void * mensaje = NULL;
	mensaje = recibirPaquete(fdClienteNuevo, &tipoMensaje, &tamanioMensaje);
	if (mensaje != NULL) atenderPedido(fdClienteNuevo, tipoMensaje, mensaje, tamanioMensaje);
	if (mensaje == NULL) respuesta = tRespError;
	free(mensaje);

	return respuesta;
}
