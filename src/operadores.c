#include "../include/operadores.h"

typedef struct
{
    FILE *pf;
    tArbolBinBusq *ranking;
} tArmadoRanking;

int compararIndices(const void *d1, const void *d2)
{
    const tIndiceOperador *ind1 = d1, *ind2 = d2;

    return strcmp(ind1->nombre, ind2->nombre);
}

/* Mayor puntaje primero; a igual puntaje, por nombre. */
int compararRanking(const void *d1, const void *d2)
{
    const tOperador *op1 = d1, *op2 = d2;

    return op1->puntajeAcumulado != op2->puntajeAcumulado ?
           op2->puntajeAcumulado - op1->puntajeAcumulado : strcmp(op1->nombre, op2->nombre);
}

void grabarIndice(void *info, unsigned tamInfo, unsigned n, void *params)
{
    fwrite(info, tamInfo, 1, params);
}

/* El recorrido en orden deja el archivo ordenado por nombre, como lo
   necesita cargarArchivoBinOrdenadoArbolBinBusq. */
int guardarIndice(tArbolBinBusq *indice)
{
    FILE *pf = fopen(PATH_INDICE, "wb");

    if (!pf)
        return 0;
    recorrerEnOrdenArbolBinBusq(indice, pf, grabarIndice);
    return fclose(pf) == 0;
}

int ingresarOperador(tArbolBinBusq *indice, const char *nombre, unsigned *posicion)
{
    tIndiceOperador entrada;
    tOperador op;
    FILE *pf;
    int ok;

    strncpy(op.nombre, nombre, TAM_NOMBRE - 1);
    *(op.nombre + TAM_NOMBRE - 1) = '\0';
    op.puntajeAcumulado = 0;
    op.jornadas = 0;
    strcpy(entrada.nombre, op.nombre);
    if (buscarElemArbolBinBusq(indice, &entrada, sizeof(tIndiceOperador), compararIndices))
    {
        *posicion = entrada.posicion;
        return OPERADOR_EXISTENTE;
    }
    if (!(pf = fopen(PATH_OPERADORES, "ab")))
        return OPERADOR_ERROR;
    fseek(pf, 0L, SEEK_END);
    *posicion = entrada.posicion = ftell(pf) / sizeof(tOperador);
    ok = fwrite(&op, sizeof(tOperador), 1, pf) == 1;
    ok = fclose(pf) == 0 && ok;
    ok = ok && insertarArbolBinBusq(indice, &entrada, sizeof(tIndiceOperador), compararIndices) == TODO_BIEN;
    return ok && guardarIndice(indice) ? OPERADOR_NUEVO : OPERADOR_ERROR;
}

int registrarJornada(unsigned posicion, const char *nombre, const tEstadoSimulacion *e)
{
    tOperador op;
    tJornada jornada;
    FILE *pf = fopen(PATH_OPERADORES, "r+b");
    int ok = pf != NULL;

    if (ok)
    {
        fseek(pf, posicion * sizeof(tOperador), SEEK_SET);
        ok = fread(&op, sizeof(tOperador), 1, pf) == 1;
        op.puntajeAcumulado += e->puntaje;
        op.jornadas++;
        fseek(pf, posicion * sizeof(tOperador), SEEK_SET);
        ok = ok && fwrite(&op, sizeof(tOperador), 1, pf) == 1;
        ok = fclose(pf) == 0 && ok;
    }

    strncpy(jornada.operador, nombre, TAM_NOMBRE - 1);
    *(jornada.operador + TAM_NOMBRE - 1) = '\0';
    jornada.puntaje = e->puntaje;
    jornada.contenedoresEntregados = e->contenedoresEntregados;
    jornada.buquesDescargados = e->buquesDescargados;
    jornada.camionesPendientes = e->camionesPendientes;
    jornada.reorganizaciones = e->reorganizaciones;
    jornada.tiempoUtilizado = e->reloj;
    if (ok && (pf = fopen(PATH_JORNADAS, "ab")) != NULL)
        ok = (fwrite(&jornada, sizeof(tJornada), 1, pf) == 1) & (fclose(pf) == 0);
    else
        ok = 0;
    return ok;
}

/* Lee el registro al que apunta la entrada del indice y lo inserta en el
   arbol del ranking. */
void agregarAlRanking(void *info, unsigned tamInfo, unsigned n, void *params)
{
    tIndiceOperador *entrada = info;
    tArmadoRanking *armado = params;
    tOperador op;

    fseek(armado->pf, entrada->posicion * sizeof(tOperador), SEEK_SET);
    if (fread(&op, sizeof(tOperador), 1, armado->pf))
        insertarArbolBinBusq(armado->ranking, &op, sizeof(tOperador), compararRanking);
}

/* n del recorrido es el nivel del nodo, por eso la posicion se lleva aparte. */
void mostrarPosicionRanking(void *info, unsigned tamInfo, unsigned n, void *params)
{
    tOperador *op = info;
    unsigned *posicion = params;

    printf("  %s%3u.%s %-30s %s%6d puntos%s %s(%u jornadas)%s\n", *posicion < 3 ? AMARILLO NEGRITA : "",
           *posicion + 1, RESET, op->nombre, VERDE, op->puntajeAcumulado, RESET, GRIS, op->jornadas, RESET);
    (*posicion)++;
}

/* Accede a los operadores a traves del indice y los ordena por puntaje
   acumulado en un segundo arbol. */
void mostrarRanking(tArbolBinBusq *indice)
{
    tArbolBinBusq ranking;
    tArmadoRanking armado;
    unsigned posicion = 0;

    crearArbolBinBusq(&ranking);
    armado.ranking = &ranking;
    mostrarTitulo("RANKING DE OPERADORES", AMARILLO);
    if (!(armado.pf = fopen(PATH_OPERADORES, "rb")))
    {
        printf("  %sNo hay operadores registrados%s\n", GRIS, RESET);
        return;
    }
    recorrerEnOrdenArbolBinBusq(indice, &armado, agregarAlRanking);
    fclose(armado.pf);
    printf("\n");
    recorrerEnOrdenArbolBinBusq(&ranking, &posicion, mostrarPosicionRanking);
    vaciarArbolBinBusq(&ranking);
}
