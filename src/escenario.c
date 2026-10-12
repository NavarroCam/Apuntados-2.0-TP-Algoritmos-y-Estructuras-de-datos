#include <string.h>
#include "../include/escenario_interno.h"


const tCampoEncabezado CAMPOS_ENCABEZADO[CANT_CAMPOS_ENCABEZADO] =
{
    {CLAVE_JORNADA,        offsetof(tEscenario, duracionJornadaMinutos), ERROR},
    {CLAVE_MUELLES,        offsetof(tEscenario, cantidadMuelles),        ERROR},
    {CLAVE_ZONAS,          offsetof(tEscenario, cantidadZonas),          ERROR},
    {CLAVE_CAPACIDAD_PILA, offsetof(tEscenario, capacidadPila),          ERROR}
};

unsigned* campoDe(tEscenario *esc, const tCampoEncabezado *campo)
{
    return (unsigned *)((char *)esc + campo->desplazamiento);
}

void copiarCodigo(char *destino, const char *origen)
{
    memset(destino, 0, TAM_CODIGO);
    strcpy(destino, origen);
}

int compararLlegadaBuque(const void *existente, const void *nuevo)
{
    const tBuqueEscenario *a = (const tBuqueEscenario *)existente;
    const tBuqueEscenario *b = (const tBuqueEscenario *)nuevo;

    return a->minutoLlegada <= b->minutoLlegada ? -1 : 1;
}

int compararLlegadaCamion(const void *existente, const void *nuevo)
{
    const tCamionEscenario *a = (const tCamionEscenario *)existente;
    const tCamionEscenario *b = (const tCamionEscenario *)nuevo;

    return a->minutoLlegada <= b->minutoLlegada ? -1 : 1;
}

int saltarEtiqueta(char **str, const char *etiqueta)
{
    size_t len = strlen(etiqueta);
    if(strncmp(*str, etiqueta, len) == 0)
    {
        *str += len;
        return 1;
    }
    return 0;
}

int interpretarNumero(const char *str, unsigned *num)
{
    if(*str == '\0')
        return 0;
    *num = 0;
    while(*str >= '0' && *str <= '9')
    {
        *num = *num * 10 + (*str - '0');
        str++;
    }
    return *str == '\0';
}

char *siguienteCampo(char **str, char separador)
{
    if(*str == NULL)
        return NULL;
    char *inicio = *str;
    char *fin = strchr(inicio, separador);
    if(fin == NULL)
    {
        *str = NULL;
        return inicio;
    }
    *fin = '\0';
    *str = fin + 1;
    return inicio;
}

void liberarEscenario(tEscenario *esc)
{
    vaciarCola(&esc->buques);
    vaciarCola(&esc->camiones);
    esc->cantidadBuques = 0;
    esc->cantidadCamiones = 0;
}
