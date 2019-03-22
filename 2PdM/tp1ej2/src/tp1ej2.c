/**************************************************************************************************
 **  (c) Copyright 2019: Esteban VOLENTINI
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

/** @file tp1ej21.c
 ** @brief Programa principal del Ejercicio 2 del Trabajo Practico 1
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

int tecla_presionada(void) {
   int resultado = -1;

   if (gpioRead(TEC1) == 0) {
      resultado = 0;
   } else if(gpioRead(TEC2) == 0) {
      resultado = 1;
   } else if(gpioRead(TEC3) == 0) {
      resultado = 2;
   } else if(gpioRead(TEC4) == 0) {
      resultado = 3;
   }
   return resultado;
}

/* === Definiciones de funciones externas ====================================================== */

void main(void) {
   // Constante con los leds asociados a cada tecla
   const int leds[] = {
      LEDB, LED1, LED2, LED3
   };

   // Variable con el numero de la tecla presionada
   int tecla = 0;
   // Variable con la secuencia actual
   int demora = 0;

   // Inicializacion del hardware de la placa
   boardConfig();

   while (1) {
      // Espero que se presione una tecla
      do {
         tecla = tecla_presionada();
      } while (tecla < 0);

      // Espero que se libere contando el tiempo
      demora = 0;
      do {
         demora++;
         delay(1);
      } while (tecla == tecla_presionada());
   
      // Prendo y apago el led correspondiente a  la tecla
      gpioWrite(leds[tecla], ON);
      delay(demora);
      gpioWrite(leds[tecla], OFF);
   }
}
/* === Ciere de documentacion ================================================================== */

/** @} Final de la definición del modulo para doxygen */
