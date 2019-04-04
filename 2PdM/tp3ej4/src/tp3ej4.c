
/**************************************************************************************************
 **  (c) Copyright 2019: Esteban VOLENTINI
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

/** @file tp3ej4.c
 ** @brief Programa principal del Ejercicio 4 del Trabajo Practico 3
 **
 ** Programa principal para probar el modulo que gestiona las cuatro teclas mediante una MEF,
 ** el modulo de manejo una consola serial mediante la UART y el modulo de gestion del conversor
 ** analogico digital.
 **
 **| REV | YYYY.MM.DD | Autor           | Descripción de los cambios                              |
 **|-----|------------|-----------------|---------------------------------------------------------|
 **|   1 | 2019.04.03 | evolentini      | Version inicial del archivo                             |
 **
 ** @defgroup PdM Programacion de Microcontroladores
 ** @brief Trabajo Practico para Programación de Microcontroladores 
 ** @{ */

/* === Inclusiones de cabeceras ================================================================ */

#include "sapi.h"
#include "teclado.h"

/* === Definicion y Macros ===================================================================== */

/* === Definiciones de variables internas ====================================================== */


/* === Definiciones de funciones internas =====================================================- */

/* === Definiciones de funciones externas ====================================================== */
void evento_tecla(bool activa, void * datos) {
    gpioMap_t * terminal = datos;

    if (activa) {
        gpioWrite(*terminal, ON);
    } else {
        gpioWrite(*terminal, OFF);
    }
}

void main(void) {
    gpioMap_t leds[] = {
        LEDB, LED1, LED2, LED3
    };

    gpioMap_t terminales[] = {
        TEC1, TEC2, TEC3, TEC4
    };

    tecla_t teclas[4];
    int indice;

    /* Inicializar la placa */
    boardConfig();

    for(indice = 0; indice < 4; indice++) {
        teclas[indice] = tecla_configurar(terminales[indice], evento_tecla, &leds[indice]);
    }

    while(1) {
        for(indice = 0; indice < 4; indice++) {
            tecla_actualizar(teclas[indice]);
        }
    }
}
/* === Ciere de documentacion ================================================================== */

/** @} Final de la definición del modulo para doxygen */

