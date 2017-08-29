/*
 * printColor.h
 *
 *  Created on: 29/7/2017
 *      Author: utnso
 */

#ifndef LIBCOMUN_PRINTCOLOR_H_
#define LIBCOMUN_PRINTCOLOR_H_

/*---------Define de colores---------*/
#define COLOR_ROJO     "\x1b[31m"
#define COLOR_VERDE    "\x1b[32m"
#define COLOR_AMARILLO "\x1b[33m"
#define COLOR_AZUL     "\x1b[34m"
#define COLOR_MAGENTA  "\x1b[35m"
#define COLOR_CYAN     "\x1b[36m"
#define COLOR_RESET    "\x1b[0m"
/*-----------------------------------*/

/*------- Funciones -----------------*/
void inicializar_color();
void mensaje_Error(char * mensaje);
void mensaje_Info(char * mensaje);
void mensaje_OK(char * mensaje);
void mensaje_Color(char* color, char* mensaje);

#endif /* LIBCOMUN_PRINTCOLOR_H_ */
