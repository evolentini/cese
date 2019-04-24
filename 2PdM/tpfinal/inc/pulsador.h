/**************************************************************************************************
 ** (c) Copyright 2019: Esteban VOLENTINI <evolentini@gmail.com>
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

#ifndef PULSADOR_H   /*! @cond    */
#define PULSADOR_H   /*! @endcond */

/** @file pulsador.h
 ** @brief Declaraciones de la tarea para el control del pulsador de ingreso
 **
 **| REV | YYYY.MM.DD | Autor           | Descripción de los cambios                              |
 **|-----|------------|-----------------|---------------------------------------------------------|
 **|   1 | 2019.04.23 | evolentini      | Version inicial del archivo                             |
 **
 ** @defgroup PdM Programacion de Microcontroladores
 ** @brief Trabajo Practico para Programación de Microcontroladores 
 ** @{ */

/* === Inclusiones de archivos externos ======================================================== */

/* === Cabecera C++ ============================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Definicion y Macros ===================================================================== */

/* === Declaraciones de tipos de datos ========================================================= */

/* === Declaraciones de variables externas ===================================================== */

/* === Declaraciones de funciones externas ===================================================== */

/** @brief  Configura la tarea para la supervision del pulsador de ingreso
 * 
 * @param[in]   datos       Puntero con el descriptor de la puerta a liberar
 * 
 * @return      Puntero a al descriptor del pulsador de ingreso
 */
void * PulsadorConfigurar(void * datos);

/** @brief  Funcion con la tarea para la supervision del pulsador de ingreso
 * 
 * @param[in]   datos       Puntero al decriptor devuelto en la inicializacion de la tarea
 */
void PulsadorActualizar(void * datos);

/* === Ciere de documentacion ================================================================== */
#ifdef __cplusplus
}
#endif

/** @} Final de la definición del modulo para doxygen */

#endif   /* PULSADOR_H */
