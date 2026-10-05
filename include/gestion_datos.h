/* ============================================================
   include/gestion_datos.h
   Capa de persistencia: operadores, indice ABB y jornadas.

   Archivos/operadores.bin -> registros tOperador (acceso directo
                              por numero de registro).
   Archivos/operadores.idx -> registros tIndiceOperador escritos en
                              ORDEN ALFABETICO (recorrido en orden
                              del ABB). Al iniciar se reconstruye un
                              ABB perfectamente balanceado.
   Archivos/jornadas.bin   -> por cada jornada: un registro tJornada
                              seguido de 'cantidadOperaciones'
                              registros del historial.
   ============================================================ */
#ifndef GESTION_DATOS_H_INCLUDED
#define GESTION_DATOS_H_INCLUDED

#include "config.h"
#include "arbol.h"
#include "lista.h"

#define RUTA_OPERADORES "Archivos/operadores.bin"
#define RUTA_INDICE     "Archivos/operadores.idx"
#define RUTA_JORNADAS   "Archivos/jornadas.bin"

#define TAM_FECHA 20

typedef struct
{
    char nombre[TAM_NOMBRE_OPERADOR];
    unsigned jornadasJugadas;
    long puntajeAcumulado;
    long mejorPuntaje;
} tOperador;

typedef struct
{
    char nombre[TAM_NOMBRE_OPERADOR];
    unsigned nroRegistro;
} tIndiceOperador;

typedef struct
{
    char nombreOperador[TAM_NOMBRE_OPERADOR];
    char fecha[TAM_FECHA];
    long puntaje;
    unsigned contenedoresEntregados;
    unsigned buquesDescargados;
    unsigned camionesPendientes;
    unsigned reubicaciones;
    unsigned tiempoUtilizado;
    unsigned cantidadOperaciones;
    unsigned tamRegistroOperacion;
} tJornada;

void normalizarNombreOperador(char *destino, const char *origen);

int cargarIndiceOperadores(tArbolBinBusq *indice);
int guardarIndiceOperadores(const tArbolBinBusq *indice);
int obtenerOperador(const tArbolBinBusq *indice, const char *nombre,
                    tOperador *operador, unsigned *nroRegistro);
int altaOperador(tArbolBinBusq *indice, const char *nombre,
                 tOperador *operador, unsigned *nroRegistro);
int actualizarOperador(const tOperador *operador, unsigned nroRegistro);

int registrarJornada(const tJornada *jornada, tLista *historialOperaciones);
void mostrarRankingOperadores(void);
void mostrarHistorialJornadas(const char *nombreOperador);

#endif /* GESTION_DATOS_H_INCLUDED */
