#ifndef OPERADORES_H_INCLUDED
#define OPERADORES_H_INCLUDED

#include "arbol.h"
#include "puerto.h"

#define PATH_OPERADORES "Archivos/operadores.bin"
#define PATH_INDICE     "Archivos/operadores.idx"
#define PATH_JORNADAS   "Archivos/jornadas.bin"

#define TAM_NOMBRE          31
#define OPERADOR_ERROR      0
#define OPERADOR_EXISTENTE  1
#define OPERADOR_NUEVO      2

typedef struct
{
    char nombre[TAM_NOMBRE];
    int puntajeAcumulado;
    unsigned jornadas;
} tOperador;

typedef struct
{
    char nombre[TAM_NOMBRE];
    unsigned posicion;
} tIndiceOperador;

typedef struct
{
    char operador[TAM_NOMBRE];
    int puntaje;
    unsigned contenedoresEntregados;
    unsigned buquesDescargados;
    unsigned camionesPendientes;
    unsigned reorganizaciones;
    unsigned tiempoUtilizado;
} tJornada;

int ingresarOperador(tArbolBinBusq *indice, const char *nombre, unsigned *posicion);
int registrarJornada(unsigned posicion, const char *nombre, const tEstadoSimulacion *estado);
void mostrarRanking(tArbolBinBusq *indice);
int compararIndices(const void *d1, const void *d2);
int compararRanking(const void *d1, const void *d2);
void grabarIndice(void *info, unsigned tamInfo, unsigned n, void *params);
int guardarIndice(tArbolBinBusq *indice);
void agregarAlRanking(void *info, unsigned tamInfo, unsigned n, void *params);
void mostrarPosicionRanking(void *info, unsigned tamInfo, unsigned n, void *params);
void normalizarNombreOperario(char* nombre);
#endif /* OPERADORES_H_INCLUDED */
