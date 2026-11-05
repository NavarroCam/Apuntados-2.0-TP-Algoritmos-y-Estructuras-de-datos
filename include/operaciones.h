#ifndef OPERACIONES_H_INCLUDED
#define OPERACIONES_H_INCLUDED

#include "puerto.h"
#include "visualizacion.h"

#define TAM_DESCRIPCION     32
#define SIN_TIEMPO          "no alcanza el tiempo restante de la jornada"
#define SIN_MEMORIA         "memoria insuficiente"
#define SIN_PARAMETROS      "la operacion no lleva parametros"

void avanzarReloj(tEstadoSimulacion *e, unsigned hasta);
int hayTiempo(const tEstadoSimulacion *e, unsigned duracion);
void completarOperacion(tEstadoSimulacion *e, unsigned duracion, const char *descripcion);

const char *descargar(tEstadoSimulacion *e, unsigned idMuelle, unsigned idZona);
const char *reubicar(tEstadoSimulacion *e, unsigned idOrigen, unsigned idDestino);
const char *entregar(tEstadoSimulacion *e);
const char *ejecutarOrden(tEstadoSimulacion *e, char *linea);

#endif // OPERACIONES_H_INCLUDED
