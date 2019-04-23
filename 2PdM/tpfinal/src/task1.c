/*============================================================================
 * License: BSD-3-Clause
 * Copyright 2018, Eric Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * Date: 2018/10/04
 *===========================================================================*/

/*==================[inclusions]=============================================*/
#include "puerta.h"
#include "sapi.h"
#include "task1.h"  // <= own header

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

puerta_configuracion_t config = {
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
      .liberacion = 500,
      .apertura = 2000,
      .cierre = 5000,
   },
   .opciones = {
      .sensor = TRUE,
      .inversor = TRUE,
      .mecanismo = FALSE,
   }
};

puerta_t puerta;

void log_estado(puerta_t puerta, puerta_estado_t estado) {
   const char * ESTADOS[] = {
      "PUERTA CERRADA\r\n",
      "PUERTA LIBERANDO\r\n",
      "PUERTA LIBERADA\r\n",
      "PUERTA BLOQUEANDO\r\n",
      "PUERTA ESPERA\r\n",
      "PUERTA ABIERTA\r\n",
      "PUERTA FORZADA\r\n",
   };

   uartWriteString(UART_USB, ESTADOS[estado]);
}

// Funcion que inicializa la tarea
void task1_Init( int32_t pin ){
   gpioConfig(TEC1, GPIO_INPUT_PULLUP);
   puerta = PuertaConfigurar(&config, log_estado);
   uartConfig(UART_USB, 115200);

}

// Funcion que se ejecuta periodicamente
void task1_Update( void* taskParam ){

   if (!gpioRead(TEC1)) {
      PuertaLiberar(puerta);
   }
   PuertaActualizar(puerta);
}

/*==================[end of file]============================================*/
