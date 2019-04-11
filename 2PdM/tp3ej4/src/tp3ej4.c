
/**************************************************************************************************
 **  (c) Copyright 2019: Esteban VOLENTINI
 ** ALL RIGHTS RESERVED, DON'T USE OR PUBLISH THIS FILE WITHOUT AUTORIZATION
 *************************************************************************************************/

/** @file tp3ej4.c
 ** @brief Programa principal del Ejercicio 4 del Trabajo Practico 3
 **
 ** Programa principal para probar el modulo que gestiona las cuatro teclas mediante una MEF,
 ** el modulo de manejo una consola serial mediante la UART y el modulo de gestion del conversor
 ** analogico digital.
 **
 **| REV | YYYY.MM.DD | Autor           | Descripción de los cambios                              |
 **|-----|------------|-----------------|---------------------------------------------------------|
 **|   1 | 2019.04.03 | evolentini      | Version inicial del archivo                             |
 **
 ** @defgroup PdM Programacion de Microcontroladores
 ** @brief Trabajo Practico para Programación de Microcontroladores 
 ** @{ */

/* === Inclusiones de cabeceras ================================================================ */

#include "sapi.h"
#include "teclado.h"
#include "terminal.h"

/* === Definicion y Macros ===================================================================== */
#define CANTIDAD_TECLAS     ()

/* == Declaraciones de tipos de datos internos ================================================= */

/** @brief Estructura para mantener el estado de las teclas */
typedef struct {
    gpioMap_t terminal;     /** <= Terminal digital de la tecla                             */
    gpioMap_t led;          /** <= Terminal del led para mostrar el estado de la tecla      */
    tecla_t tecla;          /** <= Descriptor del sistema antirebote de la tecla            */
    int estado;             /** <= Estado actual de la tecla                                */
} estado_tecla_t;

/* === Definiciones de variables internas ====================================================== */

/* === Declaraciones de funciones internas ===================================================== */

/** @brief Actualiza el estado de las teclas
 * 
 * Se ejecuta como una funcion de callback desde el modulo de teclado cuando se detecta una
 * transicion en una tecla.
 * 
 * @param[in]   activa  Indica si la tecla esta activa o en reposo
 * @param[in]   datos   Puntero a una estructura definida al configurar la tecla
 */
void evento_tecla(bool activa, void * datos);

/** @brief Visualizacion de los GPIO por la consola serial
 * 
 * @param[in]   teclas      Vector con el estado del antirrebote de las teclas
 * @param[in]   estados     Vector con el estado actual (activa/reposo) de las teclas
 * @param[in]   cantidad    Cantidad de elementos en el vector de estado
 */
void modo_gpio(estado_tecla_t * estados, int cantidad);

/* === Definiciones de funciones internas =====================================================- */

void evento_tecla(bool activa, void * datos) {
    estado_tecla_t * accion = datos;

    if (activa) {
        accion->estado = 1;
        gpioWrite(accion->led, ON);
    } else {
        accion->estado = 0;
        gpioWrite(accion->led, OFF);
    }
}

void modo_gpio(estado_tecla_t * estados, int cantidad) {
    const char * ESTADOS[] = {
        TERMINAL_TEXTO_VERDE "REPOSO" TERMINAL_TEXTO_BLANCO,
        TERMINAL_TEXTO_ROJO "ACTIVA" TERMINAL_TEXTO_BLANCO,
    };
    const char * MENSAJES[] = {
        "Ingresando al modo GPIO (Presione 9 para salir)" 
        TERMINAL_NUEVA_LINEA TERMINAL_NUEVA_LINEA TERMINAL_NUEVA_LINEA,
        "%sTECLA %d: %s",
        "%s - ",
    };

    char opcion;
    int indice;
    uint32_t espera;
    char mensaje[128];

    /* Muestra el mensaje de bienvenida */ 
    terminal_escribir(MENSAJES[0]);

    espera = 0;
    do {
        /* Actualiza el estado de las teclas con el antirebote */
        for(indice = 0; indice < cantidad; indice++) {
            tecla_actualizar(estados[indice].tecla);
        }
        
        /* Refresca el estado de las teclas en pantalla perioricamente */
        if ((tickRead() - espera) > 200) {
            mensaje[0] = 0;
            /* Construye la cadena con el estado de cada tecla */
            for(indice = 0; indice < cantidad; indice++) {
                sprintf(mensaje, MENSAJES[1], mensaje, indice + 1, ESTADOS[estados[indice].estado]);
                if (indice < cantidad - 1) {
                    sprintf(mensaje, MENSAJES[2], mensaje);
                }
            }
            /* Actuliza en la pantalla el estado de las teclas */
            terminal_refrescar(mensaje);
            espera = tickRead();
        }

        /* Recupera la opcion ingresada por el usuario en la terminal serial */
        opcion = terminal_opcion("9");
    } while (opcion != '9');
}

void modo_adc(void) {
    const char * MENSAJES[] = {
        "Ingresando al modo ADC (Presione 9 para salir)"
        TERMINAL_NUEVA_LINEA TERMINAL_NUEVA_LINEA TERMINAL_NUEVA_LINEA,
        "Valor Actual ADC: " TERMINAL_TEXTO_VERDE "%04d" TERMINAL_TEXTO_BLANCO,
    };

    char opcion;
    uint16_t valor;
    uint32_t espera;
    char mensaje[64];

    /* Muestra el mensaje de bienvenida */ 
    terminal_escribir(MENSAJES[0]);

    espera = 0;
    adcConfig(ADC_ENABLE);
    do {
        /* Refresca el estado de las teclas en pantalla perioricamente */
        if ((tickRead() - espera) > 200) {
            valor = adcRead(CH1);
            sprintf(mensaje, MENSAJES[1], valor);

            terminal_refrescar(mensaje);
            espera = tickRead();
        }
        opcion = terminal_opcion("9");
    } while (opcion != '9');
    adcConfig(ADC_DISABLE);
}

/* === Definiciones de funciones externas ====================================================== */

void main(void) {
    estado_tecla_t estado_teclas [] = {
        { .terminal = TEC1, .led = LEDB },
        { .terminal = TEC2, .led = LED1 },
        { .terminal = TEC3, .led = LED2 },
        { .terminal = TEC4, .led = LED3 },
    };
    
    estado_tecla_t * estado = estado_teclas;
    int cantidad_teclas = sizeof(estado_teclas)/sizeof(estado_tecla_t);
    int indice;
    char opcion;

    /* Configurar la placa */
    boardConfig();

    /* Configurar las teclas */
    for(indice = 0; indice < cantidad_teclas; indice++) {
        estado->tecla = tecla_configurar(estado->terminal, evento_tecla, estado);
        estado++;
    }
    /* Configurar el terminal serial */
    terminal_configurar();

    while(1) {
        terminal_menu();
        do {
            opcion = terminal_opcion("12");
        } while (opcion == 0);
        
        if (opcion == '1') {
            modo_gpio(estado_teclas, cantidad_teclas);
        } else {
            modo_adc();
        }
    }
}
/* === Ciere de documentacion ================================================================== */

/** @} Final de la definición del modulo para doxygen */

