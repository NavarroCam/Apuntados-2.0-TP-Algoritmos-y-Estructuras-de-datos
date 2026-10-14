#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#include "comun.h"

typedef tNodo* tLista;

void crearLista(tLista *p);
int listaVacia(const tLista *p);
int listaLlena(const tLista *p, unsigned cantBytes);
int ponerAlComienzo(tLista *p, const void *d, unsigned cantBytes);
int ponerAlFinal(tLista *p, const void *d, unsigned cantBytes);
int ponerEnOrden(tLista *p, const void *d, unsigned cantBytes,
                 Cmp comparar, Acum acumular);
int sacarPrimeroLista(tLista *p, void *d, unsigned cantBytes);
int sacarUltimoLista(tLista *p, void *d, unsigned cantBytes);
int verPrimeroLista(tLista *p, void *d, unsigned cantBytes);
int verUltimoLista(tLista *p, void *d, unsigned cantBytes);
void vaciarLista(tLista *p);
int mostrarLista(const tLista *p, Mostrar mostrar, FILE* pf);

void _map(tLista *p, void *params, Accion accion);
tLista* buscarNodoLista(const tLista* p, const void* d, Cmp comparar);
int buscarElemLista(const tLista* p, void* d, unsigned cantBytes, Cmp comparar);
int aplicarEnElemLista(tLista* p, const void* clave, Cmp comparar, void* params, Accion accion);
#endif // LISTA_H_INCLUDED
