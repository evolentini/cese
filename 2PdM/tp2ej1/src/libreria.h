/**************************************************************************************************
 **  (c) Copyright: EQUISER, http://www.equiser.com.ar
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

#ifndef LIBRERIA_H   /*! @cond    */
#define LIBRERIA_H   /*! @endcond */

/** @file libreria.h
 ** @brief Declaraciones de la libreria con ejemplos de funciones de GPIO
 **
 ** Libreria con ejemplos de funciones de GPIO para la materia de Programación de 
 ** Microcontroladores, perteneciente a la Especialización en Sistemas Embebidos.
 **
 **| REV | YYYY.MM.DD | Autor           | Descripción de los cambios                              |
 **|-----|------------|-----------------|---------------------------------------------------------|
 **|   1 | 2019.03.21 | evolentini      | Version inicial del archivo                             |
 **
 ** @defgroup PdM Programacion de Microcontroladores
 ** @brief Trabajo Practico para Programación de Microcontroladores 
 ** @{ */

/* === Inclusiones de archivos externos ======================================================== */
#include <stdint.h>
#include <stdbool.h>

#include "sapi.h"

/* === Cabecera C++ ============================================================================ */
#ifdef __cplusplus
extern "C" {
#endif

/* === Definicion y Macros ===================================================================== */

/* == Declaraciones de tipos de datos ========================================================== */

typedef gpioMap_t gpio_t;

/* === Declaraciones de variables externas ===================================================== */

/* === Declaraciones de funciones externas ===================================================== */

void LED_parpadear(gpio_t led, uint16_t periodo);

void LED_parpadear_n_veces(gpio_t led, uint16_t periodo, uint8_t n_veces);

void LED_secuencia_fija(uint16_t periodo);

void LED_secuencia_arbitraria(uint16_t periodo, const gpio_t * psecuencia);

/** 
 * @brief Muestra una secuencia arbitraria en los leds
 * 
 * @param[in]   periodo     Valor en milisegundos que demora cada paso de la secuencia
 * @param[in]   psecuencia  Arreglo de pines de gpio que define la secuencia
 * @param[in]   n_leds      Cantidad de entradas en el arreglo que define la secuencia
 */
void LED_secuencia_arbitraria_n(uint16_t periodo, const gpio_t * psecuencia, uint8_t n_leds);

/* === Ciere de documentacion ================================================================== */
#ifdef __cplusplus
}
#endif

/** @} Final de la definición del modulo para doxygen */

#endif   /* LIBRERIA_H */
