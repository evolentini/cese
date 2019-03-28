/**************************************************************************************************
 **  (c) Copyright 2019: Esteban VOLENTINI
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

/** @file tp2ej1.c
 ** @brief Programa principal del Ejercicio 1 del Trabajo Practico 2
 **
 ** Programa principal para probar libreria con ejemplos de funciones de GPIO para la materia de 
 ** Programación de Microcontroladores, perteneciente a la Especialización en Sistemas Embebidos.
 **
 **| REV | YYYY.MM.DD | Autor           | Descripción de los cambios                              |
 **|-----|------------|-----------------|---------------------------------------------------------|
 **|   1 | 2019.03.21 | evolentini      | Version inicial del archivo                             |
 **
 ** @defgroup PdM Programacion de Microcontroladores
 ** @brief Trabajo Practico para Programación de Microcontroladores 
 ** @{ */

/* === Inclusiones de cabeceras ================================================================ */

#include "sapi.h"
#include "libreria.h"

/* === Definicion y Macros ===================================================================== */

/* === Definiciones de variables internas ====================================================== */

/* === Definiciones de funciones internas =====================================================- */

/* === Definiciones de funciones externas ====================================================== */

void main(void) {
    const gpio_t secuencia[] = {
        LEDR, LEDB, LEDR, LEDG
    };

    const gpio_t secuencia_n[] = {
        LED1, LED2, LED1, LED3, LED1, LED2, LED3
    };

    led_parpadeo_t parpadeo_azul;
    led_parpadeo_t parpadeo_rojo;
    led_parpadeo_t parpadeo_verde;
    led_secuencia_t secuencia_leds;

    /* Inicializar la placa */
    boardConfig();

    /* Ejemplos de uso de las funciones bloqueantes */
    #if(0)
    LED_parpadear_n_vecesLEDR, 600, 5);
    LED_parpadearLEDB, 1000);
    LED_secuencia_arbitraria(200, secuencia);
    LED_secuencia_arbitraria_n(400, secuencia_n, 8);
    #endif

    /* Configuración de tres parpadeos y una secuencia no bloqueantes */
    parpadeo_azul = LED_iniciar_parpadeo(LEDB, 200, 0);
    parpadeo_rojo = LED_iniciar_parpadeo(LEDR, 400, 4);
    parpadeo_verde = LED_iniciar_parpadeo(LEDG, 800, 8);
    secuencia_leds = LED_iniciar_secuencia(400, secuencia_n, 7);

    while(1) {
        /* Refresco de los parpadeos y la secuencia */
        LED_mantener_parpadeo(parpadeo_azul);
        LED_mantener_parpadeo(parpadeo_rojo);
        LED_mantener_parpadeo(parpadeo_verde);
        LED_mantener_secuencia(secuencia_leds);
    }
}
/* === Ciere de documentacion ================================================================== */

/** @} Final de la definición del modulo para doxygen */

