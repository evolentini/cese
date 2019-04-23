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

/* === Definicion y Macros ===================================================================== */

/* == Declaraciones de tipos de datos internos ================================================= */

/** @brief Estrcutura que describe la información de una tecla */
struct puerta_s {
    puerta_configuracion_t configuracion;
    puerta_estado_t estado;
    puerta_evento_t evento;
    uint32_t temporizador; 
};

/* === Definiciones de variables internas ====================================================== */

/** @brief Variable con el descriptor de la puerta */
static struct puerta_s puerta = {0};

/* === Declaraciones de funciones internas ===================================================== */

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

        gpioConfig(self->configuracion.entradas.abierta, GPIO_INPUT_PULLUP);
        gpioConfig(self->configuracion.entradas.bloqueada, GPIO_INPUT_PULLUP);
        gpioConfig(self->configuracion.entradas.liberada, GPIO_INPUT_PULLUP);

        gpioConfig(self->configuracion.salidas.directa, GPIO_OUTPUT);
        gpioConfig(self->configuracion.salidas.directa, GPIO_OUTPUT);
        gpioConfig(self->configuracion.salidas.alarma, GPIO_OUTPUT);
    }
    return self;
}

puerta_estado_t PuertaActualizar(puerta_t self) {
    puerta_estado_t estado = self->estado;

    bool abierta = FALSE;
    bool liberada = FALSE;
    bool bloqueada = FALSE;

    if (self->configuracion.opciones.sensor) {
        abierta = !gpioRead(self->configuracion.entradas.abierta);
    }
    if (self->configuracion.opciones.mecanismo) {
        liberada = !gpioRead(self->configuracion.entradas.liberada);
        bloqueada = !gpioRead(self->configuracion.entradas.bloqueada);
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

