#ifndef INTERFAZ_H_INCLUDED
#define INTERFAZ_H_INCLUDED

#include "comun.h"
#include "config.h"
#include "historial.h"
#include "puntuacion.h"

#define TAM_ORDEN  64
#define PROMPT_OPERADOR  "OPERADOR> "
#define NOMBRE_POR_DEFECTO  "Operador"

// Opciones del menu principal
#define OPC_INVALIDA       0
#define OPC_NUEVA_JORNADA  1
#define OPC_VER_RANKING    2
#define OPC_SALIR          3

//Motivo por el que termina la jornada (lo informa el motor, modulo 3)
#define FIN_POR_TIEMPO     1   // se alcanzo la duracion maxima
#define FIN_NORMAL         2   // no quedan eventos, buques ni camiones
#define FIN_POR_BLOQUEO    3   // no hay operaciones validas ni eventos

#define LECTURA_OK         1
#define FIN_ENTRADA        0


int leerLinea(char *dest, unsigned tam);
void pedirNombreOperador(char *nombre, unsigned tam);
void mostrarBienvenida(const char *nombre);
int mostrarMenuPrincipal();
void mostrarPendiente(const char *funcionalidad);
void normalizarOrden(char *orden);
int leerOrden(char *orden, unsigned tam);
void mostrarOrdenRechazada(const char *motivo);
void mostrarFinJornada(int motivo, const tHistorial *h, const tPuntuacion *p);




#endif // INTERFAZ_H_INCLUDED
