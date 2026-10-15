#ifndef SIMULACION_H_INCLUDED
#define SIMULACION_H_INCLUDED

#include "puerto.h"
#include "operadores.h"
#include "escenario.h"

/* Juega una jornada completa sobre el estado ya cargado: reloj, arribos,
   ordenes del operador, avance automatico, finalizacion y resumen. */
void jugarJornada(tEstadoSimulacion *estado);
void nuevaJornada(unsigned posicion, const char *nombre);

#endif /* SIMULACION_H_INCLUDED */
