#ifndef PUERTO_H_INCLUDED
#define PUERTO_H_INCLUDED

#include "comun.h"
#include "pila.h"
#include "cola.h"
#include "lista.h"
#include "config.h"

#define TAM_CODIGO      8
#define TAM_OPERACION   4
#define TAM_LINEA       1024
#define RECURSO_VACIO   "."

typedef struct
{
    char codigo[TAM_CODIGO];
} tContenedor;

/* El manifiesto es una cola de tContenedor en el orden de puerto.txt.
   Al copiar un tBuque se copian los punteros de la cola: la copia anterior
   deja de usarse (el buque "se mueve", no se duplica). */
typedef struct
{
    char codigo[TAM_CODIGO];
    unsigned llegada;
    tCola manifiesto;
} tBuque;

typedef struct
{
    char codigo[TAM_CODIGO];
    unsigned llegada;
    char contenedor[TAM_CODIGO];
} tCamion;

/* tZona y tMuelle empiezan con el id para compararlos con compararId. */
typedef struct
{
    unsigned id;
    unsigned capacidad;
    unsigned cantidad;
    tPila pila;
} tZona;

typedef struct
{
    unsigned id;
    int ocupado;
    tBuque buque;
} tMuelle;

typedef struct
{
    unsigned inicio;
    unsigned fin;
    char operacion[TAM_OPERACION];
    char contenedor[TAM_CODIGO];
    char origen[TAM_CODIGO];
    char destino[TAM_CODIGO];
} tRegistroOperacion;

typedef struct
{
    tConfig config;
    unsigned reloj;
    tCola arribosBuques;
    tCola arribosCamiones;
    tCola esperaBuques;
    tCola colaCamiones;
    tLista muelles;
    tLista zonas;
    tLista historial;
    int puntaje;
    unsigned contenedoresEntregados;
    unsigned buquesDescargados;
    unsigned reorganizaciones;
    unsigned camionesPendientes;
} tEstadoSimulacion;

/* Lo que cuentan contarZona y contarMuelle para evaluar la jornada. */
typedef struct
{
    unsigned noVacias;
    unsigned conLugar;
    unsigned ambas;
    unsigned ocupados;
    unsigned contenedores;
} tRecursos;

/* Parametro de las acciones que mueven un contenedor dentro de las listas. */
typedef struct
{
    tContenedor cont;
    unsigned idZona;
    int ok;
} tMovimiento;

void crearZona(tZona *zona, unsigned id, unsigned capacidad);
void crearMuelle(tMuelle *muelle, unsigned id);
void crearEstadoSimulacion(tEstadoSimulacion *estado, const tConfig *config);
void vaciarEstadoSimulacion(tEstadoSimulacion *estado);

int compararId(const void *d1, const void *d2);
int compararContenedorConTopeZona(const void *contenedor, const void *zona);

void verCola(tCola *cola, unsigned tamInfo, void *params, Accion accion);
void mostrarZona(void *info, unsigned tamInfo, unsigned n, void *params);
void mostrarMuelle(void *info, unsigned tamInfo, unsigned n, void *params);
void mostrarRegistroOperacion(void *info, unsigned tamInfo, unsigned n, void *params);
void mostrarCamionVentana(void *info, unsigned tamInfo, unsigned n, void *params);

void contarElementos(void *info, unsigned tamInfo, unsigned n, void *params);
void contarZona(void *info, unsigned tamInfo, unsigned n, void *params);
void contarMuelle(void *info, unsigned tamInfo, unsigned n, void *params);

void asignarMuelle(void *info, unsigned tamInfo, unsigned n, void *params);
void apilarEnZona(void *info, unsigned tamInfo, unsigned n, void *params);
void desapilarDeZona(void *info, unsigned tamInfo, unsigned n, void *params);
void descargarDelMuelle(void *info, unsigned tamInfo, unsigned n, void *params);

#endif /* PUERTO_H_INCLUDED */
