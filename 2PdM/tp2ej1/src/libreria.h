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
 **|   2 | 2019.03.27 | evolentini      | Versiones no bloqueantes de parpadeos y secuencias      |
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

/** @brief Descriptor de un led */
typedef gpioMap_t gpio_t;

/** @brief Descriptor del estado de un parpadeo */
typedef struct led_parpadeo_s * led_parpadeo_t;

/** @brief Descriptor del estado de una secuencia */
typedef struct led_secuencia_s * led_secuencia_t;

/* === Declaraciones de variables externas ===================================================== */

/* === Declaraciones de funciones externas ===================================================== */

/** @brief  Hace parpadear un led
 * 
 * @param[in]   led         Descriptor del led que se desea hacer parpadear
 * @param[in]   periodo     Valor en milisegundos que demora cada paso de la secuencia
 */
void LED_parpadear(gpio_t led, uint16_t periodo);

/** @brief  Hace parpadear un led una cantidad de veces
 * 
 * @param[in]   led         Descriptor del led que se desea hacer parpadear
 * @param[in]   periodo     Valor en milisegundos que demora cada paso de la secuencia
 * @param[in]   n_veces     Cantidad de veces que parpadea el led
 */
void LED_parpadear_n_veces(gpio_t led, uint16_t periodo, uint8_t n_veces);

/** 
 * @brief Prende en secuencia los cuatro leds
 * 
 * @param[in]   periodo     Valor en milisegundos que demora cada paso de la secuencia
 */
void LED_secuencia_fija(uint16_t periodo);

/** 
 * @brief Muestra una secuencia arbitraria de cuatro leds
 * 
 * @param[in]   periodo     Valor en milisegundos que demora cada paso de la secuencia
 * @param[in]   psecuencia  Arreglo de pines de gpio que define la secuencia
 */
void LED_secuencia_arbitraria(uint16_t periodo, const gpio_t * psecuencia);

/** 
 * @brief Muestra una secuencia arbitraria de hasta diez elementos en los leds
 * 
 * @param[in]   periodo     Valor en milisegundos que demora cada paso de la secuencia
 * @param[in]   psecuencia  Arreglo de pines de gpio que define la secuencia
 * @param[in]   n_leds      Cantidad de entradas en el arreglo que define la secuencia
 */
void LED_secuencia_arbitraria_n(uint16_t periodo, const gpio_t * psecuencia, uint8_t n_leds);

/** @brief  Inicia el parpadeo de un led
 * 
 * @param[in]   led         Descriptor del led que se desea hacer parpadear
 * @param[in]   periodo     Valor en milisegundos que demora cada paso de la secuencia
 * @param[in]   n_veces     Cantidad de veces que parpadea. Con el valor cero parpadea indefinidamente.
 * 
 * @return      Descriptor del estado del parpadeo
 */
led_parpadeo_t LED_iniciar_parpadeo(gpio_t led, uint16_t periodo, uint8_t n_veces);

/** @brief  Actualiza el parpadeo del un led
 * 
 * @param[in]   led         Descriptor devuelto por la funcion al iniciar el parpadeo
 */
void LED_mantener_parpadeo(led_parpadeo_t descriptor);

/** @brief  Inicia una secuencia de leds
 * 
 * @param[in]   periodo     Valor en milisegundos que demora cada paso de la secuencia
 * @param[in]   secuencia   Vector con los descriptores de los leds que definen la secuencia
 * @param[in]   n_leds      Cantidad de elementos en la secuencia
 * 
 * @return      Descriptor del estado de la secuencia
 */
led_secuencia_t LED_iniciar_secuencia(uint16_t periodo, const gpio_t * secuencia, uint8_t n_leds);

/** @brief  Actualiza una secuencia de leds
 * 
 * @param[in]   led         Descriptor devuelto por la funcion al iniciar la secuencia
 */
void LED_mantener_secuencia(led_secuencia_t descriptor);

/* === Ciere de documentacion ================================================================== */
#ifdef __cplusplus
}
#endif

/** @} Final de la definición del modulo para doxygen */

#endif   /* LIBRERIA_H */
