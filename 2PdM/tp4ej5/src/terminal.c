/**************************************************************************************************
 ** (c) Copyright 2019: Esteban VOLENTINI <evolentini@gmail.com>
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

/** @file terminal.c
 ** @brief Declaraciones de la libreria para manejo de la terminal serial
 **
 ** Libreria para gestion de una terminal serial para la practica de la materia Programación de
 ** Microcontroladores, perteneciente a la Especialización en Sistemas Embebidos.
 **
 **| REV | YYYY.MM.DD | Autor           | Descripción de los cambios                              |
 **|-----|------------|-----------------|---------------------------------------------------------|
 **|   1 | 2019.04.04 | evolentini      | Version inicial del archivo                             |
 **
 ** @defgroup PdM Programacion de Microcontroladores
 ** @brief Trabajo Practico para Programación de Microcontroladores 
 ** @{ */

/* === Inclusiones de cabeceras ================================================================ */

#include <string.h>

#include "terminal.h"
#include "sapi.h"

/* === Definicion y Macros ===================================================================== */

/** @brief Puerto serial utilizado por la terminal */
#define TERMINAL_UART   UART_USB

/** @brief Codigo de control para borrar una linea */
#define VT100_ERASE_LINE        "\x1B[2K"

/** @brief Codigo de control para borrar la pantalla */
#define VT100_ERASE_SCREEN      "\x1B[2J"

/** @brief Codigo de control para borrar la pantalla */
#define VT100_CURSOR_ARRIBA      "\x1B[1A"

/** @brief Codigo de control para borrar la pantalla */
#define VT100_CURSOR_INICIO      "\x1B[H"

/* == Declaraciones de tipos de datos internos ================================================= */

/* === Definiciones de variables internas ====================================================== */

/* === Declaraciones de funciones internas ===================================================== */

/* === Definiciones de funciones internas ====================================================== */


/* === Definiciones de funciones externas ====================================================== */

void terminal_configurar(void) {
    uartConfig(TERMINAL_UART, 115200);
}

void terminal_escribir(const char * cadena) {
    uartWriteString(TERMINAL_UART, cadena);  
}

void terminal_refrescar(const char * cadena) {
    terminal_escribir(VT100_CURSOR_ARRIBA);
    terminal_escribir(cadena);
    terminal_escribir(TERMINAL_NUEVA_LINEA);
}

void terminal_borrar(void) {
    terminal_escribir(VT100_ERASE_SCREEN);
    terminal_escribir(VT100_CURSOR_INICIO);
}

void terminal_menu(void) {
    const char * MENSAJE[] = {
        TERMINAL_TEXTO_AMARILLO
        "Programación de Microcontroladores",
        "Programa de gestion del la UART, ADC y GPIO",
        "-------------------------------------------",
        TERMINAL_TEXTO_BLANCO
        "Presione 1 para ver el estado del GPIO",
        "Presione 2 para ver el estado del ADC",
    };
    int indice;

    terminal_borrar();
    
    for (indice = 0; indice < 5; indice++) {
        terminal_escribir(MENSAJE[indice]);
        terminal_escribir(TERMINAL_NUEVA_LINEA);
    }
    terminal_escribir(TERMINAL_NUEVA_LINEA);
    terminal_escribir("Opcion: ");
}

char terminal_opcion(const char * opciones) {
    char tecla;
    bool valida;

    if (uartReadByte(TERMINAL_UART, &tecla)) {
        valida = (strchr(opciones, tecla) > 0);
    } else {
        valida = FALSE;
    }
    if (!valida) {
        tecla = 0;
    } else {
        uartWriteByte(TERMINAL_UART, tecla);
        terminal_escribir(TERMINAL_NUEVA_LINEA);
        terminal_escribir(TERMINAL_NUEVA_LINEA);
    }
    return tecla;
}

/* === Ciere de documentacion ================================================================== */

/** @} Final de la definición del modulo para doxygen */

