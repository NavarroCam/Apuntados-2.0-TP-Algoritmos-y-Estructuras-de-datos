#ifndef PILA_H_INCLUDED
#define PILA_H_INCLUDED

#include "definiciones.h"

#define PILA_LLENA 1
#define PILA_NO_LLENA 0
#define PILA_VACIA 1
#define PILA_NO_VACIA 0

typedef tNodo* tPila;

void crearPila(tPila* p);
int apilar(tPila* p, const void* dato, unsigned cantBytes);
int verTope(const tPila* p, void* dato, unsigned cantBytes);
int desapilar(tPila* p, void* dato, unsigned cantBytes);
void vaciarPila(tPila* p);
int pilaVacia(const tPila* p);
int pilaLlena(const tPila* p, unsigned cantBytes);

#endif // PILA_H_INCLUDED
