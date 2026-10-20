#ifndef SIMULACION_H_INCLUDED
#define SIMULACION_H_INCLUDED

#include "puerto.h"

// Puntuacion de la jornada
#define PTS_ENTR        10
#define PTS_BUQUE_DESC  5
#define PENAL_CAMION    2

/* Juega una jornada completa sobre el estado ya cargado: reloj, arribos,
   ordenes del operador, avance automatico, finalizacion y resumen. */
void jugarJornada(tEstadoSimulacion *estado);
void avanzarReloj(tEstadoSimulacion *e, unsigned hasta);
int hayTiempo(const tEstadoSimulacion *e, unsigned duracion);
void completarOperacion(tEstadoSimulacion *e, unsigned duracion, const char *descripcion);
const char *descargar(tEstadoSimulacion *e, unsigned idMuelle, unsigned idZona);
const char *reubicar(tEstadoSimulacion *e, unsigned idOrigen, unsigned idDestino);
const char *entregar(tEstadoSimulacion *e);
void mostrarEstado(tEstadoSimulacion *e);
void mostrarMuelleBreve(void *info, unsigned tamInfo, unsigned n, void *params);
void mostrarEstadoBreve(tEstadoSimulacion *e);
const char *ejecutarOrden(tEstadoSimulacion *e, char *linea);
int hayOperacionValida(tEstadoSimulacion *e, const tRecursos *r);
int finalizacionNormal(const tEstadoSimulacion *e, const tRecursos *r);
unsigned proximoEvento(const tEstadoSimulacion *e);
void aplicarPenalizacion(tEstadoSimulacion *e);
void mostrarResumen(tEstadoSimulacion *e, const char *motivo);

#endif /* SIMULACION_H_INCLUDED */
