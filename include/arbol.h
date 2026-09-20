#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include "comun.h"

typedef struct sNodoA
{
    void *info;
    unsigned tamInfo;
    struct sNodoA *izq, *der;
} tNodoArbol;

typedef tNodoArbol *tArbolBinBusq;

void crearArbolBinBusq(tArbolBinBusq *p);
int insertarArbolBinBusq(tArbolBinBusq *p, const void *d, unsigned cantBytes,
                         Cmp comparar);

void recorrerEnOrdenRecArbolBinBusq(tArbolBinBusq *p, unsigned n, void *params, Accion accion);
void recorrerEnOrdenArbolBinBusq(tArbolBinBusq *p, void *params, Accion accion);

tNodoArbol **menorNodoArbolBinBusq(const tArbolBinBusq *p);
tNodoArbol **mayorNodoArbolBinBusq(const tArbolBinBusq *p);
tNodoArbol **buscarNodoArbolBinBusq(const tArbolBinBusq *p,
                                    const void *d, Cmp comparar);
const tArbolBinBusq *mayorNodoNoClaveArbolBinBusq(const tArbolBinBusq *p, const tArbolBinBusq *mayor, Cmp cmp);
int mayorElemNoClaveArbolBinBusq(const tArbolBinBusq *p, void *d, unsigned cantBytes,
                                 Cmp cmp);
unsigned alturaArbolBin(const tArbolBinBusq *p);

#endif // ARBOL_H_INCLUDED
