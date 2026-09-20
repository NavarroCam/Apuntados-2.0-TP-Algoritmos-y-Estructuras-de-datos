#ifndef COMUN_H_INCLUDED
#define COMUN_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define TODO_BIEN       1
#define SIN_MEM         0
#define CLA_DUP         0
#define ERROR_ARCH      0
#define SIN_INICIALIZAR 0

#define MINIMO(X, Y) ((X) <= (Y) ? (X) : (Y))

#define reservarMemoriaNodo(X, Y, Z, W) (          \
    ((X) = (typeof(X))malloc(Y)) == NULL ||         \
    ((Z) = malloc(W)) == NULL ?                     \
        free(X), 0 : 1                              \
)

typedef struct sNodo
{
    void           *info;
    unsigned        tamInfo;
    struct sNodo   *sig;
} tNodo;

typedef int(*Cmp)(const void*, const void*);
typedef int(*Acum)(void**, unsigned*, const void*, unsigned);
typedef void(*Mostrar)(const void*, FILE*);
typedef void(*Accion)(void*, unsigned, unsigned, void*);


void mostrarEntero(const void *dato, FILE* pf);
int compararEnteros(const void* d1, const void* d2);

#endif // COMUN_H_INCLUDED
