/*
 * archivoConfig.h
 *
 *  Created on: 29/7/2017
 *      Author: utnso
 */

#ifndef LIBCOMUN_ARCHIVOCONFIG_H_
#define LIBCOMUN_ARCHIVOCONFIG_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>	// perror
#include <commons/config.h>
#include <commons/log.h>
#include "logs.h"

#define FAIL -1

//Variables generales
extern t_log * vg_logger;

// Prototipos
int comprobarValoresBienSeteados(); // Redefinir en cada proyecto
void leerConfig(char * configPath);
void setearValores(t_config * archivoConfig); // Redefinir en cada proyecto
void leerArchivoDeConfiguracion(char * configPath);
int verificarExistenciaDeArchivo(char * ruta);

#endif /* LIBCOMUN_ARCHIVOCONFIG_H_ */
