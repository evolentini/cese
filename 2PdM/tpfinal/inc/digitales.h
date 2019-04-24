/**************************************************************************************************
 ** (c) Copyright 2019: Esteban VOLENTINI <evolentini@gmail.com>
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

#ifndef DIGITALES_H   /*! @cond    */
#define DIGITALES_H   /*! @endcond */

/** @file digitales.h
 ** @brief Declaraciones de la libreria para gestion de entradas digitales
 **
 ** Libreria para el manejo de entradas digitales utilizando maquina de estados finitos
 ** para la eliminación de rebotes y transitorios.
 **
 **| REV | YYYY.MM.DD | Autor           | Descripción de los cambios                              |
 **|-----|------------|-----------------|---------------------------------------------------------|
 **|   2 | 2019.04.23 | evolentini      | Renombrado y generalizacion para entradas digitales     |
 **|   1 | 2019.04.03 | evolentini      | Version inicial del archivo para manejo de teclado      |
 **
 ** @defgroup PdM Programacion de Microcontroladores
 ** @brief Trabajo Practico para Programación de Microcontroladores 
 ** @{ */

/* === Inclusiones de archivos externos ======================================================== */
#include "gpio.h"

/* === Cabecera C++ ============================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Definicion y Macros ===================================================================== */

/* === Declaraciones de tipos de datos ========================================================= */

typedef void (*entrada_evento_t)(bool estado, void * datos);

/** @brief Descriptor del estado de un entrada */
typedef struct entrada_s * entrada_t;

/* === Declaraciones de variables externas ===================================================== */

/* === Declaraciones de funciones externas ===================================================== */

/** @brief  Configura la supervision de una entrada digital
 * 
 * @param[in]   terminal    Terminal de entrada digital a controlar
 * @param[in]   evento      Funcion a la que se llama cuando se activa o libera la entrada
 * @param[in]   datos       Puntero para enviar como parametro a la funcion que gestiona el evento
 * 
 * @return      Descriptor del estado de la entrada digital
 */
entrada_t EntradaConfigurar(gpio_t terminal, entrada_evento_t evento, void * datos);

/** @brief  Actualiza el estado de una entrada digital
 * 
 * @param[in]   entrada       Descriptor devuelto por la funcion al configurar la entrada digital
 * 
 * @return      Estado actual de la entrada despues de la actualizacion
 */
bool EntradaActualizar(entrada_t entrada);

/* === Ciere de documentacion ================================================================== */
#ifdef __cplusplus
}
#endif

/** @} Final de la definición del modulo para doxygen */

#endif   /* DIGITALES_H */
