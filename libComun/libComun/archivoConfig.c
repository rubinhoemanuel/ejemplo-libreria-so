/*
 * archivoConfig.c
 *
 *  Created on: 29/7/2017
 *      Author: utnso
 */
#include "archivoConfig.h"

void leerConfig(char * configPath) {
	leerArchivoDeConfiguracion(configPath);
//free(configPath);
	if (comprobarValoresBienSeteados()) {
		log_info(vg_logger, "Archivo de configuracion leido correctamente");
	}
}

/**
 * @NAME: leerArchivoDeConfiguracion
 * @DESC: Se encarga de leer el archivo que se pasa por parametro,
 * 			al momento de utilizar se debe declarar la funcion "verificarExistenciaDeArchivo" y "setearValores"
 * @PARAMS: el path del archivo de configuracion
 *
 */
void leerArchivoDeConfiguracion(char * configPath) {
	t_config * archivoConfig;

	if (verificarExistenciaDeArchivo(configPath) == FAIL)
		mensaje_Error("Archivo de configuracion no encontrado");

	archivoConfig = config_create(configPath);
	setearValores(archivoConfig);
	config_destroy(archivoConfig);
}

int verificarExistenciaDeArchivo(char* rutaArchivoConfig) {
	FILE * archivoConfig = fopen(rutaArchivoConfig, "r");
	if (archivoConfig!=NULL){
		fclose(archivoConfig);
		return 1;
	}
	return FAIL;
}

