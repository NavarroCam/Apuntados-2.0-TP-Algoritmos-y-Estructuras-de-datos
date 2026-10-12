#ifndef ESCENARIO_INTERNO_H_INCLUDED
#define ESCENARIO_INTERNO_H_INCLUDED

#include <stddef.h>
#include "escenario.h"

#define SEP_CLAVE_VALOR       ':'
#define SEP_CAMPO             ';'
#define SEP_LISTA             ','

#define SECCION_BUQUES_TXT    "[BUQUES]"
#define SECCION_CAMIONES_TXT  "[CAMIONES]"

#define ETIQUETA_MINUTO       "T="
#define ETIQUETA_CONTENEDORES "C="

#define PREFIJO_BUQUE         'B'
#define PREFIJO_CAMION        'K'
#define PREFIJO_CONTENEDOR    'C'
#define FORMATO_CODIGO        "%c%03u"

#define CANT_CAMPOS_ENCABEZADO 4

#define CLAVE_JORNADA         "JORNADA"
#define CLAVE_MUELLES         "MUELLES"
#define CLAVE_ZONAS           "ZONAS"
#define CLAVE_CAPACIDAD_PILA  "CAPACIDAD_PILA"

#define TAM_LINEA_ESCENARIO   512
#define MARCA_COMENTARIO      '#'
typedef struct
{
    const char *clave;
    size_t desplazamiento;
    int cargado;
} tCampoEncabezado;

extern const tCampoEncabezado CAMPOS_ENCABEZADO[CANT_CAMPOS_ENCABEZADO];

unsigned* campoDe(tEscenario *esc, const tCampoEncabezado *campo);
void copiarCodigo(char *destino, const char *origen);
int saltarEtiqueta(char **str, const char *etiqueta);
int interpretarNumero(const char *str, unsigned *num);
char *siguienteCampo(char **str, char separador);
int compararLlegadaBuque(const void *existente, const void *nuevo);
int compararLlegadaCamion(const void *existente, const void *nuevo);

#endif // ESCENARIO_INTERNO_H_INCLUDED
