
/**************************************************************************************************
 **  (c) Copyright 2019: Esteban VOLENTINI
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

/** @file tarea_consola.c
 ** @brief Programa principal del Ejercicio 54 del Trabajo Practico 4
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

#include <string.h>
#include "task2.h" 
#include "task1.h"
#include "terminal.h"

/* === Definicion y Macros ===================================================================== */

/* === Declaraciones de tipos de datos internos ================================================ */

/* === Definiciones de variables internas ====================================================== */

/* === Declaraciones de funciones internas ===================================================== */

/* === Definiciones de funciones internas =====================================================- */

/* === Definiciones de funciones externas ====================================================== */

void consola_init( void ){
    const char * MENSAJE = "Ingresando al modo GPIO (Presione 9 para salir)"
        TERMINAL_NUEVA_LINEA TERMINAL_NUEVA_LINEA TERMINAL_NUEVA_LINEA;

    terminal_configurar();
    terminal_borrar();
    terminal_escribir(MENSAJE);
}

void consola_update( void* taskParam ){   
    const char * ESTADOS[] = {
        TERMINAL_TEXTO_VERDE "REPOSO" TERMINAL_TEXTO_BLANCO,
        TERMINAL_TEXTO_ROJO "ACTIVA" TERMINAL_TEXTO_BLANCO,
    };
    const char * MENSAJES[] = {
        "%sTECLA %d: %s",
        "%s - ",
    };

    int indice;
    char mensaje[128];

    /* Construye la cadena con el estado de cada tecla */
    mensaje[0] = 0;
    for(indice = 0; indice < 4; indice++) {
        sprintf(mensaje, MENSAJES[0], mensaje, indice + 1, ESTADOS[estado_tecla(indice)]);
        if (indice < 3) {
            sprintf(mensaje, MENSAJES[1], mensaje);
        }
    }
    /* Muestra la cadena por pantalla sobreescribiendo la anterior */
    terminal_refrescar(mensaje);
}

/*==================[end of file]============================================*/
