#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../include/config.h"

char* recortarCadena(char *cadena)
{
    char *fin;

    while(*cadena != '\0' && isspace((char)*cadena))
    {
        cadena++;
    }
    fin = cadena + strlen(cadena);
    while(fin > cadena && isspace((char)*(fin - 1)))
    {
        fin--;
    }

    *fin = '\0';
    return cadena;
}

void convertMinusCadena(char *cadena)
{
    char* auxCad = cadena;

    while(*auxCad != '\0')
    {
        *auxCad = (char)tolower((char)*auxCad);
        auxCad++;
    }
}

void asignarParametro(tConfiguracion *config, const char *clave, unsigned valor)
{
    if(strcmp(clave, "duracion_jornada_minutos") == 0)
        config->duracionJornadaMinutos = valor;
    else if(strcmp(clave, "cantidad_muelles") == 0)
        config->cantidadMuelles = valor;
    else if(strcmp(clave, "cantidad_zonas_almacenamiento") == 0)
        config->cantidadZonasAlmacenamiento = valor;
    else if(strcmp(clave, "capacidad_pila") == 0)
        config->capacidadPila = valor;
    else if(strcmp(clave, "maximo_buques") == 0)
        config->maximoBuques = valor;
    else if(strcmp(clave, "maximo_contenedores_por_buque") == 0)
        config->maximoContenedoresPorBuque = valor;
    else if(strcmp(clave, "maximo_camiones") == 0)
        config->maximoCamiones = valor;
    else if(strcmp(clave, "tiempo_descarga_contenedor") == 0)
        config->tiempoDescargaContenedor = valor;
    else if(strcmp(clave, "tiempo_reubicacion_contenedor") == 0)
        config->tiempoReubicacionContenedor = valor;
    else if(strcmp(clave, "tiempo_carga_camion") == 0)
        config->tiempoCargaCamion = valor;
    else
        printf("  [config] Parametro desconocido ignorado: %s\n", clave);
}

void establecerConfiguracionPorDefecto(tConfiguracion *config)
{
    config->duracionJornadaMinutos = 30;
    config->cantidadMuelles = 1;
    config->cantidadZonasAlmacenamiento = 3;
    config->capacidadPila = 3;
    config->maximoBuques = 2;
    config->maximoContenedoresPorBuque = 3;
    config->maximoCamiones = 5;
    config->tiempoDescargaContenedor = 2;
    config->tiempoReubicacionContenedor = 1;
    config->tiempoCargaCamion = 2;
}

int cargarConfiguracion(tConfiguracion *config, const char *nomArch)
{
    FILE *pf;
    char linea[TAM_LINEA_CONFIG];
    char *separador;
    char *param;
    char *cadValor;
    unsigned valor;

    establecerConfiguracionPorDefecto(config);//se hace aca por si algun parametro llega a estar vacío

    pf = fopen(nomArch, "rt");
    if(!pf)
        return ERROR_ARCH;

    while(fgets(linea, TAM_LINEA_CONFIG, pf))
    {
        separador = strpbrk(linea, ":=");//la funcion busca en linea los caracteres pasados en el segundo parametro.
        if(separador != NULL)
        {
            *separador = '\0';
            param = recortarCadena(linea);
            convertMinusCadena(param);
            cadValor = recortarCadena(separador + 1);
            if(*param != '\0' && esNumeroPos(cadValor))
            {
                valor = (unsigned) (*cadValor) - '0';
                if(valor > 0)
                    asignarParametro(config, param, valor);
            }
        }
    }
    fclose(pf);
    return TODO_BIEN;
}

int esNumeroPos(const char* cad)
{
    if(*cad == '\0')
        return 0;
    while(*cad != '\0')
    {
        if(!ES_DIGITO(*cad))
            return 0;
        cad++;
    }
    return 1;
}

int guardarConfiguracion(const tConfiguracion *config, const char *nomArch)
{
    FILE *pf;

    pf = fopen(nomArch, "wt");
    if(!pf)
        return ERROR_ARCH;

    fprintf(pf, "duracion_jornada_minutos: %u\n", config->duracionJornadaMinutos);
    fprintf(pf, "cantidad_muelles: %u\n", config->cantidadMuelles);
    fprintf(pf, "cantidad_zonas_almacenamiento: %u\n", config->cantidadZonasAlmacenamiento);
    fprintf(pf, "capacidad_pila: %u\n", config->capacidadPila);
    fprintf(pf, "maximo_buques: %u\n", config->maximoBuques);
    fprintf(pf, "maximo_contenedores_por_buque: %u\n", config->maximoContenedoresPorBuque);
    fprintf(pf, "maximo_camiones: %u\n", config->maximoCamiones);
    fprintf(pf, "tiempo_descarga_contenedor: %u\n", config->tiempoDescargaContenedor);
    fprintf(pf, "tiempo_reubicacion_contenedor: %u\n", config->tiempoReubicacionContenedor);
    fprintf(pf, "tiempo_carga_camion: %u\n", config->tiempoCargaCamion);
    fclose(pf);
    return TODO_BIEN;
}

void mostrarConfiguracion(const tConfiguracion *config)
{
    printf("\n  PARAMETROS DE LA JORNADA\n");
    printf("  ------------------------------------------------------\n");
    printf("  Duracion de la jornada .............. %u minutos\n", config->duracionJornadaMinutos);
    printf("  Muelles ............................. %u\n", config->cantidadMuelles);
    printf("  Zonas de almacenamiento ............. %u\n", config->cantidadZonasAlmacenamiento);
    printf("  Capacidad de cada pila .............. %u\n", config->capacidadPila);
    printf("  Maximo de buques .................... %u\n", config->maximoBuques);
    printf("  Maximo contenedores por buque ....... %u\n", config->maximoContenedoresPorBuque);
    printf("  Maximo de camiones .................. %u\n", config->maximoCamiones);
    printf("  Tiempo de descarga .................. %u minuto(s)\n", config->tiempoDescargaContenedor);
    printf("  Tiempo de reubicacion ............... %u minuto(s)\n", config->tiempoReubicacionContenedor);
    printf("  Tiempo de carga de camion ........... %u minuto(s)\n", config->tiempoCargaCamion);
    printf("  ------------------------------------------------------\n");
}
