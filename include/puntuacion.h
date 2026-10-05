#ifndef PUNTUACION_H_INCLUDED
#define PUNTUACION_H_INCLUDED

#include "comun.h"

#define PUNTOS_POR_ENTREGA          10
#define PUNTOS_POR_BUQUE_DESCARGADO  5
#define PENALIZACION_POR_CAMION      2

typedef struct{
    unsigned contenedoresEntregados;
    unsigned buquesDescargados;
    unsigned camionesPendientes;
    unsigned reorganizaciones;
    unsigned tiempoUtilizado;
    unsigned duracionJornada;
} tPuntuacion;

void iniciarPuntuacion(tPuntuacion *p, unsigned duracionJornada);
void registrarEntrega(tPuntuacion *p);
void registrarBuqueDescargado(tPuntuacion *p);
void registrarReorganizacion(tPuntuacion *p);
void cerrarPuntuacion(tPuntuacion *p, unsigned tiempoUtilizado, unsigned camionesPendientes);
int calcularPuntajeParcial(const tPuntuacion *p);
int calcularPuntajeFinal(const tPuntuacion *p);
void mostrarResumenFinal(const tPuntuacion *p, FILE *pf);

#endif // PUNTUACION_H_INCLUDED
