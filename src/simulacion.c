#include "../include/simulacion.h"

void nuevaJornada(unsigned posicion, const char *nombre, const char* archConfig, const char* archPuerto)
{
    tConfig config;
    tEstadoSimulacion estado;

    mostrarTitulo("NUEVA JORNADA", AZUL);
    if (TEST)
        printf("%s  MODO TEST: se usa %s sin regenerarlo (solo se genera si no existe)%s\n",
               MAGENTA, archPuerto, RESET);
    if (cargarConfiguracion(&config, archConfig) != TODO_BIEN || !generarEscenario(archPuerto, &config, TEST, SEMILLA))
    {
        printf("%s  Error: %s no se pudo generar o supera los maximos, o no se pudo generar %s%s\n",
               ROJO, archConfig, archPuerto, RESET);
        return;
    }
    mostrarConfiguracion(&config);
    printf("\n");
    pausarPantalla();
    limpiarPantalla();
    mostrarTitulo("JORNADA EN CURSO", AZUL);
    crearEstadoSimulacion(&estado, &config);
    if (!cargarEscenario(archPuerto, &estado))
        printf("%s  Error: %s tiene un formato invalido%s\n", ROJO, archPuerto, RESET);
    else
    {
        jugarJornada(&estado);
        if (!registrarJornada(posicion, nombre, &estado))
            printf("%s  Error: no se pudo registrar la jornada%s\n", ROJO, RESET);
    }
    vaciarEstadoSimulacion(&estado);
}

int hayOperacionValida(tEstadoSimulacion *e, const tRecursos *r)
{
    tCamion camion;
    tContenedor pedido;

    if (hayTiempo(e, e->config.tiempoDescargaContenedor) && r->ocupados && r->conLugar)
        return 1;
    if (hayTiempo(e, e->config.tiempoReubicacionContenedor) && r->contenedores > 1 &&
        r->noVacias * r->conLugar > r->ambas)
        return 1;

    if (!hayTiempo(e, e->config.tiempoCargaCamion) || !verPrimeroCola(&e->colaCamiones, &camion, sizeof(tCamion)))
        return 0;

    strcpy(pedido.codigo, camion.contenedor);

    return aplicarEnElemLista(&e->zonas, &pedido, compararContenedorConTopeZona, NULL, NULL);
}

int finalizacionNormal(const tEstadoSimulacion *e, const tRecursos *r)
{
    return colaVacia(&e->arribosBuques) && colaVacia(&e->arribosCamiones) &&
           colaVacia(&e->esperaBuques) && colaVacia(&e->colaCamiones) && !r->ocupados;
}

unsigned proximoEvento(const tEstadoSimulacion *e)
{
    tBuque buque;
    tCamion camion;
    unsigned t = e->config.duracionJornadaMinutos;

    if (verPrimeroCola(&e->arribosBuques, &buque, sizeof(tBuque)))
        t = buque.llegada;
    if (verPrimeroCola(&e->arribosCamiones, &camion, sizeof(tCamion)) && camion.llegada < t)
        t = camion.llegada;
    return t;
}

void aplicarPenalizacion(tEstadoSimulacion *e)
{
    verCola(&e->colaCamiones, sizeof(tCamion), &e->camionesPendientes, contarElementos);
    e->puntaje -= (int)(PENAL_CAMION * e->camionesPendientes);
}

void jugarJornada(tEstadoSimulacion *e)
{
    char linea[TAM_LINEA];
    const char *motivo = NULL, *error;
    unsigned relojMostrado = e->config.duracionJornadaMinutos + 1;
    tRecursos r, sinRecursos = {0, 0, 0, 0, 0};

    printf("\n");
    mostrarSeccion("INICIO DE LA JORNADA - comandos: DES, REU, ENT, VER, ESP");
    avanzarReloj(e, 0);
    while (!motivo)
    {
        r = sinRecursos;
        map(&e->zonas, &r, contarZona);
        map(&e->muelles, &r, contarMuelle);
        if (finalizacionNormal(e, &r))
            motivo = "finalizacion normal";
        else if (e->reloj >= e->config.duracionJornadaMinutos)
            motivo = "se alcanzo la duracion maxima";
        else if (hayOperacionValida(e, &r))
        {
            if (e->reloj != relojMostrado)
                mostrarEstadoBreve(e);

            relojMostrado = e->reloj;
            printf("%s%sOPERADOR>%s ", NEGRITA, AZUL, RESET);
            if (!fgets(linea, sizeof(linea), stdin))
                motivo = "fin de la entrada";
            else if ((error = ejecutarOrden(e, linea)) != NULL)
                printf("  %sOrden rechazada: %s%s\n", ROJO, error, RESET);
        }
        else if (!colaVacia(&e->arribosBuques) || !colaVacia(&e->arribosCamiones))
        {
            printf("  %sSin operaciones validas en T=%u: avance automatico%s\n", GRIS, e->reloj, RESET);
            avanzarReloj(e, proximoEvento(e));
        }
        else
            motivo = "bloqueo operativo";
    }

    aplicarPenalizacion(e);
    mostrarResumen(e, motivo);
}
