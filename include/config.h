#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include "comun.h"

/* Limites para que los codigos entren en TAM_CODIGO (B999, K999 y
   C<buque><nn>, por ejemplo C101 o C99999). */
#define MAX_RECURSOS                999
#define MAX_BUQUES                  999
#define MAX_CAMIONES                999
#define MAX_CONTENEDORES_POR_BUQUE  99

typedef struct
{
    unsigned duracionJornadaMinutos;
    unsigned cantidadMuelles;
    unsigned cantidadZonasAlmacenamiento;
    unsigned capacidadPila;
    unsigned maximoBuques;
    unsigned maximoContenedoresPorBuque;
    unsigned maximoCamiones;
    unsigned tiempoDescargaContenedor;
    unsigned tiempoReubicacionContenedor;
    unsigned tiempoCargaCamion;
} tConfig;

/* Lee config.txt; los parametros que falten quedan con los valores del
   ejemplo de la consigna y, si el archivo no existe, lo genera con ellos.
   Devuelve TODO_BIEN, o ERROR_ARCH si no se pudo generar o algun valor
   supera los maximos. */
int cargarConfiguracion(tConfig *config, const char *nomArch);
int guardarConfiguracion(const tConfig *config, const char *nomArch);
void mostrarConfiguracion(const tConfig *config);

#endif /* CONFIG_H_INCLUDED */
