#include "../include/puerto.h"

void crearZona(tZona *zona, unsigned id, unsigned capacidad)
{
    zona->id = id;
    zona->capacidad = capacidad;
    zona->cantidad = 0;
    crearPila(&zona->pila);
}

void crearMuelle(tMuelle *muelle, unsigned id)
{
    muelle->id = id;
    muelle->ocupado = 0;
    strcpy(muelle->buque.codigo, RECURSO_VACIO);
    crearCola(&muelle->buque.manifiesto);
}

void crearEstadoSimulacion(tEstadoSimulacion *estado, const tConfig *config)
{
    estado->config = *config;
    estado->reloj = 0;
    estado->puntaje = 0;
    estado->contenedoresEntregados = 0;
    estado->buquesDescargados = 0;
    estado->reorganizaciones = 0;
    estado->camionesPendientes = 0;
    crearCola(&estado->arribosBuques);
    crearCola(&estado->arribosCamiones);
    crearCola(&estado->esperaBuques);
    crearCola(&estado->colaCamiones);
    crearLista(&estado->muelles);
    crearLista(&estado->zonas);
    crearLista(&estado->historial);
}

void vaciarBuque(void *info, unsigned tamInfo, unsigned n, void *params)
{
    tBuque *buque = info;

    vaciarCola(&buque->manifiesto);
}

void vaciarMuelle(void *info, unsigned tamInfo, unsigned n, void *params)
{
    tMuelle *muelle = info;

    vaciarCola(&muelle->buque.manifiesto);
}

void vaciarZona(void *info, unsigned tamInfo, unsigned n, void *params)
{
    tZona *zona = info;

    vaciarPila(&zona->pila);
}

void vaciarEstadoSimulacion(tEstadoSimulacion *estado)
{
    verCola(&estado->arribosBuques, sizeof(tBuque), NULL, vaciarBuque);
    verCola(&estado->esperaBuques, sizeof(tBuque), NULL, vaciarBuque);
    vaciarCola(&estado->arribosBuques);
    vaciarCola(&estado->esperaBuques);
    vaciarCola(&estado->arribosCamiones);
    vaciarCola(&estado->colaCamiones);
    map(&estado->muelles, NULL, vaciarMuelle);
    map(&estado->zonas, NULL, vaciarZona);
    vaciarLista(&estado->muelles);
    vaciarLista(&estado->zonas);
    vaciarLista(&estado->historial);
}

int compararId(const void *d1, const void *d2)
{
    const unsigned *id1 = d1, *id2 = d2;

    return (*id1 > *id2) - (*id1 < *id2);
}

/* Clave y elemento son de tipos distintos: usar solo con aplicarEnElemLista. */
int compararContenedorConTopeZona(const void *contenedor, const void *zona)
{
    const tContenedor *buscado = contenedor;
    const tZona *z = zona;
    tContenedor tope;

    if (!verTope(&z->pila, &tope, sizeof(tContenedor)))
        return 1;
    return strcmp(buscado->codigo, tope.codigo);
}

/* Aplica la accion a una copia de cada elemento sin alterar la cola: cada
   elemento pasa a una cola auxiliar y despues vuelve, en el mismo orden. */
void verCola(tCola *cola, unsigned tamInfo, void *params, Accion accion)
{
    tCola aux;
    void *dato = malloc(tamInfo);
    unsigned n = 0;

    crearCola(&aux);
    while (dato && sacarDeCola(cola, dato, tamInfo))
    {
        accion(dato, tamInfo, n++, params);
        ponerEnCola(&aux, dato, tamInfo);
    }
    while (dato && sacarDeCola(&aux, dato, tamInfo))
        ponerEnCola(cola, dato, tamInfo);
    free(dato);
}

/* Muestra Z<id>:[tope|...|fondo] desapilando a una pila auxiliar y volviendo
   a apilar, de modo que la zona queda igual que antes. */
void mostrarZona(void *info, unsigned tamInfo, unsigned n, void *params)
{
    tZona *zona = info;
    tPila aux;
    tContenedor cont;

    crearPila(&aux);
    printf("  Z%u:[", zona->id);
    while (sacarDePila(&zona->pila, &cont, sizeof(tContenedor)))
    {
        printf("%s%s", pilaVacia(&aux) ? "" : "|", cont.codigo);
        ponerEnPila(&aux, &cont, sizeof(tContenedor));
    }
    while (sacarDePila(&aux, &cont, sizeof(tContenedor)))
        ponerEnPila(&zona->pila, &cont, sizeof(tContenedor));
    printf("] (%u/%u)\n", zona->cantidad, zona->capacidad);
}

void mostrarMuelle(void *info, unsigned tamInfo, unsigned n, void *params)
{
    tMuelle *muelle = info;
    tContenedor proximo;

    if (muelle->ocupado && verPrimeroCola(&muelle->buque.manifiesto, &proximo, sizeof(tContenedor)))
        printf("  M%u: %s -> proximo contenedor %s\n", muelle->id, muelle->buque.codigo, proximo.codigo);
    else
        printf("  M%u: %s (libre)\n", muelle->id, RECURSO_VACIO);
}

void mostrarRegistroOperacion(void *info, unsigned tamInfo, unsigned n, void *params)
{
    tRegistroOperacion *reg = info;

    printf("  %3u) T=%-4u T=%-4u %-4s %-8s %-6s -> %s\n", n + 1, reg->inicio, reg->fin,
           reg->operacion, reg->contenedor, reg->origen, reg->destino);
}

void mostrarCamionVentana(void *info, unsigned tamInfo, unsigned n, void *params)
{
    tCamion *camion = info;

    if (n < 3)
        printf("  %u) %s -> solicita %s%s\n", n + 1, camion->codigo, camion->contenedor,
               n == 0 ? "   <- frente" : "");
}

void contarElementos(void *info, unsigned tamInfo, unsigned n, void *params)
{
    unsigned *cantidad = params;

    (*cantidad)++;
}

void contarZona(void *info, unsigned tamInfo, unsigned n, void *params)
{
    tZona *zona = info;
    tRecursos *r = params;

    r->noVacias += zona->cantidad > 0;
    r->conLugar += zona->cantidad < zona->capacidad;
    r->ambas += zona->cantidad > 0 && zona->cantidad < zona->capacidad;
    r->contenedores += zona->cantidad;
}

void contarMuelle(void *info, unsigned tamInfo, unsigned n, void *params)
{
    tMuelle *muelle = info;
    tRecursos *r = params;

    r->ocupados += muelle->ocupado;
}

void asignarMuelle(void *info, unsigned tamInfo, unsigned n, void *params)
{
    tMuelle *muelle = info;

    if (!muelle->ocupado && sacarDeCola(params, &muelle->buque, sizeof(tBuque)))
    {
        muelle->ocupado = 1;
        printf("  %s atraca en M%u\n", muelle->buque.codigo, muelle->id);
    }
}

void apilarEnZona(void *info, unsigned tamInfo, unsigned n, void *params)
{
    tZona *zona = info;
    tMovimiento *mov = params;

    mov->ok = ponerEnPila(&zona->pila, &mov->cont, sizeof(tContenedor));
    zona->cantidad += mov->ok;
}

void desapilarDeZona(void *info, unsigned tamInfo, unsigned n, void *params)
{
    tZona *zona = info;
    tMovimiento *mov = params;

    mov->ok = sacarDePila(&zona->pila, &mov->cont, sizeof(tContenedor));
    zona->cantidad -= mov->ok;
    mov->idZona = zona->id;
}

/* Saca el proximo contenedor del manifiesto; si era el ultimo, el buque
   completa su descarga (mov->ok) y libera el muelle. */
void descargarDelMuelle(void *info, unsigned tamInfo, unsigned n, void *params)
{
    tMuelle *muelle = info;
    tMovimiento *mov = params;

    sacarDeCola(&muelle->buque.manifiesto, &mov->cont, sizeof(tContenedor));
    mov->ok = colaVacia(&muelle->buque.manifiesto);
    muelle->ocupado = !mov->ok;
}
