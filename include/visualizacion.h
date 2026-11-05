#ifndef VISUALIZACION_H_INCLUDED
#define VISUALIZACION_H_INCLUDED

#include "puerto.h"

#define TITULO      "PUERTO DE CONTENEDORES - OPERACION CONTRARRELOJ"

void mostrarMenuPrincipal(const char *nombre, const char *aviso);
void mostrarEstado(tEstadoSimulacion *e);
void mostrarMuelleBreve(void *info, unsigned tamInfo, unsigned n, void *params);
void mostrarEstadoBreve(tEstadoSimulacion *e);
void mostrarResumen(tEstadoSimulacion *e, const char *motivo);

#endif // VISUALIZACION_H_INCLUDED
