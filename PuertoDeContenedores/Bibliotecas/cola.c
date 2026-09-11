#include "cola.h"

void crearCola(tCola* p)
{
    p->pri=NULL;
    p->ult=NULL;
}
int ponerEnCola(tCola* p, const void* info, unsigned cantB)
{
    tNodo* nuevo = (tNodo*)malloc(sizeof(tNodo));
    if(nuevo==NULL)
    {
        return COLA_LLENA;
    }
    nuevo->info = malloc(cantB);
    if(nuevo->info==NULL)
    {
        free(nuevo);
        return COLA_LLENA;
    }
    memcpy(nuevo->info,info,cantB);
    nuevo->tamInfo=cantB;
    nuevo->siguiente=NULL;
    if(p->pri==NULL)
    {
        p->pri= nuevo;
    }
    else
    {
        p->ult->siguiente=nuevo;
    }
    p->ult=nuevo;


    return TODOOK;
}
void vaciarCola(tCola* p)// pri=0 y ult=0 -->pri=ult
{
    tNodo* aux;
    while(p->pri)
    {
        aux=p->pri;
        p->pri=aux->siguiente;
        free(aux->info);
        free(aux);
    }
}
int colaLlena(const tCola* p, unsigned cantB)
{
    return COLA_NO_LLENA;
}
int colaVacia(const tCola*p)
{
    if(p->pri==NULL)
        return COLA_VACIA;
    return COLA_NO_VACIA;
}

int sacarDeCola(tCola* p,void* info, unsigned cantB)//cantB aca es de info
{
    tNodo* elim;
    if(p->pri==NULL)
    {
        return COLA_VACIA;
    }
    elim=p->pri;
    memcpy(info,elim->info,MINIMO(cantB,elim->tamInfo));
    p->pri=elim->siguiente;
    free(elim->info);
    free(elim);
    if(p->pri==NULL)
        p->ult=NULL;
    return TODOOK;
}
int verPriDeCola(const tCola* p, void* info, unsigned cantB)
{
    if(p->pri==NULL)
    {
        return COLA_VACIA;
    }
    memcpy(info,p->pri->info,MINIMO(cantB,p->pri->tamInfo));
    return TODOOK;
}
