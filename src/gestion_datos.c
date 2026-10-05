/* ============================================================
   src/gestion_datos.c
   Persistencia de operadores (con indice ABB) y de jornadas.
   ============================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/gestion_datos.h"

#define TODO_BIEN 1
#define FALLO     0

/* ---------- parametros auxiliares para recorridos ---------- */
typedef struct
{
    FILE *archivo;
    int error;
} tParamEscritura;

typedef struct
{
    unsigned posicion;
} tParamRanking;

/* ------------------------------------------------------------
   Utilidades
   ------------------------------------------------------------ */
void normalizarNombreOperador(char *destino, const char *origen)
{
    unsigned i = 0;
    unsigned j = 0;

    while(origen[i] != '\0' && isspace((unsigned char)origen[i]))
        i++;
    while(origen[i] != '\0' && j < TAM_NOMBRE_OPERADOR - 1)
    {
        destino[j] = (char)toupper((unsigned char)origen[i]);
        i++;
        j++;
    }
    while(j > 0 && isspace((unsigned char)destino[j - 1]))
        j--;
    destino[j] = '\0';
}

static int compararIndicePorNombre(const void *a, const void *b)
{
    const tIndiceOperador *indiceA = (const tIndiceOperador *)a;
    const tIndiceOperador *indiceB = (const tIndiceOperador *)b;

    return strcmp(indiceA->nombre, indiceB->nombre);
}

/* Ordena el ranking: mayor puntaje primero; a igual puntaje, por nombre. */
static int compararOperadoresPorPuntaje(const void *a, const void *b)
{
    const tOperador *opA = (const tOperador *)a;
    const tOperador *opB = (const tOperador *)b;

    if(opA->puntajeAcumulado > opB->puntajeAcumulado)
        return -1;
    if(opA->puntajeAcumulado < opB->puntajeAcumulado)
        return 1;
    return strcmp(opA->nombre, opB->nombre);
}

/* Abre un archivo binario para lectura/escritura, creandolo si no existe.
   No se crean carpetas: la estructura de directorios debe existir. */
static FILE *abrirBinarioActualizacion(const char *ruta)
{
    FILE *archivo;

    archivo = fopen(ruta, "r+b");
    if(archivo == NULL)
        archivo = fopen(ruta, "w+b");
    return archivo;
}

static long cantidadRegistros(const char *ruta, unsigned tamRegistro)
{
    FILE *archivo;
    long bytes;

    archivo = fopen(ruta, "rb");
    if(archivo == NULL)
        return 0;
    fseek(archivo, 0L, SEEK_END);
    bytes = ftell(archivo);
    fclose(archivo);
    if(bytes <= 0)
        return 0;
    return bytes / (long)tamRegistro;
}

/* ------------------------------------------------------------
   Indice ABB de operadores
   ------------------------------------------------------------ */

/* Lectura generica utilizada por cargarDesdeDatosOrdenadosArbolBinBusq. */
static unsigned leerIndiceDesdeArchivo(void **dato, void *fuente, unsigned pos, void *params)
{
    unsigned tamRegistro = *(unsigned *)params;

    *dato = malloc(tamRegistro);
    if(*dato == NULL)
        return 0;
    if(fseek((FILE *)fuente, (long)pos * (long)tamRegistro, SEEK_SET) != 0)
        return 0;
    if(fread(*dato, tamRegistro, 1, (FILE *)fuente) != 1)
        return 0;
    return tamRegistro;
}

static void accionEscribirIndice(void *info, unsigned tamInfo, void *params)
{
    tParamEscritura *parametros = (tParamEscritura *)params;

    if(!parametros->error)
        if(fwrite(info, tamInfo, 1, parametros->archivo) != 1)
            parametros->error = 1;
}

int cargarIndiceOperadores(tArbolBinBusq *indice)
{
    FILE *archivo;
    unsigned tamRegistro = (unsigned)sizeof(tIndiceOperador);
    long cantidad;
    int resultado;

    crearArbolBinBusq(indice);
    cantidad = cantidadRegistros(RUTA_INDICE, tamRegistro);
    if(cantidad <= 0)
        return 0;
    archivo = fopen(RUTA_INDICE, "rb");
    if(archivo == NULL)
        return 0;
    resultado = cargarDesdeDatosOrdenadosArbolBinBusq(indice, archivo, leerIndiceDesdeArchivo,
                                                      0, (int)cantidad - 1, &tamRegistro);
    fclose(archivo);
    if(resultado != TODO_BIEN)
    {
        vaciarArbolBinBusq(indice);
        return 0;
    }
    return 1;
}

int guardarIndiceOperadores(const tArbolBinBusq *indice)
{
    tParamEscritura parametros;

    parametros.archivo = fopen(RUTA_INDICE, "wb");
    parametros.error = 0;
    if(parametros.archivo == NULL)
        return FALLO;
    recorrerEnOrdenArbolBinBusq(indice, accionEscribirIndice, &parametros);
    fclose(parametros.archivo);
    return parametros.error ? FALLO : TODO_BIEN;
}

/* ------------------------------------------------------------
   Operadores
   ------------------------------------------------------------ */
static int leerOperador(tOperador *operador, unsigned nroRegistro)
{
    FILE *archivo;
    int leido;

    archivo = fopen(RUTA_OPERADORES, "rb");
    if(archivo == NULL)
        return FALLO;
    if(fseek(archivo, (long)nroRegistro * (long)sizeof(tOperador), SEEK_SET) != 0)
    {
        fclose(archivo);
        return FALLO;
    }
    leido = (fread(operador, sizeof(tOperador), 1, archivo) == 1);
    fclose(archivo);
    return leido ? TODO_BIEN : FALLO;
}

int obtenerOperador(const tArbolBinBusq *indice, const char *nombre,
                    tOperador *operador, unsigned *nroRegistro)
{
    tIndiceOperador clave;

    memset(&clave, 0, sizeof(tIndiceOperador));
    normalizarNombreOperador(clave.nombre, nombre);
    if(!buscarElemArbolBinBusq(indice, &clave, sizeof(tIndiceOperador), compararIndicePorNombre))
        return FALLO;
    *nroRegistro = clave.nroRegistro;
    return leerOperador(operador, clave.nroRegistro);
}

int altaOperador(tArbolBinBusq *indice, const char *nombre,
                 tOperador *operador, unsigned *nroRegistro)
{
    FILE *archivo;
    tIndiceOperador clave;
    long posicion;

    archivo = abrirBinarioActualizacion(RUTA_OPERADORES);
    if(archivo == NULL)
        return FALLO;
    fseek(archivo, 0L, SEEK_END);
    posicion = ftell(archivo) / (long)sizeof(tOperador);

    memset(operador, 0, sizeof(tOperador));
    normalizarNombreOperador(operador->nombre, nombre);
    operador->jornadasJugadas = 0;
    operador->puntajeAcumulado = 0;
    operador->mejorPuntaje = 0;

    if(fwrite(operador, sizeof(tOperador), 1, archivo) != 1)
    {
        fclose(archivo);
        return FALLO;
    }
    fclose(archivo);

    memset(&clave, 0, sizeof(tIndiceOperador));
    strcpy(clave.nombre, operador->nombre);
    clave.nroRegistro = (unsigned)posicion;
    if(insertarArbolBinBusq(indice, &clave, sizeof(tIndiceOperador), compararIndicePorNombre) != TODO_BIEN)
        return FALLO;
    *nroRegistro = (unsigned)posicion;
    return TODO_BIEN;
}

int actualizarOperador(const tOperador *operador, unsigned nroRegistro)
{
    FILE *archivo;
    int escrito;

    archivo = abrirBinarioActualizacion(RUTA_OPERADORES);
    if(archivo == NULL)
        return FALLO;
    if(fseek(archivo, (long)nroRegistro * (long)sizeof(tOperador), SEEK_SET) != 0)
    {
        fclose(archivo);
        return FALLO;
    }
    escrito = (fwrite(operador, sizeof(tOperador), 1, archivo) == 1);
    fclose(archivo);
    return escrito ? TODO_BIEN : FALLO;
}

/* ------------------------------------------------------------
   Jornadas
   ------------------------------------------------------------ */
static void accionEscribirOperacion(void *info, unsigned tamInfo, void *params)
{
    tParamEscritura *parametros = (tParamEscritura *)params;

    if(!parametros->error)
        if(fwrite(info, tamInfo, 1, parametros->archivo) != 1)
            parametros->error = 1;
}

int registrarJornada(const tJornada *jornada, tLista *historialOperaciones)
{
    tParamEscritura parametros;

    parametros.archivo = abrirBinarioActualizacion(RUTA_JORNADAS);
    parametros.error = 0;
    if(parametros.archivo == NULL)
        return FALLO;
    fseek(parametros.archivo, 0L, SEEK_END);
    if(fwrite(jornada, sizeof(tJornada), 1, parametros.archivo) != 1)
    {
        fclose(parametros.archivo);
        return FALLO;
    }
    recorrerLista(historialOperaciones, accionEscribirOperacion, &parametros);
    fclose(parametros.archivo);
    return parametros.error ? FALLO : TODO_BIEN;
}

/* ------------------------------------------------------------
   Consultas
   ------------------------------------------------------------ */
static void accionMostrarRanking(void *info, unsigned tamInfo, void *params)
{
    tOperador *operador = (tOperador *)info;
    tParamRanking *parametros = (tParamRanking *)params;

    (void)tamInfo;
    parametros->posicion++;
    printf("   %3u  %-30s %8ld %10u %10ld\n",
           parametros->posicion, operador->nombre, operador->puntajeAcumulado,
           operador->jornadasJugadas, operador->mejorPuntaje);
}

void mostrarRankingOperadores(void)
{
    FILE *archivo;
    tLista ranking;
    tOperador operador;
    tParamRanking parametros;
    unsigned cantidad = 0;

    crearLista(&ranking);
    archivo = fopen(RUTA_OPERADORES, "rb");
    if(archivo == NULL)
    {
        printf("\n   Todavia no hay operadores registrados.\n");
        return;
    }
    while(fread(&operador, sizeof(tOperador), 1, archivo) == 1)
    {
        if(insertarEnOrden(&ranking, &operador, sizeof(tOperador), compararOperadoresPorPuntaje))
            cantidad++;
    }
    fclose(archivo);

    printf("\n   ==================================================================\n");
    printf("                    RANKING DE OPERADORES DEL PUERTO\n");
    printf("   ==================================================================\n");
    printf("   Pos  Operador                          Puntaje   Jornadas     Mejor\n");
    printf("   ------------------------------------------------------------------\n");
    if(cantidad == 0)
        printf("   (sin registros)\n");
    else
    {
        parametros.posicion = 0;
        recorrerLista(&ranking, accionMostrarRanking, &parametros);
    }
    printf("   ==================================================================\n");
    vaciarLista(&ranking);
}

void mostrarHistorialJornadas(const char *nombreOperador)
{
    FILE *archivo;
    tJornada jornada;
    char nombreNormalizado[TAM_NOMBRE_OPERADOR];
    unsigned encontradas = 0;
    long salto;
    int seguir;

    normalizarNombreOperador(nombreNormalizado, nombreOperador);
    archivo = fopen(RUTA_JORNADAS, "rb");
    if(archivo == NULL)
    {
        printf("\n   Todavia no hay jornadas registradas.\n");
        return;
    }
    printf("\n   ==================================================================\n");
    printf("      JORNADAS REGISTRADAS DE %s\n", nombreNormalizado);
    printf("   ==================================================================\n");
    printf("   Fecha                 Puntaje  Entreg.  Buques  Pend.  Reub.  Tiempo\n");
    printf("   ------------------------------------------------------------------\n");
    seguir = 1;
    while(seguir)
    {
        if(fread(&jornada, sizeof(tJornada), 1, archivo) != 1)
            seguir = 0;
        else
        {
            if(strcmp(jornada.nombreOperador, nombreNormalizado) == 0)
            {
                encontradas++;
                printf("   %-19s %8ld %8u %7u %6u %6u %7u\n",
                       jornada.fecha, jornada.puntaje, jornada.contenedoresEntregados,
                       jornada.buquesDescargados, jornada.camionesPendientes,
                       jornada.reubicaciones, jornada.tiempoUtilizado);
            }
            salto = (long)jornada.cantidadOperaciones * (long)jornada.tamRegistroOperacion;
            if(fseek(archivo, salto, SEEK_CUR) != 0)
                seguir = 0;
        }
    }
    fclose(archivo);
    if(encontradas == 0)
        printf("   (este operador todavia no completo ninguna jornada)\n");
    printf("   ==================================================================\n");
}
