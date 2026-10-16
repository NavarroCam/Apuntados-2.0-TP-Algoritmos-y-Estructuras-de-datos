#ifndef ESCENARIO_H_INCLUDED
#define ESCENARIO_H_INCLUDED

#include "puerto.h"

/* Genera puerto.txt al azar a partir de la configuracion. Con modoTest en 1
   no lo regenera si ya existe. Con semilla distinta de 0 reinicia rand() con
   ella, de modo que siempre genera el mismo archivo; con 0 usa la semilla que
   el llamador inicializo con srand.
   Devuelve 1 si todo salio bien y 0 ante cualquier error. */
int generarEscenario(const char *path, const tConfig *config, int modoTest, unsigned semilla);

/* Carga el escenario en el estado, que debe estar recien creado con
   crearEstadoSimulacion. Devuelve 1 si todo salio bien y 0 si no. */
int cargarEscenario(const char *path, tEstadoSimulacion *estado);

#endif /* ESCENARIO_H_INCLUDED */
