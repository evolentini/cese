/**************************************************************************************************
 **  (c) Copyright 2019: Esteban VOLENTINI
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

/** @file tp1ej1.c
 ** @brief Programa principal del Ejercicio 1 del Trabajo Practico 1
 **
 ** Programa que controla los led mediante las teclas, con las tecla TEC_2 la secuencia se 
 ** efectua de izquierda a derecha y con la tecla TEC_3 cambia de derecha a izquierda.
 **
 **| REV | YYYY.MM.DD | Autor           | Descripción de los cambios                              |
 **|-----|------------|-----------------|---------------------------------------------------------|
 **|   1 | 2019.03.13 | evolentini      | Version inicial del archivo                             |
 **
 ** @defgroup PdM Programacion de Microcontroladores
 ** @brief Trabajo Practico para Programación de Microcontroladores 
 ** @{ */

/* === Inclusiones de cabeceras ================================================================ */

#include "sapi.h"

/* === Definicion y Macros ===================================================================== */

/* === Definiciones de variables internas ====================================================== */

/* === Definiciones de funciones internas =====================================================- */

void cambiar_sentido(int * actual) {
   if (gpioRead(TEC2) == 0) {
      *actual = 0;
   }
   if (gpioRead(TEC3) == 0) {
      *actual = 1;
   }
}

/* === Definiciones de funciones externas ====================================================== */

void main(void) {
   // Constante con los leds de cada secuencia
   const int secuencia[2][4] = {
      {
         LEDB, LED1, LED2, LED3
      },{
         LED3, LED2, LED1, LEDB
      }
   };

   // Variable con el elemento actual de la secuencia
   int indice = 0;
   // Variable con la secuencia actual
   int actual = 0;

   // Inicializacion del hardware de la placa
   boardConfig();

   while (1) {
      /* Prendo y apago el led correspondiente a la secuencia */
      gpioWrite(secuencia[actual][indice], ON);
      delay(500);
      gpioWrite(secuencia[actual][indice], OFF);

      /* Reviso un cambio de sentido, espero apagado y vuelvo a revisar */
      cambiar_sentido(&actual);
      delay(500);
      cambiar_sentido(&actual);

      /* Incremento al siguiente paso de la secuencia */
      indice = (indice + 1) & 0x03;
   }
}
/* === Ciere de documentacion ================================================================== */

/** @} Final de la definición del modulo para doxygen */
