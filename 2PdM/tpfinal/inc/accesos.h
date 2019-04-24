/**************************************************************************************************
 ** (c) Copyright 2019: Esteban VOLENTINI <evolentini@gmail.com>
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

#ifndef ACCESOS_H   /*! @cond    */
#define ACCESOS_H   /*! @endcond */

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

/* === Inclusiones de archivos externos ======================================================== */

/* === Cabecera C++ ============================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Definicion y Macros ===================================================================== */

/* === Declaraciones de tipos de datos ========================================================= */

/* === Declaraciones de variables externas ===================================================== */

/* === Declaraciones de funciones externas ===================================================== */

/** @brief  Configura la tarea para la supervision del sistema del control de accesos
 * 
 * @param[in]   datos       Puntero con datos, no utilizado
 * 
 * @return      Puntero a al descriptor de la puerta del control de accesos
 */
void * AccesosConfigurar(void * datos);

/** @brief  Funcion con la tarea para la supervision del sistema del control de accesos
 * 
 * @param[in]   datos       Puntero al decriptor devuelto en la inicializacion de la tarea
 */
void AccesosActualizar(void * datos);

/* === Ciere de documentacion ================================================================== */
#ifdef __cplusplus
}
#endif

/** @} Final de la definición del modulo para doxygen */

#endif   /* ACCESOS_H */
