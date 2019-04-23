/**************************************************************************************************
 ** (c) Copyright 2019: Esteban VOLENTINI <evolentini@gmail.com>
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

#ifndef TERMINAL_H   /*! @cond    */
#define TERMINAL_H   /*! @endcond */

/** @file terminal.h
 ** @brief Declaraciones de la libreria para manejo de la terminal serial
 **
 ** Libreria para gestion de una terminal serial para la practica de la materia Programación de
 ** Microcontroladores, perteneciente a la Especialización en Sistemas Embebidos.
 **
 **| REV | YYYY.MM.DD | Autor           | Descripción de los cambios                              |
 **|-----|------------|-----------------|---------------------------------------------------------|
 **|   1 | 2019.04.04 | evolentini      | Version inicial del archivo                             |
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

/** @brief Codigos de control para generar una nueva linea */
#define TERMINAL_NUEVA_LINEA        "\r\n"

/** @brief Codigos de control para escribir el texto en rojo */
#define TERMINAL_TEXTO_ROJO         "\x1B[31m"

/** @brief Codigos de control para escribir el texto en verde */
#define TERMINAL_TEXTO_VERDE        "\x1B[32m"

/** @brief Codigos de control para escribir el texto en amarillo */
#define TERMINAL_TEXTO_AMARILLO     "\x1B[33m"

/** @brief Codigos de control para escribir el texto en blanco */
#define TERMINAL_TEXTO_BLANCO       "\x1B[37m"

/* === Declaraciones de tipos de datos ========================================================= */

/* === Declaraciones de variables externas ===================================================== */

/* === Declaraciones de funciones externas ===================================================== */

/** @brief Configura el terminal serial */
void terminal_configurar(void);

/** @brief Escribe una cadena de caracteres en la terminal serial
 * 
 * @param[in]   opiciones    Cadena de caracteres que se quiere escribir
 */
void terminal_escribir(const char * cadena);

/** @brief Sobreescribe la ultima linea de la terminal serial
 * 
 * @param[in]   opiciones    Cadena de caracteres que se quiere refrescar
 */
void terminal_refrescar(const char * cadena);

/** @brief Borra la pantalla de la terminal serial */
void terminal_borrar(void);

/** @brief Borra la pantalla y muestra el menu */
void terminal_menu(void);

/** @brief Lee una tecla de la termina y la valida 
 * 
 * @param[in]   opiciones    Cadena de caracteres con las opciones validas
 * 
 * @return      Codigo ascii de la opcion ingresada si es valida, cero si es invalida
 */
char terminal_opcion(const char * opciones);


/* === Ciere de documentacion ================================================================== */
#ifdef __cplusplus
}
#endif

/** @} Final de la definición del modulo para doxygen */

#endif   /* TERMINAL_H */
