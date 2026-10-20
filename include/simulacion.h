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

#endif /* SIMULACION_H_INCLUDED */
