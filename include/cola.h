#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED

#include "comun.h"

typedef struct
{
    tNodo *pri, *ult;
} tCola;

void crearCola(tCola *p);
int colaVacia(const tCola *p);
int colaLlena(const tCola *p, unsigned cantBytes);
int ponerEnCola(tCola *p, const void *d, unsigned cantBytes);
int sacarDeCola(tCola *p, void *d, unsigned cantBytes);
int verPrimeroCola(const tCola *p, void *d, unsigned cantBytes);
void vaciarCola(tCola *p);

#endif // COLA_H_INCLUDED
