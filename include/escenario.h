#ifndef ESCENARIO_H_INCLUDED
#define ESCENARIO_H_INCLUDED
#include "puerto.h"


int generarEscenario(const char *path, const tConfig *config, int modoTest, unsigned semilla);
int cargarEscenario(const char *path, tEstadoSimulacion *estado);

#endif // ESCENARIO_H_INCLUDED
