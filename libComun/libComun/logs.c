/*
 * logs.c
 *
 *  Created on: 29/7/2017
 *      Author: utnso
 */

#include "logs.h"

// Logger
t_log* crearLogger(char * dir_log, char * logMemoNombreArch) {

	t_log * logger = log_create(dir_log, logMemoNombreArch, false, LOG_LEVEL_INFO);
	return logger;
}
