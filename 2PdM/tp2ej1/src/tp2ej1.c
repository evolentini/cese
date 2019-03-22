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
        LEDR, LEDB, LEDR, LEDG, LED2, LED1, LED2, LED3,
    };

    /* Inicializar la placa */
    boardConfig();

    // LED_parpadear(LEDB, 1000);

    LED_parpadear_n_veces(LEDR, 600, 5);

    LED_secuencia_arbitraria(200, secuencia);

    LED_secuencia_arbitraria_n(400, secuencia_n, 8);

    while(1) {
    }
}
/* === Ciere de documentacion ================================================================== */

/** @} Final de la definición del modulo para doxygen */

