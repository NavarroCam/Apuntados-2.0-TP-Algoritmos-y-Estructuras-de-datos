#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include "comun.h"

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

int cargarConfiguracion(tConfig *config, const char *nomArch);
int guardarConfiguracion(const tConfig *config, const char *nomArch);
void mostrarConfiguracion(const tConfig *config);

char *recortarCadena(char *cadena);
void convertMinusCadena(char *cadena);
int esNumeroPos(const char* cad);
void asignarParametro(tConfig *config, const char *clave, unsigned valor);
void establecerConfiguracionPorDefecto(tConfig *config);
int configuracionValida(const tConfig *config);
#endif
