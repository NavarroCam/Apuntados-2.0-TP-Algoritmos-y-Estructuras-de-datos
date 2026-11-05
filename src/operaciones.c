#include "../include/operaciones.h"
void avanzarReloj(tEstadoSimulacion *e, unsigned hasta)
{
    tBuque buque;
    tCamion camion;

    e->reloj = hasta;
    while (verPrimeroCola(&e->arribosBuques, &buque, sizeof(tBuque)) && buque.llegada <= hasta)
    {
        printf("  %s[T=%u]%s arriba el buque %s%s%s\n", GRIS, buque.llegada, RESET, NEGRITA, buque.codigo, RESET);
        sacarDeCola(&e->arribosBuques, &buque, sizeof(tBuque));
        ponerEnCola(&e->esperaBuques, &buque, sizeof(tBuque));
    }
    while (verPrimeroCola(&e->arribosCamiones, &camion, sizeof(tCamion)) && camion.llegada <= hasta)
    {
        printf("  %s[T=%u]%s arriba el camion %s%s%s (solicita %s%s%s)\n", GRIS, camion.llegada, RESET,
               NEGRITA, camion.codigo, RESET, AMARILLO, camion.contenedor, RESET);
        sacarDeCola(&e->arribosCamiones, &camion, sizeof(tCamion));
        ponerEnCola(&e->colaCamiones, &camion, sizeof(tCamion));
    }
    map(&e->muelles, &e->esperaBuques, asignarMuelle);
}

int hayTiempo(const tEstadoSimulacion *e, unsigned duracion)
{
    return e->reloj + duracion <= e->config.duracionJornadaMinutos;
}

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
    printf("  Descarga de %s%s%s desde %s (M%u) hacia Z%u\n", AMARILLO, mov.cont.codigo, RESET,
           muelle.buque.codigo, idMuelle, idZona);
    if (mov.ok)
    {
        printf("  %s%s completa su descarga (+%d puntos) y libera M%u%s\n", VERDE, muelle.buque.codigo, PTS_BUQUE_DESC, idMuelle, RESET);
        e->puntaje += PTS_BUQUE_DESC;
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
    printf("  Reubicacion de %s%s%s desde Z%u hacia Z%u\n", AMARILLO, mov.cont.codigo, RESET, idOrigen, idDestino);
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
    e->puntaje += PTS_ENTR;
    e->contenedoresEntregados++;
    printf("  %s%s retira %s desde Z%u (+%d puntos) y abandona el puerto%s\n", VERDE, camion.codigo,
           mov.cont.codigo, mov.idZona, PTS_ENTR, RESET);
    sprintf(descripcion, "ENT %s Z%u %s", mov.cont.codigo, mov.idZona, camion.codigo);
    completarOperacion(e, e->config.tiempoCargaCamion, descripcion);

    return NULL;
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
