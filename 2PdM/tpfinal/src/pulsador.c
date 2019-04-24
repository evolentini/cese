/**************************************************************************************************
 ** (c) Copyright 2019: Esteban VOLENTINI <evolentini@gmail.com>
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

/** @file pulsador.h
 ** @brief Declaraciones de la tarea para el control del pulsador de ingreso
 **
 **| REV | YYYY.MM.DD | Autor           | Descripción de los cambios                              |
 **|-----|------------|-----------------|---------------------------------------------------------|
 **|   1 | 2019.04.23 | evolentini      | Version inicial del archivo                             |
 **
 ** @defgroup PdM Programacion de Microcontroladores
 ** @brief Trabajo Practico para Programación de Microcontroladores 
 ** @{ */


/* === Inclusiones de cabeceras ================================================================ */
#include "pulsador.h"
#include "digitales.h"
#include "puerta.h"

/* === Definicion y Macros ===================================================================== */

/* === Declaraciones de tipos de datos internos =================================================*/

/* === Definiciones de variables internas ====================================================== */

/* === Declaraciones de funciones internas ===================================================== */

/** @brief Funcion para gestionar los eventos del pulsador de ingreso
 * 
 * @param[in]   estado      Estado actual del pulsador
 * @param[in]   datos       Puntero con la puerta a liberar
 */
void PulsadorEvento(puerta_t puerta, puerta_estado_t estado);

/* === Definiciones de funciones internas ====================================================== */

void PulsadorEvento(bool estado, void * datos) {
    puerta_t puerta = datos;

    if (estado) {
        PuertaLiberar(puerta);
    }
}

/* === Definiciones de funciones externas ====================================================== */

void * PulsadorConfigurar(void * datos) {
    const gpio_t terminal = TEC1;
    gpioConfig(terminal, GPIO_INPUT_PULLUP);
    return (void *) EntradaConfigurar(terminal, PulsadorEvento, datos);
}

void PulsadorActualizar(void * datos) {
    entrada_t pulsador = datos;
    EntradaActualizar(pulsador);
}
/* === Ciere de documentacion ================================================================== */

/** @} Final de la definición del modulo para doxygen */

