/**************************************************************************************************
 ** (c) Copyright 2019: Esteban VOLENTINI <evolentini@gmail.com>
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

#ifndef PUERTA_H   /*! @cond    */
#define PUERTA_H   /*! @endcond */

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

/* === Inclusiones de archivos externos ======================================================== */
#include "gpio.h"

/* === Cabecera C++ ============================================================================ */
#ifdef __cplusplus
extern "C" {
#endif

/* === Definicion y Macros ===================================================================== */

/* === Declaraciones de tipos de datos ========================================================= */

//! Tipo de datos para la definicion de un tiempo de espera
typedef uint16_t tiempo_t;

//! Estructura con la descripcion y configuracion de la puerta
typedef struct {    
    struct {
        gpio_t directa;         //!< Salida de cerradura en polaridad directa (PNK-ES003)
        gpio_t inversa;         //!< Salida de cerradura en polaridad inversa (PNK-ES003)
        gpio_t alarma;          //!< Terminal para salida de alarma (PNK-ES007)
    } salidas;                  //!< Estructura con los terminales de salida de la puerta    
    struct {
        gpio_t abierta;         //! Terminal del sensor de puerta abierta (PNK-ES002)
        gpio_t liberada;        //! Terminal del sensor de mecanismo liberado (PNK-ES005)
        gpio_t bloqueada;       //! Terminal del sensor de mecanismo bloqueado (PNK-ES005)
    } entradas;                 //! Estructura con los terminales de entrada de la puerta
    struct {
        tiempo_t liberacion;    //!< Tiempo para el motor libere el mecanismo (PNK-PO005)
        tiempo_t apertura;      //!< Tiempo para que el usuario abra la puerta (PNK-PO004)
        tiempo_t cierre;        //!< Tiempo para que el usuario cierre la puerta (PNK-PO006)
    } tiempos;                  //! Estructura con los tiempos de funcionamiento de la puerta    
    struct {        
        bool sensor;            //!< El sensor de puerta esta conectado (PNK-PO007)        
        bool inversor;          //!< El actuador requiere inversion de polaridad (PNK-PO008)        
        bool mecanismo;         //!< Los sensores del mecanismos estan conectados (PNK-PO009)
    } opciones;                 //!< Estructura con las opciones de funcionamiento de la puerta 
} puerta_configuracion_t;

//! Estados de de una puerta
typedef enum {
    PUERTA_CERRADA,             //!< La puerta esta cerrada y la cerradura bloqueada
    PUERTA_LIBERANDO,           //!< Se activo la salida y la cerradura se esta liberando
    PUERTA_LIBERADA,            //!< La puerta esta cerrada y la cerradura liberada
    PUERTA_BLOQUEANDO,          //!< Se activo la salida y la cerradura se esta bloqueando
    PUERTA_ESPERA,              //!< Se espera que el usuario cierre la puerta
    PUERTA_ABIERTA,             //!< La puerta permanece abierta mas tiempo de esperado
    PUERTA_FORZADA,             //!< La puerta fué abierta sin liberar la cerradura
} puerta_estado_t;

//! Descriptor del estado de una puerta
typedef struct puerta_s * puerta_t;

//! Evento para notificar un cambio de estado en la puerta
typedef void (*puerta_evento_t)(puerta_t puerta, puerta_estado_t estado);

/* === Declaraciones de variables externas ===================================================== */

/* === Declaraciones de funciones externas ===================================================== */

/** @brief  Configura la supervision de una puerta
 * 
 * @param[in]   configuracion    Estructura con la descripcion y configuracion de la puerta
 * 
 * @return      Descriptor del estado de puerta
 */
puerta_t PuertaConfigurar(const puerta_configuracion_t * configuracion, puerta_evento_t evento);

/** @brief Actualiza el estado de una puerta
 * 
 * @param[in]   puerta       Descriptor devuelto por la funcion al configurar la puerta
 */
puerta_estado_t PuertaActualizar(puerta_t puerta);

/** @brief Libera la cerradura para que el usuario pueda abrir la puerta
 * 
 * @param[in]   puerta       Descriptor devuelto por la funcion al configurar la puerta
 */
void PuertaLiberar(puerta_t puerta);

/* === Ciere de documentacion ================================================================== */
#ifdef __cplusplus
}
#endif

/** @} Final de la definición del modulo para doxygen */

#endif   /* TECLADO_H */
