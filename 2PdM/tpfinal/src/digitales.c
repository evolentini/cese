/**************************************************************************************************
 ** (c) Copyright 2019: Esteban VOLENTINI <evolentini@gmail.com>
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

/** @file digitales.c
 ** @brief Declaraciones de la libreria para gestion de entradas digitales
 **
 ** Libreria para el manejo de entradas digitales utilizando maquina de estados finitos
 ** para la eliminación de rebotes transitorios.
 **
 **| REV | YYYY.MM.DD | Autor           | Descripción de los cambios                              |
 **|-----|------------|-----------------|---------------------------------------------------------|
 **|   2 | 2019.04.23 | evolentini      | Renombrado y generalizacion para entradas digitales     |
 **|   1 | 2019.04.03 | evolentini      | Version inicial del archivo para manejo de teclado      |
 **
 ** @defgroup PdM Programacion de Microcontroladores
 ** @brief Trabajo Practico para Programación de Microcontroladores 
 ** @{ */

/* === Inclusiones de cabeceras ================================================================ */
#include "digitales.h"

/* === Definicion y Macros ===================================================================== */

//! Cantidad máxima de entradas a controlar simultaneamente 
#define ENTRADAS_CANTIDAD           8

//! @brief Tiempo de espera para el filtrado del rebote
#define ENTRADAS_ESPERA              50

/* == Declaraciones de tipos de datos internos ================================================= */

//! Estados de la maquina que supervisa una entrada digital
typedef enum {
    ENTRADA_INVALIDA = 0,       //!< El descriptor no esta utilizado
    ENTRADA_ACTIVA,             //!< La entrada esta activa
    ENTRADA_LIBRE,              //!< La entrada no esta activa osea que esta libre
    ENTRADA_ACTIVANDOSE,        //!< La entrada esta en transicion de libre a activa
    ENTRADA_LIBERANDOSE,        //!< La entrada esta en transicion de activa a libre
} entrada_estado_t;

//! Estrcutura que describe la información de una entrada digital
struct entrada_s {
    gpio_t terminal;            //!< Terminal al que se conecta la entrada
    entrada_estado_t estado;    //!<  Estado de la maquina que controla a la entrada
    uint32_t temporizador;      //!<  Valor del temporizado cuando empezo la transicion
    entrada_evento_t evento;    //!<  Puntero a la funcion para informar un cambio
    void * datos;               //!<  Puntero a datos para enviar al gesto de eventos
};

/* === Definiciones de variables internas ====================================================== */

/** @brief Vector con los descriptores de entradas */
static struct entrada_s entradas[ENTRADAS_CANTIDAD] = {0};

/* === Declaraciones de funciones internas ===================================================== */

/** @brief Busca un nuevo descriptor de entrada sin usar
 *
 * @return      Descriptor del estado de la entrada
 */
static entrada_t EntradaNueva(void);

/** @brief Determina si un descriptor de entrada esta utilizado
 * 
 * @param[in]   self         Descriptor devuelto por la funcion al configurar la entrada
 * 
 * @return      Valor logico que indica que el descriptor de entrada esta utilizado
 */
static bool EntradaUsada(entrada_t self);

/** @brief Determina si se cumplio el periodo de espera del antirebote
 * 
 * @param[in]   self         Descriptor devuelto por la funcion al configurar la entrada
 * 
 * @return      Valor logico que indica que se cumplio el tiempo de espera
 */
static bool EsperaExpirada(entrada_t self);

/** @brief Determina si el terminal digital de la entrada esta activo
 * 
 * @param[in]   self         Descriptor devuelto por la funcion al iniciar la entrada
 * 
 * @return      Valor logico que indica que la entrada esta activa
 */
static bool TerminalActivo(entrada_t self);

/* === Definiciones de funciones internas ====================================================== */

static entrada_t EntradaNueva(void) {
    entrada_t self = NULL;
    int indice;
    
    for(indice = 0; indice < ENTRADAS_CANTIDAD; indice++) {
        self = &entradas[indice];
        if (EntradaUsada(self) == FALSE) {
            break;
        }
        self = NULL;
    }
    return self;
}

static bool EntradaUsada(entrada_t self) {
    return (self->estado != ENTRADA_INVALIDA);
}

static bool EsperaExpirada(entrada_t self) {
    return (tickRead() - self->temporizador > ENTRADAS_ESPERA);
}

static bool TerminalActivo(entrada_t self) {
    return (gpioRead(self->terminal) == 0);
}

/* === Definiciones de funciones externas ====================================================== */
entrada_t EntradaConfigurar(gpio_t terminal, entrada_evento_t evento, void * datos) {
    entrada_t self = EntradaNueva();

    if (self != NULL) {
        self->terminal = terminal;
        self->evento = evento;
        self->datos = datos;
        self->temporizador = tickRead(); 
        if (TerminalActivo(self)) {
            self->estado = ENTRADA_ACTIVA;
        } else {
            self->estado = ENTRADA_LIBRE;
        }
    }

    return self;
}

bool EntradaActualizar(entrada_t self) {
    bool activa = TerminalActivo(self);
    bool resultado;

    switch (self->estado) {
        case ENTRADA_ACTIVA:
            resultado = TRUE;
            if (!activa) {
                self->estado = ENTRADA_LIBERANDOSE;
                self->temporizador = tickRead();
            }
            break;

        case ENTRADA_LIBRE:
            resultado = FALSE;
            if (activa) {
                self->estado = ENTRADA_ACTIVANDOSE;
                self->temporizador = tickRead();
            }
            break;

        case ENTRADA_ACTIVANDOSE:
            resultado = FALSE;
            if (!activa) {
                self->estado = ENTRADA_LIBRE;
            } else if (EsperaExpirada(self)) {
                resultado = TRUE;
                self->estado = ENTRADA_ACTIVA;
                if (self->evento != NULL) {
                    self->evento(TRUE, self->datos);
                }
            }
            break;

        case ENTRADA_LIBERANDOSE:
            resultado = TRUE;
            if (activa) {
                self->estado = ENTRADA_ACTIVA;
            } else if (EsperaExpirada(self)) {
                resultado = FALSE;
                self->estado = ENTRADA_LIBRE;
                if (self->evento != NULL) {
                    self->evento(FALSE, self->datos);
                }
            }
            break;

        default:
            break;
    }
    return resultado;
}

/* === Ciere de documentacion ================================================================== */

/** @} Final de la definición del modulo para doxygen */

