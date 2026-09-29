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

tNodoArbol **mayorNodoRecArbolBinBusq(const tArbolBinBusq *p);
int mayorElemArbolBinBusq(const tArbolBinBusq *p, void *d, unsigned cantBytes);
tNodoArbol **menorNodoRecArbolBinBusq(const tArbolBinBusq *p);
tNodoArbol **menorNodoArbolBinBusq(const tArbolBinBusq *p);
tNodoArbol **mayorNodoArbolBinBusq(const tArbolBinBusq *p);
int menorElemArbolBinBusq(const tArbolBinBusq *p, void *d, unsigned cantBytes);
tNodoArbol **buscarNodoArbolBinBusq(const tArbolBinBusq *p,
                                    const void *d, Cmp comparar);
int buscarElemArbolBinBusq(const tArbolBinBusq *p, void *d, unsigned cantBytes,
                           Cmp comparar);
unsigned alturaArbolBin(const tArbolBinBusq *p);

int eliminarRaizArbol(tArbolBinBusq *p);
int eliminarElemArbolBinBusq(tArbolBinBusq *p, void *d, unsigned cantBytes,
                             Cmp comparar);

unsigned leerDesdeArchivo(void **d, void *pf, unsigned pos, void *params);
int cargarDesdeDatosOrdenadosRec(tArbolBinBusq *p, void *ds,
                                 unsigned (*leer)(void **, void *, unsigned, void *),
                                 int li, int ls, void *params);
int cargarArchivoBinOrdenadoAbiertoArbolBinBusq(tArbolBinBusq *p, FILE *pf,
                                                unsigned tamlnfo);
int cargarArchivoBinOrdenadoArbolBinBusq(tArbolBinBusq *p, const char *path,
                                         unsigned tamInfo);

const tArbolBinBusq *mayorNodoNoClaveArbolBinBusq(const tArbolBinBusq *p, const tArbolBinBusq *mayor, Cmp cmp);
int mayorElemNoClaveArbolBinBusq(const tArbolBinBusq *p, void *d, unsigned cantBytes,
                                 Cmp cmp);

unsigned cantNodosArbolBinBusq(const tArbolBinBusq *p);
unsigned cantNodosHastaNivelArbolBin(const tArbolBinBusq *p, int n);
int esCompletoHastaNivelArbolBin(const tArbolBinBusq *p, int n);
int esCompletoArbolBin(const tArbolBinBusq *p);
int esBalanceadoArbolBin(const tArbolBinBusq *p);
int esAVLArbolBin(const tArbolBinBusq *p);

#endif // ARBOL_H_INCLUDED
