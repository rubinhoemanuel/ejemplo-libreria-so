/*
 * servidorSelect.c
 *
 *  Created on: 1/8/2017
 *      Author: utnso
 */

#include "servidorSelect.h"

int vg_puerto = 6000;
int vg_cant_conexiones = 50;

int main(int argc, char *argv[]) {

	inicializar_color();

	int argOK = recibirArgumento(argc, argv);

	if (argOK == 1) {
		mensaje_Error("Cantidad de argumentos invalido");
		return argOK;
	}

	vg_logger = crearLogger(vg_dir_log, "Proceso Servidor Select");

	leerConfig(vg_path_config); free(vg_path_config);

	servidorSelect(vg_puerto);

	inicializar_color();
	return EXIT_SUCCESS;
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

void manejarMensajeEjemplo(paqEjemplo * mensaje, int fdCliente){
	char* linea = malloc(100);
	memset(linea, 0, 100);
	sprintf(linea, "Valor recibido: %d", mensaje->valor);
	mensaje_Color(COLOR_MAGENTA, linea);

	memset(linea, 0, 100);
	sprintf(linea, "Tamanio de string: %d", mensaje->mensaje.tamanio);
	mensaje_Color(COLOR_MAGENTA, linea);

	memset(linea, 0, 100);
	sprintf(linea, "Mensaje: %s", mensaje->mensaje.cadena);
	mensaje_Color(COLOR_MAGENTA, linea);

	free(linea);
}

void atenderPedido(int fdCliente, int tipoMensaje, void * mensaje, int tamanioMensaje){

	//Semaforo IN

	switch (tipoMensaje) {
	case tEjemplo:
		manejarMensajeEjemplo((paqEjemplo *) mensaje, fdCliente);
		break;
	}

	//Semaforo OFF
}

void setearValores(t_config * archivoConfig){ }

int comprobarValoresBienSeteados(){	return 1; }

