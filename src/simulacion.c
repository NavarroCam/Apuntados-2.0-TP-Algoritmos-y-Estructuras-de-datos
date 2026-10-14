#include "../include/simulacion.h"

#define TAM_DESCRIPCION     32
#define SIN_TIEMPO          "no alcanza el tiempo restante de la jornada"
#define SIN_MEMORIA         "memoria insuficiente"
#define SIN_PARAMETROS      "la operacion no lleva parametros"

/* Lleva el reloj a "hasta": pasa a espera los arribos con T <= hasta en orden
   cronologico y asigna los muelles libres en orden creciente. */
void avanzarReloj(tEstadoSimulacion *e, unsigned hasta)
{
    tBuque buque;
    tCamion camion;

    e->reloj = hasta;
    while (verPrimeroCola(&e->arribosBuques, &buque, sizeof(tBuque)) && buque.llegada <= hasta)
    {
        printf("  [T=%u] arriba %s\n", buque.llegada, buque.codigo);
        sacarDeCola(&e->arribosBuques, &buque, sizeof(tBuque));
        ponerEnCola(&e->esperaBuques, &buque, sizeof(tBuque));
    }
    while (verPrimeroCola(&e->arribosCamiones, &camion, sizeof(tCamion)) && camion.llegada <= hasta)
    {
        printf("  [T=%u] arriba %s (solicita %s)\n", camion.llegada, camion.codigo, camion.contenedor);
        sacarDeCola(&e->arribosCamiones, &camion, sizeof(tCamion));
        ponerEnCola(&e->colaCamiones, &camion, sizeof(tCamion));
    }
    _map(&e->muelles, &e->esperaBuques, asignarMuelle);
}

int hayTiempo(const tEstadoSimulacion *e, unsigned duracion)
{
    return e->reloj + duracion <= e->config.duracionJornadaMinutos;
}

/* Registra la operacion y avanza el reloj hasta su fin. La descripcion trae
   "OPERACION CONTENEDOR ORIGEN DESTINO", por ejemplo "DES C101 M1 Z1". Las
   consecuencias directas ya fueron aplicadas por quien la llama. */
void completarOperacion(tEstadoSimulacion *e, unsigned duracion, const char *descripcion)
{
    tRegistroOperacion reg;

    reg.inicio = e->reloj;
    reg.fin = e->reloj + duracion;
    /* %3s y %7s corresponden a TAM_OPERACION - 1 y TAM_CODIGO - 1 */
    sscanf(descripcion, "%3s %7s %7s %7s", reg.operacion, reg.contenedor, reg.origen, reg.destino);
    ponerAlFinal(&e->historial, &reg, sizeof(tRegistroOperacion));
    avanzarReloj(e, reg.fin);
}

/* Las validaciones se hacen sobre copias; los cambios, con acciones sobre
   los elementos de las listas. */
const char *descargar(tEstadoSimulacion *e, unsigned idMuelle, unsigned idZona)
{
    tMuelle muelle;
    tZona zona;
    tMovimiento mov;
    char descripcion[TAM_DESCRIPCION];

    muelle.id = idMuelle;
    zona.id = idZona;
    if (!buscarElemLista(&e->muelles, &muelle, sizeof(tMuelle), compararId))
        return "el muelle no existe";
    if (!buscarElemLista(&e->zonas, &zona, sizeof(tZona), compararId))
        return "la zona no existe";
    if (!muelle.ocupado)
        return "el muelle no tiene un buque con carga pendiente";
    if (zona.cantidad >= zona.capacidad)
        return "la zona no tiene capacidad disponible";
    if (!hayTiempo(e, e->config.tiempoDescargaContenedor))
        return SIN_TIEMPO;
    verPrimeroCola(&muelle.buque.manifiesto, &mov.cont, sizeof(tContenedor));
    aplicarEnElemLista(&e->zonas, &zona, compararId, &mov, apilarEnZona);
    if (!mov.ok)
        return SIN_MEMORIA;

    aplicarEnElemLista(&e->muelles, &muelle, compararId, &mov, descargarDelMuelle);
    printf("  Descarga de %s desde %s (M%u) hacia Z%u\n", mov.cont.codigo, muelle.buque.codigo, idMuelle, idZona);
    if (mov.ok)
    {
        printf("  %s completa su descarga (+5 puntos) y libera M%u\n", muelle.buque.codigo, idMuelle);
        e->puntaje += 5;
        e->buquesDescargados++;
    }
    sprintf(descripcion, "DES %s M%u Z%u", mov.cont.codigo, idMuelle, idZona);
    completarOperacion(e, e->config.tiempoDescargaContenedor, descripcion);
    return NULL;
}

const char *reubicar(tEstadoSimulacion *e, unsigned idOrigen, unsigned idDestino)
{
    tZona origen, destino;
    tMovimiento mov;
    char descripcion[TAM_DESCRIPCION];

    origen.id = idOrigen;
    destino.id = idDestino;
    if (!buscarElemLista(&e->zonas, &origen, sizeof(tZona), compararId))
        return "la zona de origen no existe";
    if (!buscarElemLista(&e->zonas, &destino, sizeof(tZona), compararId))
        return "la zona de destino no existe";
    if (!origen.cantidad)
        return "la zona de origen esta vacia";
    if (idOrigen == idDestino)
        return "las zonas de origen y destino deben ser diferentes";
    if (destino.cantidad >= destino.capacidad)
        return "la zona de destino no tiene capacidad disponible";
    if (!hayTiempo(e, e->config.tiempoReubicacionContenedor))
        return SIN_TIEMPO;
    verTope(&origen.pila, &mov.cont, sizeof(tContenedor));
    aplicarEnElemLista(&e->zonas, &destino, compararId, &mov, apilarEnZona);
    if (!mov.ok)
        return SIN_MEMORIA;

    aplicarEnElemLista(&e->zonas, &origen, compararId, &mov, desapilarDeZona);
    e->reorganizaciones++;
    printf("  Reubicacion de %s desde Z%u hacia Z%u\n", mov.cont.codigo, idOrigen, idDestino);
    sprintf(descripcion, "REU %s Z%u Z%u", mov.cont.codigo, idOrigen, idDestino);
    completarOperacion(e, e->config.tiempoReubicacionContenedor, descripcion);
    return NULL;
}

const char *entregar(tEstadoSimulacion *e)
{
    tCamion camion;
    tMovimiento mov;
    char descripcion[TAM_DESCRIPCION];

    if (!verPrimeroCola(&e->colaCamiones, &camion, sizeof(tCamion)))
        return "no hay camiones esperando";
    strcpy(mov.cont.codigo, camion.contenedor);
    if (!aplicarEnElemLista(&e->zonas, &mov.cont, compararContenedorConTopeZona, NULL, NULL))
        return "el contenedor solicitado no esta en el tope de ninguna zona";
    if (!hayTiempo(e, e->config.tiempoCargaCamion))
        return SIN_TIEMPO;

    aplicarEnElemLista(&e->zonas, &mov.cont, compararContenedorConTopeZona, &mov, desapilarDeZona);
    sacarDeCola(&e->colaCamiones, &camion, sizeof(tCamion));
    e->puntaje += 10;
    e->contenedoresEntregados++;
    printf("  %s retira %s desde Z%u (+10 puntos) y abandona el puerto\n", camion.codigo, mov.cont.codigo, mov.idZona);
    sprintf(descripcion, "ENT %s Z%u %s", mov.cont.codigo, mov.idZona, camion.codigo);
    completarOperacion(e, e->config.tiempoCargaCamion, descripcion);
    return NULL;
}

void mostrarEstado(tEstadoSimulacion *e)
{
    unsigned esperando = 0;

    verCola(&e->esperaBuques, sizeof(tBuque), &esperando, contarElementos);
    printf("\nT=%u de %u | puntaje provisorio: %d | buques esperando muelle: %u\n",
           e->reloj, e->config.duracionJornadaMinutos, e->puntaje, esperando);
    _map(&e->muelles, NULL, mostrarMuelle);
    _map(&e->zonas, NULL, mostrarZona);
    printf("  Cola de camiones:%s\n", colaVacia(&e->colaCamiones) ? " vacia" : "");
    verCola(&e->colaCamiones, sizeof(tCamion), NULL, mostrarCamionVentana);
}

const char *ejecutarOrden(tEstadoSimulacion *e, char *linea)
{
    char operacion[TAM_LINEA], recurso1, recurso2, extra;
    unsigned id1, id2;
    int leidos;
    char *c;

    for (c = linea; *c; c++)
        *c = toupper((unsigned char)*c);
    leidos = sscanf(linea, "%s %c%u %c%u %c", operacion, &recurso1, &id1, &recurso2, &id2, &extra);
    if (leidos < 1 || strlen(operacion) != 3)
        return "el codigo de operacion debe tener exactamente tres caracteres";
    if (!strcmp(operacion, "DES"))
        return leidos == 5 && recurso1 == 'M' && recurso2 == 'Z' ? descargar(e, id1, id2) :
               "sintaxis: DES <muelle> <zona>, por ejemplo DES M1 Z1";
    if (!strcmp(operacion, "REU"))
        return leidos == 5 && recurso1 == 'Z' && recurso2 == 'Z' ? reubicar(e, id1, id2) :
               "sintaxis: REU <zona_origen> <zona_destino>, por ejemplo REU Z1 Z2";
    if (!strcmp(operacion, "ENT"))
        return leidos == 1 ? entregar(e) : SIN_PARAMETROS;
    if (!strcmp(operacion, "ESP"))
    {
        if (leidos != 1 || !hayTiempo(e, 1))
            return leidos != 1 ? SIN_PARAMETROS : SIN_TIEMPO;
        completarOperacion(e, 1, "ESP . . .");
        return NULL;
    }
    if (!strcmp(operacion, "VER"))
    {
        if (leidos == 1)
            mostrarEstado(e);
        return leidos == 1 ? NULL : SIN_PARAMETROS;
    }
    return "codigo de operacion inexistente (DES, REU, ENT, VER o ESP)";
}

int hayOperacionValida(tEstadoSimulacion *e, const tRecursos *r)
{
    tCamion camion;
    tContenedor pedido;

    if (hayTiempo(e, e->config.tiempoDescargaContenedor) && r->ocupados && r->conLugar)
        return 1;
    /* pares (origen no vacia, destino con lugar) con origen distinto de destino.
       Agregado del equipo: con un unico contenedor almacenado REU no cuenta,
       porque moverlo de zona no cambia nada. */
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

void mostrarResumen(tEstadoSimulacion *e, const char *motivo)
{
    int penalizacion;

    verCola(&e->colaCamiones, sizeof(tCamion), &e->camionesPendientes, contarElementos);
    penalizacion = 2 * e->camionesPendientes;
    e->puntaje -= penalizacion;
    printf("\n== FIN DE LA JORNADA: %s ==\n\nHistorial de operaciones:\n", motivo);
    _map(&e->historial, NULL, mostrarRegistroOperacion);
    printf("\nRESUMEN\n  Tiempo utilizado: %u min de %u\n", e->reloj, e->config.duracionJornadaMinutos);
    printf("  Buques descargados: %u -> %u puntos\n", e->buquesDescargados, 5 * e->buquesDescargados);
    printf("  Contenedores entregados: %u -> %u puntos\n", e->contenedoresEntregados, 10 * e->contenedoresEntregados);
    printf("  Reorganizaciones: %u\n", e->reorganizaciones);
    printf("  Camiones pendientes: %u -> %d puntos de penalizacion\n", e->camionesPendientes, penalizacion);
    printf("  Puntuacion final: %d puntos\n", e->puntaje);
}

/* En cada vuelta se cuentan zonas y muelles una sola vez y con eso se evalua
   primero la finalizacion normal y despues si hay una operacion valida. */
void jugarJornada(tEstadoSimulacion *e)
{
    char linea[TAM_LINEA];
    const char *motivo = NULL, *error;
    unsigned relojMostrado = e->config.duracionJornadaMinutos + 1;
    tRecursos r, sinRecursos = {0, 0, 0, 0, 0};

    printf("\n== INICIO DE LA JORNADA (comandos: DES, REU, ENT, VER, ESP) ==\n");
    avanzarReloj(e, 0);
    while (!motivo)
    {
        r = sinRecursos;
        _map(&e->zonas, &r, contarZona);
        _map(&e->muelles, &r, contarMuelle);
        if (finalizacionNormal(e, &r))
            motivo = "finalizacion normal";
        else if (e->reloj >= e->config.duracionJornadaMinutos)
            motivo = "se alcanzo la duracion maxima";
        else if (hayOperacionValida(e, &r))
        {
            if (e->reloj != relojMostrado)
                mostrarEstado(e);
            relojMostrado = e->reloj;
            printf("OPERADOR> ");
            if (!fgets(linea, sizeof(linea), stdin))
                motivo = "fin de la entrada";
            else if ((error = ejecutarOrden(e, linea)) != NULL)
                printf("  Orden rechazada: %s\n", error);
        }
        else if (!colaVacia(&e->arribosBuques) || !colaVacia(&e->arribosCamiones))
        {
            printf("  Sin operaciones validas en T=%u: avance automatico\n", e->reloj);
            avanzarReloj(e, proximoEvento(e));
        }
        else
            motivo = "bloqueo operativo";
    }
    mostrarResumen(e, motivo);
}
