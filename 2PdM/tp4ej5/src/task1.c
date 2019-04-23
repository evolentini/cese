
/**************************************************************************************************
 **  (c) Copyright 2019: Esteban VOLENTINI
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

/** @file tarea_teclado.c
 ** @brief Programa principal del Ejercicio 5 del Trabajo Practico 4
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
#define  CANTIDAD_TECLAS (sizeof(estado_teclas)/sizeof(estado_tecla_t))

/* === Declaraciones de tipos de datos internos ================================================ */

/** @brief Estructura para mantener el estado de las teclas */
typedef struct {
    gpioMap_t terminal;     /** <= Terminal digital de la tecla                             */
    gpioMap_t led;          /** <= Terminal del led para mostrar el estado de la tecla      */
    tecla_t tecla;          /** <= Descriptor del sistema antirebote de la tecla            */
    int estado;             /** <= Estado actual de la tecla                                */
} estado_tecla_t;

/* === Definiciones de variables internas ====================================================== */
static estado_tecla_t estado_teclas [] = {
    { .terminal = TEC1, .led = LEDB },
    { .terminal = TEC2, .led = LED1 },
    { .terminal = TEC3, .led = LED2 },
    { .terminal = TEC4, .led = LED3 },
};   

/* === Declaraciones de funciones internas ===================================================== */

/** @brief Actualiza el estado de las teclas
 * 
 * Se ejecuta como una funcion de callback desde el modulo de teclado cuando se detecta una
 * transicion en una tecla.
 * 
 * @param[in]   activa  Indica si la tecla esta activa o en reposo
 * @param[in]   datos   Puntero a una estructura definida al configurar la tecla
 */
void evento_tecla(bool activa, void * datos);

/* === Definiciones de funciones internas =====================================================- */

void evento_tecla(bool activa, void * datos) {
    estado_tecla_t * accion = datos;

    if (activa) {
        accion->estado = 1;
        gpioWrite(accion->led, ON);
    } else {
        accion->estado = 0;
        gpioWrite(accion->led, OFF);
    }
}

uint8_t estado_tecla(int tecla) {
    uint8_t resultado = 0;
    int cantidad_teclas = sizeof(estado_teclas)/sizeof(estado_tecla_t);
    if ((tecla >= 0) && (tecla < CANTIDAD_TECLAS)) {
        resultado = estado_teclas[tecla].estado;
    }
    return resultado;
}
/* === Definiciones de funciones externas ====================================================== */

// Funcion que inicializa la tarea
void teclado_init( void ){
    estado_tecla_t * estado = estado_teclas;
    int indice;
    /* Configurar las teclas */
    for(indice = 0; indice < CANTIDAD_TECLAS; indice++) {
        estado->tecla = tecla_configurar(estado->terminal, evento_tecla, estado);
        estado++;
    }
}

// Funcion que se ejecuta periodicamente
void teclado_update( void * taskParam ){   
    estado_tecla_t * estado = estado_teclas;

    int indice;
    /* Actualiza el estado de las teclas con el antirebote */
    for(indice = 0; indice < CANTIDAD_TECLAS; indice++) {
        tecla_actualizar(estado->tecla);
        estado++;
    }
}

/* === Ciere de documentacion ================================================================== */

/** @} Final de la definición del modulo para doxygen */
