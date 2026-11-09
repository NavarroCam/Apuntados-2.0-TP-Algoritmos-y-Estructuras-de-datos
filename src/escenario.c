#include "../include/escenario.h"

#define MULTIPLICADOR_CONTENEDOR    100

unsigned numeroAleatorio(unsigned n)
{
    unsigned long r = (unsigned long)rand() * (RAND_MAX + 1UL) + rand();

    return r % n;
}

int generarEscenario(const char *path, const tConfig *c, int modoTest, unsigned semilla)
{
    unsigned *codigos, *fin, *actual, *elegido;
    unsigned cantBuques, cantCamiones = 0, total;
    unsigned i, j, cant, aux, llegada = 0;
    FILE *pf;

    if (modoTest)
    {
        pf = fopen(path, "rt");
        if (pf)
        {
            fclose(pf);
            return 1;
        }
    }
    if (semilla)
        srand(semilla);
    codigos = malloc(sizeof(unsigned) * c->maximoBuques * c->maximoContenedoresPorBuque);
    if(!codigos)
        return 0;
    fin = codigos;
    pf = fopen(path, "wt");
    if (!pf)
    {
        free(codigos);
        return 0;
    }
    cantBuques = 1 + numeroAleatorio(c->maximoBuques);
    fprintf(pf, "JORNADA:%u\nMUELLES:%u\nZONAS:%u\nCAPACIDAD_PILA:%u\n\n[BUQUES]\n",
            c->duracionJornadaMinutos, c->cantidadMuelles, c->cantidadZonasAlmacenamiento, c->capacidadPila);
    for (i = 1; i <= cantBuques; i++)
    {
        llegada += numeroAleatorio((c->duracionJornadaMinutos - 1) / cantBuques + 1);
        cant = 1 + numeroAleatorio(c->maximoContenedoresPorBuque);
        fprintf(pf, "B%03u;T=%u;C=", i, llegada);
        for (j = 1; j <= cant; j++, fin++)
        {
            *fin = i * MULTIPLICADOR_CONTENEDOR + j;
            fprintf(pf, "%sC%u", j > 1 ? "," : "", *fin);
        }
        fprintf(pf, "\n");
    }
    total = fin - codigos;
    if (MINIMO(c->maximoCamiones, total) > 0)
        cantCamiones = 1 + numeroAleatorio(MINIMO(c->maximoCamiones, total));
    fprintf(pf, "\n[CAMIONES]\n");
    for (actual = codigos, i = 1, llegada = 0; i <= cantCamiones; actual++, i++)
    {
        elegido = actual + numeroAleatorio(fin - actual);
        aux = *actual;
        *actual = *elegido;
        *elegido = aux;
        llegada += numeroAleatorio((c->duracionJornadaMinutos - 1) / cantCamiones + 1);
        fprintf(pf, "K%03u;T=%u;C=C%u\n", i, llegada, *actual);
    }
    fclose(pf);
    free(codigos);
    return 1;
}

int cargarBuque(char *linea, tEstadoSimulacion *e, unsigned *ultimaLlegada)
{
    tBuque buque;
    tContenedor cont;
    char *token;
    int fin = 0, ok;

    ok = sscanf(linea, "%7[^;];T=%u;C=%n", buque.codigo, &buque.llegada, &fin) == 2 && fin &&
         buque.llegada >= *ultimaLlegada && buque.llegada < e->config.duracionJornadaMinutos;
    *ultimaLlegada = buque.llegada;
    crearCola(&buque.manifiesto);
    for (token = strtok(linea + fin, ",\n"); ok && token; token = strtok(NULL, ",\n"))
    {
        ok = *token == 'C' && strlen(token) < TAM_CODIGO;
        if (ok)
            ok = ponerEnCola(&buque.manifiesto, strcpy(cont.codigo, token), sizeof(tContenedor));
    }
    ok = ok && !colaVacia(&buque.manifiesto) && ponerEnCola(&e->arribosBuques, &buque, sizeof(tBuque));
    if (!ok)
        vaciarCola(&buque.manifiesto);
    return ok;
}

int cargarCamion(const char *linea, tEstadoSimulacion *e, unsigned *ultimaLlegada)
{
    tCamion camion;
    int ok;

    ok = sscanf(linea, "%7[^;];T=%u;C=%7s", camion.codigo, &camion.llegada, camion.contenedor) == 3 &&
         camion.llegada >= *ultimaLlegada && camion.llegada < e->config.duracionJornadaMinutos &&
         ponerEnCola(&e->arribosCamiones, &camion, sizeof(tCamion));
    *ultimaLlegada = camion.llegada;
    return ok;
}

int cargarEscenario(const char *path, tEstadoSimulacion *e)
{
    const tConfig *c = &e->config;
    char linea[TAM_LINEA];
    unsigned jornada, muelles, zonas, capacidad, i, ultimoBuque = 0, ultimoCamion = 0;
    tMuelle muelle;
    tZona zona;
    FILE *pf = fopen(path, "rt");

    if (!pf)
        return 0;
    if (fscanf(pf, " JORNADA:%u MUELLES:%u ZONAS:%u CAPACIDAD_PILA:%u",
               &jornada, &muelles, &zonas, &capacidad) != 4 ||
            jornada != c->duracionJornadaMinutos || muelles != c->cantidadMuelles ||
            zonas != c->cantidadZonasAlmacenamiento || capacidad != c->capacidadPila)
    {
        fclose(pf);
        return 0;
    }
    for (i = 1; i <= muelles; i++)
    {
        crearMuelle(&muelle, i);
        ponerAlFinal(&e->muelles, &muelle, sizeof(tMuelle));
    }
    for (i = 1; i <= zonas; i++)
    {
        crearZona(&zona, i, capacidad);
        ponerAlFinal(&e->zonas, &zona, sizeof(tZona));
    }
    while (fgets(linea, sizeof(linea), pf))
    {
        if (*linea == 'B')
            cargarBuque(linea, e, &ultimoBuque);
        else if (*linea == 'K')
            cargarCamion(linea, e, &ultimoCamion);
    }
    fclose(pf);
    return 1;
}
