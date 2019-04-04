/**************************************************************************************************
 ** (c) Copyright 2019: Esteban VOLENTINI <evolentini@gmail.com>
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

/** @file teclado.c
 ** @brief Declaraciones de la libreria para gestion del teclado
 **
 ** Libreria para el manejo de un teclado utilizando maquina de estados finitos
 ** para la practica de la materia de Programación de Microcontroladores, 
 ** perteneciente a la Especialización en Sistemas Embebidos.
 **
 **| REV | YYYY.MM.DD | Autor           | Descripción de los cambios                              |
 **|-----|------------|-----------------|---------------------------------------------------------|
 **|   1 | 2019.04.03 | evolentini      | Version inicial del archivo                             |
 **
 ** @defgroup PdM Programacion de Microcontroladores
 ** @brief Trabajo Practico para Programación de Microcontroladores 
 ** @{ */

/* === Inclusiones de cabeceras ================================================================ */
#include "teclado.h"

/* === Definicion y Macros ===================================================================== */

/** @brief Cantidad máxima de teclas a controlar simultaneamente    */
#define TECLAS_CANTIDAD         4

/** @brief Tiempo de espera para el filtrado del rebote             */
#define TECLAS_ESPERA           40

/* == Declaraciones de tipos de datos internos ================================================= */

/** @brief Estados de la maquina que supervisa una tecla            */
typedef enum {
    TECLA_INVALIDA = 0,     /** <= El descriptor no esta utilizado                      */ 
    TECLA_ACTIVA,           /** <= La tecla esta presionada                             */
    TECLA_LIBRE,            /** <= La tecla no esta presionada osea que esta libre      */
    TECLA_ACTIVANDOSE,      /** <= La tecla esta en transicion de libre a presionada    */
    TECLA_LIBERANDOSE,      /** <= La tecla esta en transicion de presionada a libre    */
} tecla_estado_t;

/** @brief Estrcutura que describe la información de una tecla */
struct tecla_s {
    tecla_terminal_t terminal;  /** <= Terminal al que se conecta la tecla                  */
    tecla_estado_t estado;      /** <= Estado de la maquina que controla a la tecla         */
    uint32_t temporizador;      /** <= Valor del temporizado cuando empezo la transicion    */
    tecla_evento_t evento;      /** <= Puntero a la funcion para informar un cambio         */
    void * datos;               /** <= Puntero a datos para enviar al gesto de eventos      */
};

/* === Definiciones de variables internas ====================================================== */

/** @brief Vector con los descriptores de teclas */
static struct tecla_s teclas[TECLAS_CANTIDAD] = {0};

/* === Declaraciones de funciones internas ===================================================== */

/** @brief Busca un nuevo descriptor de tecla sin usar
 *
 * @return      Descriptor del estado de la tecla
 */
static tecla_t nueva_tecla(void);

/** @brief Determina si un descriptor de tecla esta utilizado
 * 
 * @param[in]   self         Descriptor devuelto por la funcion al iniciar la tecla
 * 
 * @return      Valor logico que indica que el descriptor de tecla esta utilizado
 */
static bool tecla_usada(tecla_t self);

/** @brief Determina si se cumplio el periodo de espera del antirebote
 * 
 * @param[in]   self         Descriptor devuelto por la funcion al iniciar la tecla
 * 
 * @return      Valor logico que indica que se cumplio el tiempo de espera
 */
static bool tecla_expirada(tecla_t self);

/** @brief Determina si el terminal digital de la tecla esta activo
 * 
 * @param[in]   self         Descriptor devuelto por la funcion al iniciar la tecla
 * 
 * @return      Valor logico que indica que la tecla esta activa
 */
static bool tecla_activa(tecla_t self);

/* === Definiciones de funciones internas ====================================================== */

static tecla_t nueva_tecla(void) {
    tecla_t self = NULL;
    int indice;
    
    for(indice = 0; indice < TECLAS_CANTIDAD; indice++) {
        self = &teclas[indice];
        if (tecla_usada(self) == FALSE) {
            break;
        }
        self = NULL;
    }
    return self;
}

static bool tecla_usada(tecla_t self) {
    return (self->estado != TECLA_INVALIDA);
}

static bool tecla_expirada(tecla_t self) {
    return (tickRead() - self->temporizador > TECLAS_ESPERA);
}

static bool tecla_activa(tecla_t self) {
    return (gpioRead(self->terminal) == 0);
}

/* === Definiciones de funciones externas ====================================================== */
tecla_t tecla_configurar(tecla_terminal_t terminal, tecla_evento_t evento, void * datos) {
    tecla_t self = nueva_tecla();

    if (self != NULL) {
        self->terminal = terminal;
        self->evento = evento;
        self->datos = datos;
        self->temporizador = tickRead(); 
        if (tecla_activa(self)) {
            self->estado = TECLA_ACTIVA;
        } else {
            self->estado = TECLA_LIBRE;
        }
    }

    return self;
}

void tecla_actualizar(tecla_t self) {
    bool activa = tecla_activa(self);

    switch (self->estado) {
        case TECLA_ACTIVA:
            if (!activa) {
                self->estado = TECLA_LIBERANDOSE;
                self->temporizador = tickRead();
            }
            break;

        case TECLA_LIBRE:
            if (activa) {
                self->estado = TECLA_ACTIVANDOSE;
                self->temporizador = tickRead();
            }
            break;

        case TECLA_ACTIVANDOSE:
            if (!activa) {
                self->estado = TECLA_LIBRE;
            } else if (tecla_expirada(self)) {
                self->estado = TECLA_ACTIVA;
                self->evento(TRUE, self->datos);
            }
            break;

        case TECLA_LIBERANDOSE:
            if (activa) {
                self->estado = TECLA_ACTIVA;
            } else if (tecla_expirada(self)) {
                self->estado = TECLA_LIBRE;
                self->evento(FALSE, self->datos);
            }
            break;

        default:
            break;
    }
}

/* === Ciere de documentacion ================================================================== */

/** @} Final de la definición del modulo para doxygen */

