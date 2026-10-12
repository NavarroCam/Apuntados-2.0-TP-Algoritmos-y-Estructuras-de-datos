#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#include "comun.h"

typedef tNodo* tLista;
typedef int (*tCmpLista)(const void *, const void *);

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
int insertarEnOrden(tLista *pLista, const void *dato, unsigned cantBytes, tCmpLista cmp);

#endif // LISTA_H_INCLUDED
