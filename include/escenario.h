#ifndef ESCENARIO_H_INCLUDED
#define ESCENARIO_H_INCLUDED

#include "comun.h"
#include "cola.h"
#include "config.h"

#define RUTA_ESCENARIO "Archivos/puerto.txt"
#define TAM_CODIGO          12
#define MAX_CONT_POR_BUQUE         40u


typedef struct
{
    char codigo[TAM_CODIGO];
    unsigned minutoLlegada;
    unsigned cantidadContenedores;
    char manifiesto[MAX_CONT_POR_BUQUE][TAM_CODIGO];
} tBuqueEscenario;

typedef struct
{
    char codigo[TAM_CODIGO];
    unsigned minutoLlegada;
    char contenedorSolicitado[TAM_CODIGO];
} tCamionEscenario;

typedef struct
{
    unsigned duracionJornadaMinutos;
    unsigned cantidadMuelles;
    unsigned cantidadZonas;
    unsigned capacidadPila;
    tCola buques;
    tCola camiones;
    unsigned cantidadBuques;
    unsigned cantidadCamiones;
} tEscenario;

int generarEscenario(const tConfiguracion *config, const char *nomArch);
int leerEscenario(tEscenario *esc, const char *nomArch);
void liberarEscenario(tEscenario *esc);


#endif // ESCENARIO_H_INCLUDED
