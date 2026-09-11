#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED

#include "definiciones.h"

#define COLA_VACIA 1
#define COLA_LLENA 1
#define COLA_NO_LLENA 0
#define COLA_NO_VACIA 0

typedef struct{
    tNodo* pri;
    tNodo* ult;
}tCola;

void crearCola(tCola* p);
void vaciarCola(tCola* p);// pri=0 y ult=0 -->pri=ult
int colaLlena(const tCola* p, unsigned cantBytes
int colaVacia(const tCola*p);
int ponerEnCola(tCola* p, const void* info, unsigned cantBytes
int sacarDeCola(tCola* p,void* info, unsigned cantB)=//cantB aca es de info
int verPriDeCola(const tCola* p, void* info, unsigned cantBytes);

#endif // COLA_H_INCLUDED
