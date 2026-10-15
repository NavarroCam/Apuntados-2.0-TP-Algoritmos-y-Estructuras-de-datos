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

#define PATH_CONFIG "Archivos/config.txt"
#define PATH_PUERTO "Archivos/puerto.txt"
#define TITULO      "PUERTO DE CONTENEDORES - OPERACION CONTRARRELOJ"

/* Pruebas: TEST en 1 juega con el puerto.txt existente sin regenerarlo.
   SEMILLA distinta de 0 hace que cada jornada genere siempre el mismo
   puerto.txt; en 0 cada jornada es aleatoria. Para la entrega: 0 y 0. */
#define TEST        1
#define SEMILLA     0

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
