/**************************************************************************************************
 **  (c) Copyright 2019: Esteban VOLENTINI
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

/** @file biometricos.c
 ** @brief Implementación de la libreria con ejemplos de funciones de GPIO
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

/* === Inclusiones de cabeceras ================================================================ */

#include <assert.h>

#include "libreria.h"

/* === Definicion y Macros ===================================================================== */

/* === Definiciones de variables internas ====================================================== */

/* === Definiciones de funciones internas =====================================================- */

/* === Definiciones de funciones externas ====================================================== */

void LED_parpadear(gpio_t led, uint16_t periodo) {
    LED_secuencia_arbitraria_n(periodo, &led, 1);
}

void LED_parpadear_n_veces(gpio_t led, uint16_t periodo, uint8_t n_veces) {
    int indice;

    assert(periodo >= 100 && periodo <= 1000);
    assert(n_veces >= 1 && n_veces <= 10);

    for(indice = 0; indice < n_veces; indice++) {
        gpioWrite(led, ON);
        delay(periodo >> 1);
        gpioWrite(led, OFF);
        delay(periodo >> 1);
    }
}

void LED_secuencia_fija(uint16_t periodo) {
    const gpio_t secuencia[] = {
        LEDB, LED1, LED2, LED3
    };

    LED_secuencia_arbitraria_n(periodo, secuencia, 4);
}

void LED_secuencia_arbitraria(uint16_t periodo, const gpio_t * psecuencia) {
    LED_secuencia_arbitraria_n(periodo, psecuencia, 4);
}

void LED_secuencia_arbitraria_n(uint16_t periodo, const gpio_t * psecuencia, uint8_t n_leds) {
    int indice = 0;

    assert(periodo >= 100 && periodo <= 1000);
    assert(n_leds >= 1 && n_leds <= 10);

    while(1) {
        gpioWrite(psecuencia[indice], ON);
        delay(periodo >> 1);
        gpioWrite(psecuencia[indice], OFF);
        delay(periodo >> 1);

        indice = (indice + 1) % n_leds;
    }
}

/* === Ciere de documentacion ================================================================== */

/** @} Final de la definición del modulo para doxygen */

