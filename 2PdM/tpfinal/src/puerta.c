/**************************************************************************************************
 ** (c) Copyright 2019: Esteban VOLENTINI <evolentini@gmail.com>
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

/** @file puerta.h
 ** @brief Declaraciones de la libreria para gestion de la puerta
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
#include "puerta.h"
#include "digitales.h"

/* === Definicion y Macros ===================================================================== */

/* == Declaraciones de tipos de datos internos ================================================= */

//! Estrcutura que describe la información de una puerta
struct puerta_s {
    puerta_configuracion_t configuracion;   //!< Parametros de configuracion de la puerta
    puerta_estado_t estado;                 //!< Estado actual de la puerta
    puerta_evento_t evento;                 //!< Funcion para informar un evento de puerta
    entrada_t abierta;                      //!< Entrada digital del sensor de puerta abierta
    entrada_t bloqueada;                    //!< Entrada digital del sensor de mecanismo bloqueado
    entrada_t liberada;                     //!< Entrada digital del sensor de mecanismo liberado
    uint32_t temporizador;                  //!< Valor del temporizador cuando expire la espera
};

/* === Definiciones de variables internas ====================================================== */

//! Variable con el descriptor de la puerta
static struct puerta_s puerta = {0};

/* === Declaraciones de funciones internas ===================================================== */

/** @brief Busca un nuevo descriptor de entrada sin usar
 *
 * @return      Descriptor del estado de la entrada
 */
puerta_t PuertaNueva(void);

/* === Definiciones de funciones internas ====================================================== */
puerta_t PuertaNueva(void) {
    return &puerta;
}

/* === Definiciones de funciones externas ====================================================== */
puerta_t PuertaConfigurar(const puerta_configuracion_t * configuracion, puerta_evento_t evento) {
    puerta_t self = PuertaNueva();

    if (self != NULL) {
        memcpy(&self->configuracion, configuracion, sizeof(puerta_configuracion_t));
        self->estado = PUERTA_CERRADA;
        self->evento = evento;
        self->temporizador = 0; 

        gpioConfig(self->configuracion.salidas.directa, GPIO_OUTPUT);
        gpioConfig(self->configuracion.salidas.directa, GPIO_OUTPUT);
        gpioConfig(self->configuracion.salidas.alarma, GPIO_OUTPUT);

        if (self->configuracion.opciones.sensor) {
            gpioConfig(self->configuracion.entradas.abierta, GPIO_INPUT_PULLUP);
            self->abierta = EntradaConfigurar(self->configuracion.entradas.abierta, NULL, NULL);
        }
        if (self->configuracion.opciones.mecanismo) {
            gpioConfig(self->configuracion.entradas.bloqueada, GPIO_INPUT_PULLUP);
            gpioConfig(self->configuracion.entradas.liberada, GPIO_INPUT_PULLUP);
            self->bloqueada = EntradaConfigurar(self->configuracion.entradas.bloqueada, NULL, NULL);
            self->liberada = EntradaConfigurar(self->configuracion.entradas.liberada, NULL, NULL);
        }
    }
    return self;
}

puerta_estado_t PuertaActualizar(puerta_t self) {
    puerta_estado_t estado = self->estado;

    bool abierta = FALSE;
    bool liberada = FALSE;
    bool bloqueada = FALSE;

    if (self->configuracion.opciones.sensor) {
        abierta = EntradaActualizar(self->abierta);
    }
    if (self->configuracion.opciones.mecanismo) {
        liberada = EntradaActualizar(self->liberada);
        bloqueada = EntradaActualizar(self->bloqueada);
    }

    switch (self->estado) {
        case PUERTA_CERRADA:
            if (abierta) {
                gpioWrite(self->configuracion.salidas.alarma, TRUE);
                estado = PUERTA_FORZADA;
            }
            break;

        case PUERTA_LIBERANDO:
            if ((liberada) || (tickRead() > self->temporizador)) {
                gpioWrite(self->configuracion.salidas.directa, FALSE);
                self->temporizador = tickRead() + self->configuracion.tiempos.apertura;
                estado = PUERTA_LIBERADA;
            }
            break;

        case PUERTA_LIBERADA:
            if (self->configuracion.opciones.inversor) {
                if ((abierta) || (tickRead() > self->temporizador)) {
                    self->temporizador = tickRead() + self->configuracion.tiempos.liberacion;
                    estado = PUERTA_BLOQUEANDO;
                    gpioWrite(self->configuracion.salidas.inversa, TRUE);
                }
            } else {
                if (abierta) {
                    gpioWrite(self->configuracion.salidas.directa, FALSE);
                    self->temporizador = tickRead() + self->configuracion.tiempos.cierre;
                    estado = PUERTA_ESPERA;
                } else if (tickRead() > self->temporizador) {
                    gpioWrite(self->configuracion.salidas.directa, FALSE);
                    estado = PUERTA_CERRADA;
                }
            }
            break;

        case PUERTA_BLOQUEANDO:
            if ((bloqueada) || (tickRead() > self->temporizador)) {
                gpioWrite(self->configuracion.salidas.inversa, FALSE);
                if (abierta) {
                    self->temporizador = tickRead() + self->configuracion.tiempos.cierre;
                    estado = PUERTA_ESPERA;
                } else {
                    estado = PUERTA_CERRADA;
                }
            }
            break;

        case PUERTA_ESPERA:
            if (!abierta) {
                estado = PUERTA_CERRADA;
            } else  if (tickRead() > self->temporizador) {
                gpioWrite(self->configuracion.salidas.alarma, TRUE);
                estado = PUERTA_ABIERTA;
            }
            break;

        case PUERTA_ABIERTA:
            if (!abierta) {
                gpioWrite(self->configuracion.salidas.alarma, FALSE);
                estado = PUERTA_CERRADA;
            }
            break;

        case PUERTA_FORZADA:
            if (!abierta) {
                gpioWrite(self->configuracion.salidas.alarma, FALSE);
                estado = PUERTA_CERRADA;
            }
            break;

        default:
            break;
    }
    if (estado != self->estado) {
        self->estado = estado;
        if (self->evento) self->evento(self, self->estado);
    }
    return self->estado;
}

void PuertaLiberar(puerta_t self) {
    if (self->estado == PUERTA_CERRADA) {
        if (self->configuracion.opciones.inversor) {
            self->temporizador = tickRead() + self->configuracion.tiempos.liberacion;
            self->estado = PUERTA_LIBERANDO;
        } else {
            self->temporizador = tickRead() + self->configuracion.tiempos.apertura;
            self->estado = PUERTA_LIBERADA;
        }
        gpioWrite(self->configuracion.salidas.directa, TRUE);
        if (self->evento) self->evento(self, self->estado);
    }
}

/* === Ciere de documentacion ================================================================== */

/** @} Final de la definición del modulo para doxygen */

