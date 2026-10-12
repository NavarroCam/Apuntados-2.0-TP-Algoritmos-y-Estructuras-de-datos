#include <stdio.h>
#include <string.h>
#include "../include/escenario_interno.h"
#include "../include/lista.h"


static unsigned sortear(unsigned minimo, unsigned maximo)
{
    if(maximo <= minimo)
        return minimo;
    return minimo + (unsigned)rand() % (maximo - minimo + 1);
}

static int configuracionUsable(const tConfiguracion *config)
{
    return config->duracionJornadaMinutos > 0 &&
           config->cantidadMuelles > 0 &&
           config->cantidadZonasAlmacenamiento > 0 &&
           config->capacidadPila > 0 &&
           config->maximoBuques > 0 &&
           config->maximoContenedoresPorBuque > 0 &&
           config->maximoCamiones > 0 &&
           config->tiempoDescargaContenedor > 0 &&
           config->tiempoCargaCamion > 0;
}

static unsigned calcularLimiteContenedores(const tConfiguracion *config)
{
    unsigned limite = config->cantidadZonasAlmacenamiento * config->capacidadPila;

    while(limite > 1 &&
          limite * config->tiempoDescargaContenedor +
          MINIMO(limite, config->maximoCamiones) * config->tiempoCargaCamion >
          config->duracionJornadaMinutos)
        limite--;
    return limite;
}

static unsigned calcularTopeCamiones(const tConfiguracion *config, unsigned totalContenedores)
{
    unsigned tiempoDescargas = totalContenedores * config->tiempoDescargaContenedor;
    unsigned holgura = tiempoDescargas < config->duracionJornadaMinutos ?
                       config->duracionJornadaMinutos - tiempoDescargas : 0;

    return MINIMO(MINIMO(config->maximoCamiones, totalContenedores),
                  holgura / config->tiempoCargaCamion);
}

static void completarEncabezado(tEscenario *esc, const tConfiguracion *config)
{
    esc->duracionJornadaMinutos = config->duracionJornadaMinutos;
    esc->cantidadMuelles = config->cantidadMuelles;
    esc->cantidadZonas = config->cantidadZonasAlmacenamiento;
    esc->capacidadPila = config->capacidadPila;
}

static void escribirEncabezado(FILE *pf, tEscenario *esc)
{
    const tCampoEncabezado *campo = CAMPOS_ENCABEZADO;
    const tCampoEncabezado *fin = CAMPOS_ENCABEZADO + CANT_CAMPOS_ENCABEZADO;

    while(campo < fin)
    {
        fprintf(pf, "%s%c%u\n", campo->clave, SEP_CLAVE_VALOR, *campoDe(esc, campo));
        campo++;
    }
}

static void escribirInicioLinea(FILE *pf, char prefijo, unsigned numero, unsigned minuto)
{
    fprintf(pf, FORMATO_CODIGO, prefijo, numero);
    fprintf(pf, "%c%s%u%c%s", SEP_CAMPO, ETIQUETA_MINUTO, minuto, SEP_CAMPO, ETIQUETA_CONTENEDORES);
}

static void generarBuque(FILE *pf, unsigned numero, unsigned minuto,char (*codigos)[TAM_CODIGO], unsigned primero, unsigned cantidad)
{
    unsigned fila;

    escribirInicioLinea(pf, PREFIJO_BUQUE, numero, minuto);
    for(fila = primero; fila < primero + cantidad; fila++)
    {
        sprintf(codigos[fila], FORMATO_CODIGO, PREFIJO_CONTENEDOR, fila + 1);
        if(fila > primero)
            fputc(SEP_LISTA, pf);
        fputs(codigos[fila], pf);
    }
    fputc('\n', pf);
}

static unsigned generarBuques(FILE *pf, const tConfiguracion *config, unsigned limite, char (*codigos)[TAM_CODIGO])
{
    unsigned cantidadBuques = sortear(1, MINIMO(config->maximoBuques, limite));
    unsigned topeManifiesto = MINIMO(config->maximoContenedoresPorBuque, MAX_CONT_POR_BUQUE);
    unsigned total = 0;
    unsigned minuto = 0;
    unsigned numero;
    unsigned cantidad;

    for(numero = 1; numero <= cantidadBuques; numero++)
    {
        cantidad = sortear(1, MINIMO(topeManifiesto, limite - total - (cantidadBuques - numero)));
        minuto = sortear(minuto, MINIMO(total * config->tiempoDescargaContenedor,
                                        config->duracionJornadaMinutos));
        generarBuque(pf, numero, minuto, codigos, total, cantidad);
        total += cantidad;
    }
    return total;
}

static int elegirCamiones(char (*codigos)[TAM_CODIGO], unsigned total, const tConfiguracion *config, tLista *camiones)
{
    unsigned tope = calcularTopeCamiones(config, total);
    unsigned faltan = tope > 0 ? sortear(1, tope) : 0;
    unsigned fila = 0;
    tCamionEscenario camion;
    int correcto = TODO_BIEN;

    memset(&camion, 0, sizeof(camion));
    while(correcto && faltan > 0)
    {
        if(sortear(1, total - fila) <= faltan)
        {
            camion.minutoLlegada = sortear(0, MINIMO((fila + 1) * config->tiempoDescargaContenedor,
                                                     config->duracionJornadaMinutos));
            copiarCodigo(camion.contenedorSolicitado, codigos[fila]);
            correcto = insertarEnOrden(camiones, &camion, sizeof(camion), compararLlegadaCamion);
            faltan--;
        }
        fila++;
    }
    return correcto;
}

static void escribirCamiones(FILE *pf, tLista *camiones)
{
    tCamionEscenario camion;
    unsigned numero = 1;

    while(sacarPrimeroLista(camiones, &camion, sizeof(camion)))
    {
        escribirInicioLinea(pf, PREFIJO_CAMION, numero, camion.minutoLlegada);
        fprintf(pf, "%s\n", camion.contenedorSolicitado);
        numero++;
    }
}

static int escribirEscenario(FILE *pf, const tConfiguracion *config)
{
    tEscenario encabezado;
    tLista camiones;
    unsigned limite = calcularLimiteContenedores(config);
    char (*codigos)[TAM_CODIGO] = malloc(limite * sizeof(*codigos));
    unsigned total;
    int correcto;

    if(codigos == NULL)
        return ERROR;
    crearLista(&camiones);
    completarEncabezado(&encabezado, config);
    escribirEncabezado(pf, &encabezado);
    fprintf(pf, "\n%s\n", SECCION_BUQUES_TXT);
    total = generarBuques(pf, config, limite, codigos);
    fprintf(pf, "\n%s\n", SECCION_CAMIONES_TXT);
    correcto = elegirCamiones(codigos, total, config, &camiones);
    escribirCamiones(pf, &camiones);
    vaciarLista(&camiones);
    free(codigos);
    return correcto && !ferror(pf);
}

static int verificarEscenario(const char *nomArch)
{
    tEscenario leido;
    int valido = leerEscenario(&leido, nomArch) == TODO_BIEN;

    if(valido)
        liberarEscenario(&leido);
    return valido;
}

int generarEscenario(const tConfiguracion *config, const char *nomArch)
{
    FILE *pf;
    int correcto;

    if(!configuracionUsable(config))
        return ERROR_ARCH;
    pf = fopen(nomArch, "wt");
    if(pf == NULL)
        return ERROR_ARCH;
    correcto = escribirEscenario(pf, config);
    correcto = fclose(pf) == 0 && correcto;
    return correcto && verificarEscenario(nomArch) ? TODO_BIEN : ERROR_ARCH;
}
