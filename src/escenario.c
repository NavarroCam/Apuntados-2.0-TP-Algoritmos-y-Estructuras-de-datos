#include "../include/escenario.h"

#define MULTIPLICADOR_CONTENEDOR    100

/* Entero al azar en [0, n). Combina dos llamadas a rand() porque RAND_MAX
   puede ser 32767 y la cantidad de contenedores puede superarlo. */
unsigned numeroAleatorio(unsigned n)
{
    unsigned long r = rand();

    r = r * (RAND_MAX + 1UL) + rand();
    return r % n;
}

/* Cantidades al azar dentro de los maximos de config.txt. Las llegadas se
   generan crecientes (archivo ordenado por T) y con salto = (duracion - 1) /
   cantidad nunca superan la jornada. Cada camion pide un contenedor distinto,
   elegido con Fisher-Yates parcial sobre los codigos generados. */
int generarEscenario(const char *path, const tConfig *c, int modoTest, unsigned semilla)
{
    unsigned *codigos, *fin, *actual, *elegido;
    unsigned cantBuques, cantCamiones = 0, total;
    unsigned i, j, cant, aux, llegada = 0;
    FILE *pf;
    int ok;

    if (modoTest && (pf = fopen(path, "rt")) != NULL)
        return fclose(pf) == 0;
    if (semilla)
        srand(semilla);
    codigos = malloc(sizeof(unsigned) * c->maximoBuques * c->maximoContenedoresPorBuque);
    fin = codigos;
    cantBuques = 1 + numeroAleatorio(c->maximoBuques);
    pf = fopen(path, "wt");
    ok = codigos && pf;
    if (ok)
    {
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
        ok = !ferror(pf);
    }
    if (pf)
        ok = fclose(pf) == 0 && ok;
    free(codigos);
    return ok;
}

int cargarBuque(char *linea, tEstadoSimulacion *e, unsigned *ultimaLlegada)
{
    tBuque buque;
    tContenedor cont;
    char *token;
    int fin = 0, ok;

    /* %7 corresponde a TAM_CODIGO - 1 */
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

/* La cabecera debe coincidir con config.txt, del que se genero el escenario. */
int cargarEscenario(const char *path, tEstadoSimulacion *e)
{
    const tConfig *c = &e->config;
    char linea[TAM_LINEA];
    unsigned jornada, muelles, zonas, capacidad, i, ultimoBuque = 0, ultimoCamion = 0, seccion = 0;
    int fin = 0, ok;
    tMuelle muelle;
    tZona zona;
    FILE *pf = fopen(path, "rt");

    if (!pf)
        return 0;
    ok = fscanf(pf, " JORNADA:%u MUELLES:%u ZONAS:%u CAPACIDAD_PILA:%u [BUQUES]%n",
                &jornada, &muelles, &zonas, &capacidad, &fin) == 4 && fin &&
         jornada == c->duracionJornadaMinutos && muelles == c->cantidadMuelles &&
         zonas == c->cantidadZonasAlmacenamiento && capacidad == c->capacidadPila;
    for (i = 1; ok && i <= muelles; i++)
    {
        crearMuelle(&muelle, i);
        ok = ponerAlFinal(&e->muelles, &muelle, sizeof(tMuelle));
    }
    for (i = 1; ok && i <= zonas; i++)
    {
        crearZona(&zona, i, capacidad);
        ok = ponerAlFinal(&e->zonas, &zona, sizeof(tZona));
    }
    while (ok && fgets(linea, sizeof(linea), pf))
    {
        if (seccion == 0 && *linea == 'B')
            ok = cargarBuque(linea, e, &ultimoBuque);
        else if (seccion == 1 && *linea == 'K')
            ok = cargarCamion(linea, e, &ultimoCamion);
        else if (seccion == 0 && !strcmp(linea, "[CAMIONES]\n"))
            seccion = 1;
        else
            ok = !strcmp(linea, "\n");
    }
    fclose(pf);
    return ok && seccion == 1;
}
