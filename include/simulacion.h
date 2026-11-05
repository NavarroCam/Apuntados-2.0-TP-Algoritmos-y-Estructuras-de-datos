#ifndef SIMULACION_H_INCLUDED
#define SIMULACION_H_INCLUDED

#include "operaciones.h"
#include "operadores.h"
#include "escenario.h"
#include "visualizacion.h"

#define TEST        0
#define SEMILLA     0

void nuevaJornada(unsigned posicion, const char *nombre, const char* archConfig, const char* archPuerto);

void jugarJornada(tEstadoSimulacion *estado);

int hayOperacionValida(tEstadoSimulacion *e, const tRecursos *r);
int finalizacionNormal(const tEstadoSimulacion *e, const tRecursos *r);
unsigned proximoEvento(const tEstadoSimulacion *e);
void aplicarPenalizacion(tEstadoSimulacion *e);

#endif /* SIMULACION_H_INCLUDED */
