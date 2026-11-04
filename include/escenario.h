#ifndef ESCENARIO_H_INCLUDED
#define ESCENARIO_H_INCLUDED

#include "puerto.h"

int generarEscenario(const char *path, const tConfig *config, int modoTest, unsigned semilla);
int cargarEscenario(const char *path, tEstadoSimulacion *estado);
unsigned numeroAleatorio(unsigned n);
int cargarBuque(char *linea, tEstadoSimulacion *e, unsigned *ultimaLlegada);
int cargarCamion(const char *linea, tEstadoSimulacion *e, unsigned *ultimaLlegada);
#endif /* ESCENARIO_H_INCLUDED */
