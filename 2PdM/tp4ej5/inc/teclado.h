/**************************************************************************************************
 ** (c) Copyright 2019: Esteban VOLENTINI <evolentini@gmail.com>
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

#ifndef TECLADO_H   /*! @cond    */
#define TECLADO_H   /*! @endcond */

/** @file teclado.h
 ** @brief Declaraciones de la libreria para gestion del teclado
 **
 ** Libreria para el manejo de un teclado utilizando maquina de estados finitos
 ** para la practica de la materia de Programación de Microcontroladores, 
 ** perteneciente a la Especialización en Sistemas Embebidos.
 **
 **| REV | YYYY.MM.DD | Autor           | Descripción de los cambios                              |
 **|-----|------------|-----------------|---------------------------------------------------------|
 **|   1 | 2019.04.03 | evolentini      | Version inicial del archivo                             |
 **
 ** @defgroup PdM Programacion de Microcontroladores
 ** @brief Trabajo Practico para Programación de Microcontroladores 
 ** @{ */

/* === Inclusiones de archivos externos ======================================================== */

#include "sapi.h"

/* === Cabecera C++ ============================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Definicion y Macros ===================================================================== */

/* === Declaraciones de tipos de datos ========================================================= */

/** @brief Terminal de entrada digital */
typedef gpioMap_t tecla_terminal_t;

typedef void (*tecla_evento_t)(bool presionada, void * datos);

/** @brief Descriptor del estado de un tecla */
typedef struct tecla_s * tecla_t;

/* === Declaraciones de variables externas ===================================================== */

/* === Declaraciones de funciones externas ===================================================== */

/** @brief  Configura la supervision de una tecla
 * 
 * @param[in]   terminal    Terminal de entrada digital al que se conecta la tecla
 * @param[in]   evento      Funcion a la que se llama cuando se presiona o libera la tecla
 * @param[in]   datos       Puntero con una estructura para utilizar en evento de tecla
 * 
 * @return      Descriptor del estado de tecla
 */
tecla_t tecla_configurar(tecla_terminal_t terminal, tecla_evento_t evento, void * datos);

/** @brief  Actualiza el estado de una tecla
 * 
 * @param[in]   tecla       Descriptor devuelto por la funcion al configurar la tecla
 */
void tecla_actualizar(tecla_t tecla);

/* === Ciere de documentacion ================================================================== */
#ifdef __cplusplus
}
#endif

/** @} Final de la definición del modulo para doxygen */

#endif   /* TECLADO_H */
