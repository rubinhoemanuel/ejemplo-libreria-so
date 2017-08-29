/*
 * cliente.c
 *
 *  Created on: 30/7/2017
 *      Author: utnso
 */

#include "cliente.h"

int vg_puerto = 6000;
char* vg_ipDestino = "127.0.0.1";

int main(int argc, char *argv[]) {

	inicializar_color();

	int argOK = recibirArgumento(argc, argv);

	if (argOK == 1) {
		mensaje_Error("Cantidad de argunmentos invalido");
		return argOK;
	}

	vg_logger = crearLogger(vg_dir_log, "Proceso Memoria");

	leerConfig(vg_path_config); free(vg_path_config);

	//Conectar cliente con el servidor
	int socketServidor = conectarCliente(vg_ipDestino, vg_puerto);

	//Enviar paquete
	paqEjemplo * paquete = malloc(sizeof(paqEjemplo));
	paquete->valor = 1991;
	paquete->mensaje.cadena = malloc(18);
	memset(paquete->mensaje.cadena, 0, 18);
	sprintf(paquete->mensaje.cadena, "mensaje de prueba");
	paquete->mensaje.cadena[17]='\0';
	paquete->mensaje.tamanio = strlen(paquete->mensaje.cadena);
//	int tamanioPaquete = sizeof(paqEjemplo) - sizeof(char*) + paquete->mensaje.tamanio;
	int tamanioPaquete = 0;

	int respuesta = enviarPaquete(socketServidor, tEjemplo, paquete, &tamanioPaquete);
	while(respuesta > 0){

	char* porPantalla = malloc(100);
	memset(porPantalla, 0, 100);
	sprintf(porPantalla, "Respuesta del enviarPaquete %d", respuesta);
	mensaje_Info(porPantalla);
	free(porPantalla);
		sleep(4);
		respuesta = enviarPaquete(socketServidor, tEjemplo, paquete, &tamanioPaquete);
	}

	cerrarSocket(socketServidor);
}

int recibirArgumento(int argc, char* argv[]) {

	if (argc - 1 != 1) {
		return 1;
	}

	char * aux = (char *) malloc(strlen(argv[1]) + 1);

	memset(aux, 0, strlen(argv[1]) + 1);
	memcpy(aux, argv[1], strlen(argv[1]));
	aux[strlen(argv[1])] = '\0';
	vg_path_base = aux;

	char * cfg = "src/archConfig.cfg";
	vg_path_config = malloc(strlen(vg_path_base) + strlen(cfg) + 1);
	memset(vg_path_config, 0, strlen(vg_path_base) + strlen(cfg) + 1);
	memcpy(vg_path_config, vg_path_base, strlen(vg_path_base));
	memcpy(vg_path_config + strlen(vg_path_base), cfg, strlen(cfg));
	vg_path_config[strlen(vg_path_base) + strlen(cfg)] = '\0';

	char * logs = "logs.txt";

	vg_dir_log = malloc(strlen(vg_path_base) + strlen(logs) + 1);
	memset(vg_dir_log, 0, strlen(vg_path_base) + strlen(logs) + 1);
	memcpy(vg_dir_log, vg_path_base, strlen(vg_path_base));
	memcpy(vg_dir_log + strlen(vg_path_base), logs, strlen(logs));
	vg_dir_log[strlen(vg_path_base) + strlen(logs)] = '\0';

	return 0;
}

void atenderPedido(int fdCliente, int tipoMensaje, void * mensaje, int tamanioMensaje){};

void setearValores(t_config * archivoConfig){ }

int comprobarValoresBienSeteados(){	return 1; }
