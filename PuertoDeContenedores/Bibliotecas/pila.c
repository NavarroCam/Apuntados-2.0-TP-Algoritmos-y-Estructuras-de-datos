#include "pila.h"

void crearPila(tPila* p)
{
    *p=NULL;
}

int apilar(tPila* p, const void* dato, unsigned cantBytes)
{
    tNodo* nuevo = (tNodo*)malloc(sizeof(tNodo));
    if(nuevo==NULL)
    {
        return PILA_LLENA;
    }
    nuevo->info = malloc(cantBytes);
    if(nuevo->info==NULL)
    {
        free(nuevo);
        return PILA_LLENA;
    }
    memcpy(nuevo->info,dato,cantBytes);
    nuevo->tamInfo=cantBytes;
    nuevo->sig=*p;///para el primer nodo va a ser NULL
    *p=nuevo;
    return TODO_OK;
}
int verTope(const tPila* p, void* dato, unsigned cantBytes)
{
    tNodo* tope=*p;
    if(tope==NULL)
    {
        return PILA_VACIA;
    }
    memcpy(dato,tope->info,MINIMO(cantBytes,tope->tamInfo));
    return TODO_OK;

}
int desapilar(tPila* p, void* dato, unsigned cantBytes)
{
    tNodo* elim= *p;
    if(elim==NULL)
    {
        return PILA_VACIA;
    }
    memcpy(dato,elim->info,MINIMO(cantBytes,elim->tamInfo));
    *p=elim->sig;
    free(elim->info);
    free(elim);
    return TODO_OK;
}
void vaciarPila(tPila* p)
{
    tNodo* aux;
    while(*p)
    {
        aux=*p;
        *p=aux->sig;
        free(aux->info);
        free(aux);
    }
}
int pilaVacia(const tPila* pila)
{
    if(*pila==NULL)
    {
        return PILA_VACIA;
    }
    return PILA_NO_VACIA;
}
int pilaLlena(const tPila* p, unsigned cantBytes)
{
    tNodo* aux = (tNodo*)malloc(sizeof(tNodo));
    void* info = malloc(cantBytes);

    free(aux);
    free(info);

    if(aux==NULL || info ==NULL)
    {
        return PILA_LLENA;
    }
    return PILA_NO_LLENA;
}
