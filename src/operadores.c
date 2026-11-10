#include "../include/operadores.h"

int compararIndices(const void *d1, const void *d2)
{
    const tIndiceOperador *ind1 = d1, *ind2 = d2;

    return strcmp(ind1->nombre, ind2->nombre);
}

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

int guardarIndice(tArbolBinBusq *indice)
{
    FILE *pf = fopen(PATH_INDICE, "wb");

    if (!pf)
        return 0;
    recorrerEnOrdenArbolBinBusq(indice, pf, grabarIndice);
    fclose(pf);
    return 1;
}

int ingresarOperador(tArbolBinBusq *indice, const char *nombre, unsigned *posicion)
{
    tIndiceOperador entrada;
    tOperador op;
    FILE *pf;

    memset(&op, 0, sizeof(tOperador));
    strncpy(op.nombre, nombre, TAM_NOMBRE - 1);
    memset(&entrada, 0, sizeof(tIndiceOperador));
    strcpy(entrada.nombre, op.nombre);
    if (buscarElemArbolBinBusq(indice, &entrada, sizeof(tIndiceOperador), compararIndices))
    {
        *posicion = entrada.posicion;
        return OPERARIO_EXISTENTE;
    }
    if (!(pf = fopen(PATH_OPERADORES, "ab")))
        return ERROR_OPERARIO;
    fseek(pf, 0L, SEEK_END);
    *posicion = entrada.posicion = ftell(pf) / sizeof(tOperador);
    fwrite(&op, sizeof(tOperador), 1, pf);
    fclose(pf);
    insertarArbolBinBusq(indice, &entrada, sizeof(tIndiceOperador), compararIndices);
    return guardarIndice(indice) ? NUEVO_OPERARIO : ERROR_OPERARIO;
}

int registrarJornada(unsigned posicion, const char *nombre, const tEstadoSimulacion *e)
{
    tOperador op;
    tJornada jornada;
    FILE *pf = fopen(PATH_OPERADORES, "r+b");

    if(!pf)
        return 0;
    fseek(pf, posicion * sizeof(tOperador), SEEK_SET);
    fread(&op, sizeof(tOperador), 1, pf);
    op.puntajeAcumulado += e->puntaje;
    op.jornadas++;
    fseek(pf, posicion * sizeof(tOperador), SEEK_SET);
    fwrite(&op, sizeof(tOperador), 1, pf);
    fclose(pf);
    pf = fopen(PATH_JORNADAS, "ab");
    if(!pf)
        return 0;
    memset(&jornada, 0, sizeof(tJornada));
    strncpy(jornada.operador, nombre, TAM_NOMBRE - 1);
    jornada.puntaje = e->puntaje;
    jornada.contenedoresEntregados = e->contenedoresEntregados;
    jornada.buquesDescargados = e->buquesDescargados;
    jornada.camionesPendientes = e->camionesPendientes;
    jornada.reorganizaciones = e->reorganizaciones;
    jornada.tiempoUtilizado = e->reloj;
    fwrite(&jornada, sizeof(tJornada), 1, pf);
    fclose(pf);
    return 1;
}

void agregarAlRanking(void *info, unsigned tamInfo, unsigned n, void *params)
{
    tIndiceOperador *entrada = info;
    tArmadoRanking *armado = params;
    tOperador op;

    fseek(armado->pf, entrada->posicion * sizeof(tOperador), SEEK_SET);
    if (fread(&op, sizeof(tOperador), 1, armado->pf))
        insertarArbolBinBusq(armado->ranking, &op, sizeof(tOperador), compararRanking);
}

void mostrarPosicionRanking(void *info, unsigned tamInfo, unsigned n, void *params)
{
    tOperador *op = info;
    unsigned *posicion = params;

    printf("  %s%3u.%s %-30s %s%6d puntos%s %s(%u jornadas)%s\n", *posicion < 3 ? AMARILLO NEGRITA : "",
           *posicion + 1, RESET, op->nombre, VERDE, op->puntajeAcumulado, RESET, GRIS, op->jornadas, RESET);
    (*posicion)++;
}

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

void normalizarNombreOperario(char* nombre)
{
    char *ini = nombre;
    char *fin;

    while (isspace((unsigned char)*ini))
    {
        ini++;
    }
    fin = ini + strlen(ini);
    while (fin > ini && isspace((unsigned char)*(fin - 1)))
    {
        fin--;
    }
    *fin = '\0';

    memmove(nombre, ini, fin - ini + 1);

    for (fin = nombre; *fin; fin++)
    {
        *fin = toupper((unsigned char)*fin);
    }

}
