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
 **|   2 | 2019.03.27 | evolentini      | Versiones no bloqueantes de parpadeos y secuencias      |
 **
 ** @defgroup PdM Programacion de Microcontroladores
 ** @brief Trabajo Practico para Programación de Microcontroladores 
 ** @{ */

/* === Inclusiones de cabeceras ================================================================ */

#include <assert.h>
#include <string.h>

#include "libreria.h"

/* === Definicion y Macros ===================================================================== */

/** @brief Cantidad máxima de parpadeos simultaneos */
#define PARPADEOS_SIMULTANEOS       4

/** @brief Longitud máxima del vector de secuencias */
#define SECUENCIA_LONGITUD_MAXIMA   10

/* == Declaraciones de tipos de datos internos ================================================= */

/** @brief Descriptor de un parpadeo */
struct led_parpadeo_s {
    bool estado;                /** <= Estado actual del led                    */
    gpio_t led;                 /** <= Descriptor del led a parpadar            */
    uint16_t periodo;           /** <= Periodo del parpadeo                     */
    tick_t inicio;              /** <= Valor del contador de tick               */
    uint8_t restantes;          /** <= Cantidad de parpadeos pendientes         */
}; 

/** @brief Vector para almacenar los leds que componen luna secuencia */
typedef secuencia_t secuencia[SECUENCIA_LONGITUD_MAXIMA];
/** @brief Descriptor de una secuencia */
struct led_secuencia_s {
    secuencia_t secuencia;      /** <= Vector con los leds a encender           */
    uint16_t periodo;           /** <= Periodo del parpadeo                     */
    tick_t inicio;              /** <= Valor del contador de tick               */
    uint8_t actual;             /** <= Elemento actualmente encendido           */
    uint8_t n_leds;             /** <= Cantidad de elementos en la secuencia    */
}; 

/* === Definiciones de variables internas ====================================================== */

/** @brief Vector con los descriptores de parpadeos */
static struct led_parpadeo_s parpadeos[PARPADEOS_SIMULTANEOS] = {0};

/** @brief Variable con el descriptor de secuencia */
static struct led_secuencia_s secuencias = {0};

/* === Declaraciones de funciones internas ===================================================== */

/** @brief Busca un nuevo descriptor de parpadeo sin usar
 *
 * @return      Descriptor del estado del parpadeo
 */
led_parpadeo_t nuevo_parpadeo(void);

/** @brief Determina si un parpadeo esta activo
 * 
 * @param[in]   led         Descriptor devuelto por la funcion al iniciar la secuencia
 */
bool parpadeo_activo(led_parpadeo_t self);

/** @brief Determina si se cumplio el periodo de un parpadeo
 * 
 * @param[in]   led         Descriptor devuelto por la funcion al iniciar la secuencia
 */
bool parpadeo_expirado(led_parpadeo_t self);

/** @brief Termina un parpadeo
 * 
 * @param[in]   led         Descriptor devuelto por la funcion al iniciar la secuencia
 */
void terminar_parpadeo(led_parpadeo_t self);

/** @brief Decrementa en uno la cantidad de parpadeos pendientes
 * 
 * @param[in]   led         Descriptor devuelto por la funcion al iniciar la secuencia
 */
void contar_parpadeo(led_parpadeo_t self);

/* --------------------------------------------------------------------------------------------- */

/** @brief Busca un nuevo descriptor de secuencia sin usar
 *
 * @return      Descriptor del estado de la secuencia
 */
led_secuencia_t nueva_secuencia();

/** @brief Determina si una secuencia esta activa
 * 
 * @param[in]   led         Descriptor devuelto por la funcion al iniciar la secuencia
 */
bool secuencia_activa(led_secuencia_t self);

/** @brief Determina si se cumplio el periodo de una secuencia
 * 
 * @param[in]   led         Descriptor devuelto por la funcion al iniciar la secuencia
 */
bool secuencia_expirada(led_secuencia_t self);

/** @brief Avanza en forma circular al proximo elemento de la secuencia
 * 
 * @param[in]   led         Descriptor devuelto por la funcion al iniciar la secuencia
 */
void secuencia_siguiente(led_secuencia_t self);

/* === Definiciones de funciones internas ====================================================== */
led_parpadeo_t nuevo_parpadeo(void) {
    led_parpadeo_t self = NULL;
    int indice;
    
    for(indice = 0; indice < PARPADEOS_SIMULTANEOS; indice++) {
        self = &parpadeos[indice];
        if (parpadeo_activo(self) == FALSE) {
            break;
        }
        self = NULL;
    }
    return self;
}

bool parpadeo_activo(led_parpadeo_t self) {
    return (self->periodo != 0);
}

bool parpadeo_expirado(led_parpadeo_t self) {
    return ((tickRead() - self->inicio) > self->periodo);
}

void terminar_parpadeo(led_parpadeo_t self) {
    self->periodo = 0;
}

void contar_parpadeo(led_parpadeo_t self) {
    if (self->restantes != 0) {
        self->restantes--;
        if (self->restantes == 0) {
            terminar_parpadeo(self);
        }
    }
}

/* --------------------------------------------------------------------------------------------- */

led_secuencia_t nueva_secuencia() {
    return &secuencias;
}
bool secuencia_activa(led_secuencia_t self) {
    return (self->periodo != 0);
}

bool secuencia_expirada(led_secuencia_t self) {
    return ((tickRead() - self->inicio) > self->periodo);
}

void secuencia_siguiente(led_secuencia_t self) {
    self->actual = (self->actual + 1) % self->n_leds;
}

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

/* --------------------------------------------------------------------------------------------- */

led_parpadeo_t LED_iniciar_parpadeo(gpio_t led, uint16_t periodo, uint8_t n_veces) {
    led_parpadeo_t self = nuevo_parpadeo();

    if (self != NULL) {
        self->inicio = 0;
        self->estado = FALSE;
        self->led = led;
        self->periodo = periodo;
        self->restantes = n_veces;
    }

    return self;
}

void LED_mantener_parpadeo(led_parpadeo_t self) {
    assert(self != NULL);

    if (parpadeo_activo(self) && parpadeo_expirado(self)) {
        self->estado = !(self->estado);
        gpioWrite(self->led, self->estado);
    
        if (self->estado == FALSE) {
            contar_parpadeo(self);
        }
    
        if (parpadeo_activo(self)) {
            self->inicio = tickRead(); 
        } else {
            self->inicio = 0;
        }
    }
}

/* --------------------------------------------------------------------------------------------- */

led_secuencia_t LED_iniciar_secuencia(uint16_t periodo, const gpio_t * secuencia, uint8_t n_leds) {
    led_secuencia_t self = nueva_secuencia();

    if (self != NULL) {
        self->inicio = n_leds;
        self->actual = 0;
        self->n_leds = n_leds;
        self->periodo = periodo;
        memcpy(self->secuencia, secuencia, sizeof(gpio_t) * n_leds);
    }

    return self;
}

void LED_mantener_secuencia(led_secuencia_t self) {
    assert(self != NULL);
    if (secuencia_activa(self) && secuencia_expirada(self)) {
        gpioWrite(self->secuencia[self->actual], OFF);
        secuencia_siguiente(self);
        gpioWrite(self->secuencia[self->actual], ON);
        self->inicio = tickRead(); 
    }
}

/* === Ciere de documentacion ================================================================== */

/** @} Final de la definición del modulo para doxygen */

