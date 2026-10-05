#ifndef HISTORIAL_H_INCLUDED
#define HISTORIAL_H_INCLUDED

#include "comun.h"
#include "lista.h"

#define TAM_COD_OPERACION  4    // "DES", "REU", "ENT", "ESP" + '\0'
#define TAM_COD_CONTENEDOR 11
#define TAM_RECURSO        11
#define SIN_DATO           "-"  // se muestra cuando no corresponde

typedef struct {
    unsigned inicio;
    unsigned fin;
    char codigo[TAM_COD_OPERACION];
    char contenedor[TAM_COD_CONTENEDOR];
    char origen[TAM_RECURSO];
    char destino[TAM_RECURSO];
} tOperacion;

typedef tLista tHistorial;

void copiarCampo(char *dest, const char *orig, unsigned tam);
void mostrarOperacion(const void *dato, FILE *pf);
void crearHistorial(tHistorial *h);
int registrarOperacion(tHistorial *h, unsigned inicio, unsigned fin, const char *codigo, const char *contenedor, const char *origen, const char *destino);
void mostrarHistorial(const tHistorial *h, FILE *pf);
void vaciarHistorial(tHistorial *h);

#endif // HISTORIAL_H_INCLUDED
