#ifndef PILA_H_INCLUDED
#define PILA_H_INCLUDED

#include "comun.h"

typedef tNodo* tPila;

void crearPila(tPila *p);
int pilaVacia(const tPila *p);
int pilaLlena(const tPila *p, unsigned cantBytes);
int ponerEnPila(tPila *p, const void *d, unsigned cantBytes);
int sacarDePila(tPila *p, void *d, unsigned cantBytes);
int verTope(const tPila *p, void *d, unsigned cantBytes);
void vaciarPila(tPila *p);

#endif // PILA_H_INCLUDED
