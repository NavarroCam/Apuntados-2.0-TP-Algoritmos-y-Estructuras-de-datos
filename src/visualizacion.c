#include "../include/visualizacion.h"

void mostrarMenuPrincipal(const char *nombre, const char *aviso)
{
    limpiarPantalla();
    mostrarTitulo(TITULO, CIAN);
    printf("\n  Operador: %s%s%s\n\n", NEGRITA, nombre, RESET);
    mostrarBorde('-', GRIS);
    printf("    %s[1]%s Iniciar nueva jornada\n", AMARILLO, RESET);
    printf("    %s[2]%s Ver ranking de operadores\n", AMARILLO, RESET);
    printf("    %s[3]%s Salir\n", AMARILLO, RESET);
    mostrarBorde('-', GRIS);
    if (aviso)
        printf("  %s\n", aviso);
    printf("\n  %sOpcion:%s ", NEGRITA, RESET);
}

void mostrarEstado(tEstadoSimulacion *e)
{
    unsigned esperando = 0;

    verCola(&e->esperaBuques, sizeof(tBuque), &esperando, contarElementos);
    printf("\n");
    mostrarBorde('=', AZUL);
    printf("  %sT=%u%s de %u   |   puntaje provisorio: %s%d%s   |   buques esperando: %u\n",
           NEGRITA, e->reloj, RESET, e->config.duracionJornadaMinutos, VERDE, e->puntaje, RESET, esperando);
    mostrarBorde('=', AZUL);
    mostrarSeccion("MUELLES");
    map(&e->muelles, NULL, mostrarMuelle);
    mostrarSeccion("ZONAS (tope a la izquierda)");
    map(&e->zonas, NULL, mostrarZona);
    mostrarSeccion("CAMIONES");
    if (colaVacia(&e->colaCamiones))
        printf("  %s(sin camiones esperando)%s\n", GRIS, RESET);
    verCola(&e->colaCamiones, sizeof(tCamion), NULL, mostrarCamionVentana);
}

void mostrarMuelleBreve(void *info, unsigned tamInfo, unsigned n, void *params)
{
    tMuelle *muelle = info;
    tContenedor proximo;

    if (muelle->ocupado && verPrimeroCola(&muelle->buque.manifiesto, &proximo, sizeof(tContenedor)))
        printf(" | %sM%u%s: %s -> prox. %s%s%s", CIAN, muelle->id, RESET, muelle->buque.codigo, AMARILLO, proximo.codigo, RESET);
    else
        printf(" | %sM%u%s: %slibre%s", CIAN, muelle->id, RESET, GRIS, RESET);
}

void mostrarEstadoBreve(tEstadoSimulacion *e)
{
    tCamion frente;

    printf("\n  %sT=%u/%u%s", NEGRITA, e->reloj, e->config.duracionJornadaMinutos, RESET);
    map(&e->muelles, NULL, mostrarMuelleBreve);
    if (verPrimeroCola(&e->colaCamiones, &frente, sizeof(tCamion)))
        printf(" | Frente: %s pide %s%s%s\n", frente.codigo, AMARILLO, frente.contenedor, RESET);
    else
        printf(" | Frente: %ssin camiones%s\n", GRIS, RESET);
}

void mostrarResumen(tEstadoSimulacion *e, const char *motivo)
{
    printf("\n");

    mostrarTitulo("FIN DE LA JORNADA", MAGENTA);
    printf("  Motivo: %s%s%s\n\n", NEGRITA, motivo, RESET);
    mostrarSeccion("HISTORIAL DE OPERACIONES");
    map(&e->historial, NULL, mostrarRegistroOperacion);

    if (listaVacia(&e->historial))
        printf("  %s(sin operaciones)%s\n", GRIS, RESET);

    mostrarSeccion("RESUMEN");
    printf("  Tiempo utilizado ........... %u min de %u\n", e->reloj, e->config.duracionJornadaMinutos);
    printf("  Buques descargados ......... %u  %s(+%u puntos)%s\n", e->buquesDescargados, VERDE, PTS_BUQUE_DESC * e->buquesDescargados, RESET);
    printf("  Contenedores entregados .... %u  %s(+%u puntos)%s\n", e->contenedoresEntregados, VERDE,
           PTS_ENTR * e->contenedoresEntregados, RESET);
    printf("  Reorganizaciones ........... %u\n", e->reorganizaciones);
    printf("  Camiones pendientes ........ %u  %s(-%d puntos)%s\n", e->camionesPendientes, ROJO, PENAL_CAMION * e->camionesPendientes, RESET);
    mostrarBorde('-', GRIS);

    printf("  %sPUNTUACION FINAL: %d puntos%s\n", NEGRITA, e->puntaje, RESET);
    mostrarBorde('-', GRIS);
}
