#include "../include/comun.h"

void mostrarEntero(const void *dato, FILE* pf)
{
    fprintf(pf, "%d\n", *(int*)dato);
}

int compararEnteros(const void *d1, const void *d2)
{
    return *(int*)d1 - *(int*)d2;
}
