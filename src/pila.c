#include "../include/pila.h"

void crearPila(tPila* p)
{
    *p = NULL;
}

int pilaVacia(const tPila* p)
{
    return *p == NULL;
}

int pilaLlena(const tPila* p, unsigned cantBytes)
{
    tNodo *aux = malloc(sizeof(tNodo));
    void *info = malloc(cantBytes);

    free(aux);
    free(info);
    return aux == NULL || info == NULL;
}

int ponerEnPila(tPila* p, const void* d, unsigned cantBytes)
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

int sacarDePila(tPila* p, void* d, unsigned cantBytes)
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

int verTope(const tPila* p, void* d, unsigned cantBytes)
{
    if(*p == NULL)
        return 0;
    memcpy(d, (*p)->info, MINIMO(cantBytes, (*p)->tamInfo));
    return 1;
}

void vaciarPila(tPila* p)
{
    while (*p)
    {
        tNodo *aux = *p;
        *p = aux->sig;
        free(aux->info);
        free(aux);
    }
}
