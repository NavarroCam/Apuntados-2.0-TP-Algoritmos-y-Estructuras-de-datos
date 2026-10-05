#include <stdio.h>
#include <stdlib.h>
#include "./include/comun.h"
#include "./include/cola.h"
#include "./include/pila.h"
#include "./include/lista.h"

#include "./include/gestion_datos.h"

#define TAM_ENTRADA 128

/* Solicita el nombre del operador. Devuelve 0 si no pudo obtenerse. */
static int leerNombreOperador(char *nombre)
{
    char entrada[TAM_ENTRADA];

    printf("\n Ingrese el nombre del operador: ");
    fflush(stdout);
    if(fgets(entrada, TAM_ENTRADA, stdin) == NULL)
        return 0;
    normalizarNombreOperador(nombre, entrada);
    return nombre[0] != '\0';
}

static void obtenerFechaActual(char *destino)
{
    time_t instante;
    struct tm *fecha;

    instante = time(NULL);
    fecha = localtime(&instante);
    if(fecha == NULL)
        strcpy(destino, "00/00/0000 00:00");
    else
        strftime(destino, TAM_FECHA, "%d/%m/%Y %H:%M", fecha);
}

static void actualizarEstadisticas(tOperador *operador, unsigned nroRegistro, long puntaje)
{
    operador->jornadasJugadas++;
    operador->puntajeAcumulado += puntaje;
    if(operador->jornadasJugadas == 1 || puntaje > operador->mejorPuntaje)
        operador->mejorPuntaje = puntaje;
    if(!actualizarOperador(operador, nroRegistro))
        printf("\n   [aviso] No fue posible actualizar %s.\n", RUTA_OPERADORES);
}

int main()
{
    tArbolBinBusq indiceOperadores;
    tOperador operador;
    char nombre[TAM_NOMBRE_OPERADOR];
    unsigned nroRegistro = 0;

    crearArbolBinBusq(&indiceOperadores);
    if(cargarIndiceOperadores(&indiceOperadores))
        printf("\n   Indice de operadores cargado desde %s (%u operador/es).\n",
               RUTA_INDICE, contarArbolBinBusq(&indiceOperadores));
    else
        printf("\n   No existe un indice previo de operadores: se creara al salir.\n");

    if(!leerNombreOperador(nombre))
    {
        printf("\n   No se ingreso un nombre valido. Fin del programa.\n");
        vaciarArbolBinBusq(&indiceOperadores);
        return 1;
    }
    if(obtenerOperador(&indiceOperadores, nombre, &operador, &nroRegistro))
        printf("\n   Bienvenido nuevamente, %s.\n", operador.nombre);
    else
    {
        if(!altaOperador(&indiceOperadores, nombre, &operador, &nroRegistro))
        {
            printf("\n   [error] No fue posible dar de alta al operador. Fin del programa.\n");
            vaciarArbolBinBusq(&indiceOperadores);
            return 1;
        }
        printf("\n   Operador %s dado de alta correctamente.\n", operador.nombre);
    }

    if(!guardarIndiceOperadores(&indiceOperadores))
        printf("\n   [aviso] No fue posible persistir el indice en %s.\n", RUTA_INDICE);
    else
        printf("\n   Indice de operadores persistido en %s.\n", RUTA_INDICE);
    vaciarArbolBinBusq(&indiceOperadores);
    printf("\n   Hasta la proxima jornada.\n\n");

    return 0;
}

