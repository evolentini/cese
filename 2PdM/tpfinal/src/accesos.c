/**************************************************************************************************
 ** (c) Copyright 2019: Esteban VOLENTINI <evolentini@gmail.com>
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

/** @file accesos.h
 ** @brief Declaraciones de la tarea para el control de accesos
 **
 **| REV | YYYY.MM.DD | Autor           | Descripción de los cambios                              |
 **|-----|------------|-----------------|---------------------------------------------------------|
 **|   1 | 2019.04.23 | evolentini      | Version inicial del archivo                             |
 **
 ** @defgroup PdM Programacion de Microcontroladores
 ** @brief Trabajo Practico para Programación de Microcontroladores 
 ** @{ */


/* === Inclusiones de cabeceras ================================================================ */
#include "accesos.h"
#include "puerta.h"

/* === Definicion y Macros ===================================================================== */

/* === Declaraciones de tipos de datos internos =================================================*/

/* === Definiciones de variables internas ====================================================== */

//! Configuracion de la puerta para el control de accesos
const puerta_configuracion_t configuracion = {
   .salidas = {
      .directa = LEDG,
      .inversa = LEDR,
      .alarma = LED2,
   },
   .entradas = {
      .abierta = TEC2,
      .liberada = TEC3,
      .bloqueada = TEC4,
   },
   .tiempos = {
      .liberacion = 1000,
      .apertura = 2000,
      .cierre = 5000,
   },
   .opciones = {
      .sensor = TRUE,
      .inversor = TRUE,
      .mecanismo = TRUE,
   }
};

/* === Declaraciones de funciones internas ===================================================== */

/** @brief Funcion para gestionar los eventos de la puerta
 * 
 * @param[in]   puerta      Descriptor devuelto por la funcion al configurar la puerta
 * @param[in]   estado      Estado actual de la puerta
 */
void PuertaEvento(puerta_t puerta, puerta_estado_t estado);

/* === Definiciones de funciones internas ====================================================== */

void PuertaEvento(puerta_t puerta, puerta_estado_t estado) {
   // Constante con las cadenas de caracteres correspondientes para cada estado
   const char * ESTADOS[] = {
      "PUERTA CERRADA\r\n",
      "PUERTA LIBERANDO\r\n",
      "PUERTA LIBERADA\r\n",
      "PUERTA BLOQUEANDO\r\n",
      "PUERTA ESPERA\r\n",
      "PUERTA ABIERTA\r\n",
      "PUERTA FORZADA\r\n",
   };

   // Escritura en la terminal del estado actual
   uartWriteString(UART_USB, ESTADOS[estado]);
}

/* === Definiciones de funciones externas ====================================================== */
void * AccesosConfigurar(void * datos) {
    uartConfig(UART_USB, 115200);
    return (void *) PuertaConfigurar(&configuracion, PuertaEvento);
}

void AccesosActualizar(void * datos) {
    puerta_t puerta = datos;
    PuertaActualizar(puerta);
}
/* === Ciere de documentacion ================================================================== */

/** @} Final de la definición del modulo para doxygen */

