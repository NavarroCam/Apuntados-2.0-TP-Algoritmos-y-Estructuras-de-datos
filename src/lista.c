#include "../include/lista.h"

void crearLista(tLista* p)
{
    *p = NULL;
}

int listaVacia(const tLista* p)
{
    return *p == NULL;
}

int listaLlena(const tLista* p, unsigned cantBytes)
{
    tNodo *aux = malloc(sizeof(tNodo));
    void *info = malloc(cantBytes);

    free(aux);
    free(info);
    return aux == NULL || info == NULL;
}

void vaciarLista(tLista* p)
{
    while (*p)
    {
        tNodo *aux = *p;
        *p = aux->sig;
        free(aux->info);
        free(aux);
    }
}

int ponerAlComienzo(tLista* p, const void* d, unsigned cantBytes)
{
    tNodo *nue;

    if(!reservarMemoriaNodo(nue, sizeof(tNodo), nue->info, cantBytes))
        return 0;
    memcpy(nue->info, d, cantBytes);
    nue->tamInfo = cantBytes;
    nue->sig = *p;
    *p = nue;
    return 1;
}

int ponerAlFinal(tLista* p, const void* d, unsigned cantBytes)
{
    tNodo *nue;

    if(!reservarMemoriaNodo(nue, sizeof(tNodo), nue->info, cantBytes))
        return 0;
    while(*p)
        p = &(*p)->sig;
    memcpy(nue->info, d, cantBytes);
    nue->tamInfo = cantBytes;
    nue->sig = NULL;
    *p = nue;
    return 1;
}

int ponerEnOrden(tLista* p, const void* d, unsigned cantBytes,
                 Cmp comparar, Acum acumular)
{
    tNodo *nue;

    while (*p && comparar((*p)->info, d) < 0)
        p = &(*p)->sig;
    if(*p && comparar((*p)->info, d) == 0)
    {
        if(acumular)
            if(!acumular(&(*p)->info, &(*p)->tamInfo, d, cantBytes))
            return SIN_MEM;
        return CLA_DUP;
    }
    if(!reservarMemoriaNodo(nue, sizeof(tNodo), nue->info, cantBytes))
        return 0;
    memcpy(nue->info, d, cantBytes);
    nue->tamInfo = cantBytes;
    nue->sig = *p;
    *p = nue;
    return 1;
}

int sacarPrimeroLista(tLista* p, void* d, unsigned cantBytes)
{
    tNodo *aux = *p;

    if(aux == NULL)
        return 0;
    *p = aux->sig;
    memcpy(d, aux->info, MINIMO(cantBytes, aux->tamInfo));
    free(aux->info);
    free(aux);
    return 1;
}

int sacarUltimoLista(tLista* p, void* d, unsigned cantBytes)
{
    if(*p == NULL)
        return 0;
    while ((*p)->sig)
        p = &(*p)->sig;
    memcpy(d, (*p)->info, MINIMO(cantBytes, (*p)->tamInfo));
    free((*p)->info);
    free(*p);
    *p = NULL;
    return 1;
}

int verPrimeroLista(tLista* p, void* d, unsigned cantBytes)
{
    if(*p == NULL)
        return 0;
    memcpy(d, (*p)->info, MINIMO(cantBytes, (*p)->tamInfo));
    return 1;
}

int verUltimoLista(tLista* p, void* d, unsigned cantBytes)
{
    if(*p == NULL)
        return 0;
    while ((*p)->sig)
        p = &(*p)->sig;
    memcpy(d, (*p)->info, MINIMO(cantBytes, (*p)->tamInfo));
    return 1;
}

int mostrarLista(const tLista* p, Mostrar mostrar, FILE* pf)
{
    while (*p)
    {
        mostrar((*p)->info, pf);
        p = &(*p)->sig;
    }
    return 1;
}
