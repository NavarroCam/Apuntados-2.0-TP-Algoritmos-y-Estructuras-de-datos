#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED

#include "definiciones.h"

typedef struct{
    tNodo* pri;
    tNodo* ult;
}tCola;

void crearCola(tCola* p);
int colaLlena(const tCola* p, unsigned cantBytes);
int ponerEnCola();
#endif // COLA_H_INCLUDED
