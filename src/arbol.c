#include "../include/arbol.h"

void crearArbolBinBusq(tArbolBinBusq *p)
{
    *p = NULL;
}

int insertarArbolBinBusq(tArbolBinBusq *p, const void *d, unsigned cantBytes, Cmp comparar)
{
    tNodoArbol *nue;
    int rc;

    while (*p)
    {
        if ((rc = comparar(d, (*p)->info)) < 0)
            p = &(*p)->izq;
        else if (rc > 0)
            p = &(*p)->der;
        else
            return CLA_DUP;
    }

    if (!reservarMemoriaNodo(nue, sizeof(tNodoArbol), nue->info, cantBytes))
        return SIN_MEM;
    memcpy(nue->info, d, cantBytes);
    nue->tamInfo = cantBytes;
    nue->izq = nue->der = NULL;
    *p = nue;
    return TODO_BIEN;
}

void recorrerEnOrdenRecArbolBinBusq(tArbolBinBusq *p, unsigned n, void *params, Accion accion)
{
    if (!*p)
        return;
    recorrerEnOrdenRecArbolBinBusq(&(*p)->izq, n + 1, params, accion);
    accion((*p)->info, (*p)->tamInfo, n, params);
    recorrerEnOrdenRecArbolBinBusq(&(*p)->der, n + 1, params, accion);
}

void recorrerEnOrdenArbolBinBusq(tArbolBinBusq *p, void *params, Accion accion)
{
    recorrerEnOrdenRecArbolBinBusq(p, 0, params, accion);
}

tNodoArbol **mayorNodoRecArbolBinBusq(const tArbolBinBusq *p)
{
    if (!*p)
        return NULL;
    if (!(*p)->der)
        return (tNodoArbol **)p;
    return mayorNodoRecArbolBinBusq(&(*p)->der);
}

int mayorElemArbolBinBusq(const tArbolBinBusq *p, void *d, unsigned cantBytes)
{
    if (!(p = mayorNodoRecArbolBinBusq(p)))
        return 0;
    memcpy(d, (*p)->info, MINIMO(cantBytes, (*p)->tamInfo));
    return 1;
}

tNodoArbol **menorNodoRecArbolBinBusq(const tArbolBinBusq *p)
{
    if (!*p)
        return NULL;
    if (!(*p)->izq)
        return (tNodoArbol **)p;
    return menorNodoRecArbolBinBusq(&(*p)->izq);
}

tNodoArbol **menorNodoArbolBinBusq(const tArbolBinBusq *p)
{
    if (!*p)
        return NULL;
    while ((*p)->izq)
        p = &(*p)->izq;
    return (tNodoArbol **)p;
}

tNodoArbol **mayorNodoArbolBinBusq(const tArbolBinBusq *p)
{
    if (!*p)
        return NULL;
    while ((*p)->der)
        p = &(*p)->der;
    return (tNodoArbol **)p;
}

int menorElemArbolBinBusq(const tArbolBinBusq *p, void *d, unsigned cantBytes)
{
    if (!(p = menorNodoRecArbolBinBusq(p)))
        return 0;
    memcpy(d, (*p)->info, MINIMO(cantBytes, (*p)->tamInfo));
    return 1;
}

tNodoArbol **buscarNodoArbolBinBusq(const tArbolBinBusq *p, const void *d, Cmp comparar)
{
    int rc;

    while (*p && (rc = comparar(d, (*p)->info)))
    {
        if (rc < 0)
            p = &(*p)->izq;
        else
            p = &(*p)->der;
    }
    if (!*p)
        return NULL;
    return (tNodoArbol **)p;
}

int buscarElemArbolBinBusq(const tArbolBinBusq *p, void *d, unsigned cantBytes, Cmp comparar)
{
    if (!(p = buscarNodoArbolBinBusq(p, d, comparar)))
        return 0;

    memcpy(d, (*p)->info, MINIMO((*p)->tamInfo, cantBytes));
    return 1;
}

unsigned alturaArbolBin(const tArbolBinBusq *p)
{
    int hi, hd;

    if (!*p)
        return 0;

    hi = alturaArbolBin(&(*p)->izq);
    hd = alturaArbolBin(&(*p)->der);

    return (hi > hd ? hi : hd) + 1;
}

int eliminarRaizArbol(tArbolBinBusq *p)
{
    tNodoArbol **remp, *elim;

    if (!*p)
        return 0;

    free((*p)->info);
    if (!(*p)->izq && !(*p)->der)
    {
        free(*p);
        *p = NULL;
        return 1;
    }

    remp = alturaArbolBin(&(*p)->izq) > alturaArbolBin(&(*p)->der)
               ? mayorNodoArbolBinBusq(&(*p)->izq)
               : menorNodoArbolBinBusq(&(*p)->der);

    elim = *remp;
    (*p)->info = elim->info;
    (*p)->tamInfo = elim->tamInfo;
    *remp = elim->izq ? elim->izq : elim->der;
    free(elim);
    return 1;
}

int eliminarElemArbolBinBusq(tArbolBinBusq *p, void *d, unsigned cantBytes, Cmp comparar)
{
    if (!(p = buscarNodoArbolBinBusq(p, d, comparar)))
        return 0;
    memcpy(d, (*p)->info, MINIMO(cantBytes, (*p)->tamInfo));
    return eliminarRaizArbol(p);
}

unsigned leerDesdeArchivo(void **d, void *pf, unsigned pos, void *params)
{
    unsigned tam = *(int *)params;
    *d = malloc(tam);
    if (!*d)
        return 0;
    fseek((FILE *)pf, pos * tam, SEEK_SET);
    return fread(*d, tam, 1, (FILE *)pf) ? tam : 0;
}

int cargarDesdeDatosOrdenadosRec(tArbolBinBusq *p, void *ds, unsigned (*leer)(void **, void *, unsigned, void *),
                                 int li, int ls, void *params)
{
    int m = (li + ls) / 2, r;

    *p = malloc(sizeof(tNodoArbol));
    if (!*p || !((*p)->tamInfo = leer(&(*p)->info, ds, m, params)))
    {
        free(*p);
        return SIN_MEM;
    }

    (*p)->izq = (*p)->der = NULL;
    if ((r = cargarDesdeDatosOrdenadosRec(&(*p)->izq, ds, leer, li, m - 1, params)) != TODO_BIEN)
        return r;
    return cargarDesdeDatosOrdenadosRec(&(*p)->der, ds, leer, m + 1, ls, params);
}

int cargarArchivoBinOrdenadoAbiertoArb01BinBusq(tArbolBinBusq *p, FILE *pf, unsigned tamlnfo)
{
    int cantReg;

    if (*p || !pf)
        return 0;
    fseek(pf, 0L, SEEK_END);
    cantReg = ftell(pf) / tamlnfo;
    return cargarDesdeDatosOrdenadosRec(p, pf, leerDesdeArchivo, 0, cantReg - 1, &tamlnfo);
}

int cargarArchivoBinOrdenadoArbolBinBusq(tArbolBinBusq *p, const char *path, unsigned tamInfo)
{
    FILE *pf;
    int cantReg, r;

    if (*p)
        return SIN_INICIALIZAR;

    if (!(pf = fopen(path, "rt")))
        return ERROR_ARCH;

    fseek(pf, 0L, SEEK_END);
    cantReg = ftell(pf) / tamInfo;
    r = cargarDesdeDatosOrdenadosRec(p, pf, leerDesdeArchivo, 0, cantReg - 1, &tamInfo);
    fclose(pf);
    return r;
}

const tArbolBinBusq *mayorNodoNoClaveArbolBinBusq(const tArbolBinBusq *p, const tArbolBinBusq *mayor, Cmp cmp)
{
    if (!*p)
        return mayor;
    if (cmp((*p)->info, (*mayor)->info) > 0)
        mayor = p;
    mayor = mayorNodoNoClaveArbolBinBusq(&(*p)->izq, mayor, cmp);
    mayor = mayorNodoNoClaveArbolBinBusq(&(*p)->der, mayor, cmp);
    return mayor;
}

int mayorElemNoClaveArbolBinBusq(const tArbolBinBusq *p, void *d, unsigned cantBytes, Cmp comparar)
{
    const tArbolBinBusq *mayor = p;
    if (!*p)
        return 0;
    mayor = mayorNodoNoClaveArbolBinBusq(&(*p)->izq, mayor, comparar);
    mayor = mayorNodoNoClaveArbolBinBusq(&(*p)->der, mayor, comparar);

    memcpy(d, (*mayor)->info, MINIMO(cantBytes, (*mayor)->tamInfo));
    return 1;
}

unsigned cantNodosArbolBinBusq(const tArbolBinBusq *p)
{
    if (!*p)
        return 0;
    return cantNodosArbolBinBusq(&(*p)->izq) +
           cantNodosArbolBinBusq(&(*p)->der) + 1;
}

unsigned cantNodosHastaNivelArbolBin(const tArbolBinBusq *p, int n)
{
    if (!*p)
        return 0;
    if (n == 0)
        return 1;
    return cantNodosHastaNivelArbolBin(&(*p)->izq, n - 1) +
           cantNodosHastaNivelArbolBin(&(*p)->der, n - 1) + 1;
}

int esCompletoHastaNivelArbolBin(const tArbolBinBusq *p, int n)
{
    if (!*p)
        return n < 0;
    if (n == 0)
        return 1;

    return esCompletoHastaNivelArbolBin(&(*p)->izq, n - 1) &&
           esCompletoHastaNivelArbolBin(&(*p)->der, n - 1);
}

int esCompletoArbolBin(const tArbolBinBusq *p)
{
    return esCompletoHastaNivelArbolBin(p, alturaArbolBin(p) - 1);
}

int esBalanceadoArbolBin(const tArbolBinBusq *p)
{
    return esCompletoHastaNivelArbolBin(p, alturaArbolBin(p) - 2);
}

int esAVLArbolBin(const tArbolBinBusq *p)
{
    int hi, hd;

    if (!*p)
        return 1;

    hi = alturaArbolBin(&(*p)->izq);
    hd = alturaArbolBin(&(*p)->der);

    if (abs(hi - hd) > 1)
        return 0;

    return esAVLArbolBin(&(*p)->izq) && esAVLArbolBin(&(*p)->der);
}
