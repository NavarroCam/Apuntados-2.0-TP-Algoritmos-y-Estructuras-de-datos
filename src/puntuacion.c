#include "../include/puntuacion.h"

void iniciarPuntuacion(tPuntuacion *p, unsigned duracionJornada)
{
    p->contenedoresEntregados = 0;
    p->buquesDescargados = 0;
    p->camionesPendientes = 0;
    p->reorganizaciones = 0;
    p->tiempoUtilizado = 0;
    p->duracionJornada = duracionJornada;
}

void registrarEntrega(tPuntuacion *p)
{
    p->contenedoresEntregados++;
}

void registrarBuqueDescargado(tPuntuacion *p)
{
    p->buquesDescargados++;
}

void registrarReorganizacion(tPuntuacion *p)
{
    p->reorganizaciones++;
}

void cerrarPuntuacion(tPuntuacion *p, unsigned tiempoUtilizado, unsigned camionesPendientes)
{
    p->tiempoUtilizado = tiempoUtilizado;
    p->camionesPendientes = camionesPendientes;
}

int calcularPuntajeParcial(const tPuntuacion *p)
{
    return (int)(p->contenedoresEntregados * PUNTOS_POR_ENTREGA + p->buquesDescargados * PUNTOS_POR_BUQUE_DESCARGADO);
}

int calcularPuntajeFinal(const tPuntuacion *p)
{
    return calcularPuntajeParcial(p) - (int)(p->camionesPendientes * PENALIZACION_POR_CAMION);
}

void mostrarResumenFinal(const tPuntuacion *p, FILE *pf)
{
    fprintf(pf, "\n  RESUMEN DE LA JORNADA\n");
    fprintf(pf, "  ---------------------------------------------------------\n");
    fprintf(pf, "  Tiempo utilizado ............ %u min de %u\n",
            p->tiempoUtilizado, p->duracionJornada);
    fprintf(pf, "  Buques descargados .......... %u -> %u puntos\n", p->buquesDescargados, p->buquesDescargados * PUNTOS_POR_BUQUE_DESCARGADO);

    fprintf(pf, "  Contenedores entregados ..... %u -> %u puntos\n", p->contenedoresEntregados, p->contenedoresEntregados * PUNTOS_POR_ENTREGA);

    fprintf(pf, "  Reorganizaciones ............ %u\n", p->reorganizaciones);

    fprintf(pf, "  Camiones pendientes ......... %u -> %u puntos de penalizacion\n", p->camionesPendientes, p->camionesPendientes * PENALIZACION_POR_CAMION);
    fprintf(pf, "  ---------------------------------------------------------\n");

    fprintf(pf, "  PUNTUACION FINAL: %d puntos\n", calcularPuntajeFinal(p));
}
