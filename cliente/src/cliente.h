/*
 * cliente.h
 *
 *  Created on: 30/7/2017
 *      Author: utnso
 */

#ifndef SRC_CLIENTE_H_
#define SRC_CLIENTE_H_

#include <libComun/sockets.h>
#include <libComun/archivoConfig.h>
#include <libComun/logs.h>
#include <libComun/serealizacion.h>

//Variables
char* vg_path_config = NULL;
char* vg_path_base = NULL;
t_log * vg_logger;
char * vg_dir_log;

//Prototipos
int recibirArgumento(int argc, char* argv[]);

#endif /* SRC_CLIENTE_H_ */
