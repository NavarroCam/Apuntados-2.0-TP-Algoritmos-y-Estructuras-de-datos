#include <stdio.h>
#include <string.h>
#include "../include/escenario_interno.h"
#include "../include/lista.h"
#include "../include/arbol.h"
enum { SEC_ENCABEZADO, SEC_BUQUES, SEC_CAMIONES };

typedef struct
{
    int seccion;
    tCampoEncabezado campos[CANT_CAMPOS_ENCABEZADO];
    unsigned camposCargados;
    unsigned totalContenedores;
    tLista buques;
    tLista camiones;
    tArbolBinBusq contenedores;
    tArbolBinBusq solicitados;
    tArbolBinBusq vehiculos;
} tLectura;

static int esCodigoValido(const char *codigo, char prefijo)
{
    if(*codigo != prefijo || strlen(codigo) >= TAM_CODIGO)
        return ERROR;
    codigo++;
    if(*codigo == '\0')
        return ERROR;
    while(ES_DIGITO(*codigo))
        codigo++;
    return *codigo == '\0';
}


static int compararCodigos(const void *a, const void *b)
{
    return strcmp((const char *)a, (const char *)b);
}

static int registrarCodigo(tArbolBinBusq *conjunto, const char *codigo)
{
    char copia[TAM_CODIGO];

    copiarCodigo(copia, codigo);
    return insertarArbolBinBusq(conjunto, copia, TAM_CODIGO, compararCodigos) == TODO_BIEN;
}

static int existeCodigo(const tArbolBinBusq *conjunto, const char *codigo)
{
    char copia[TAM_CODIGO];

    copiarCodigo(copia, codigo);
    return buscarElemArbolBinBusq(conjunto, copia, TAM_CODIGO, compararCodigos);
}


static int pasarListaACola(tLista *lista, tCola *cola, unsigned tamElemento)
{
    void *elemento = malloc(tamElemento);
    int correcto = elemento != NULL;

    while(correcto && !listaVacia(lista))
        correcto = sacarPrimeroLista(lista, elemento, tamElemento) &&
                   ponerEnCola(cola, elemento, tamElemento);
    free(elemento);
    return correcto;
}

static int interpretarMinuto(char *campo, unsigned duracion, unsigned *minuto)
{
    return saltarEtiqueta(&campo, ETIQUETA_MINUTO) &&
           interpretarNumero(campo, minuto) &&
           *minuto <= duracion;
}

static int interpretarEncabezado(char *linea, tEscenario *esc, tLectura *lec)
{
    char *cursor = linea;
    char *clave = siguienteCampo(&cursor, SEP_CLAVE_VALOR);
    char *valor = siguienteCampo(&cursor, SEP_CLAVE_VALOR);
    tCampoEncabezado *campo = lec->campos;
    const tCampoEncabezado *fin = lec->campos + CANT_CAMPOS_ENCABEZADO;
    unsigned numero;

    while(campo < fin && strcmp(campo->clave, clave) != 0)
        campo++;
    if(campo == fin || valor == NULL || cursor != NULL || campo->cargado)
        return ERROR;
    if(!interpretarNumero(valor, &numero) || numero == 0)
        return ERROR;
    *campoDe(esc, campo) = numero;
    campo->cargado = TODO_BIEN;
    lec->camposCargados++;
    return TODO_BIEN;
}

static int interpretarManifiesto(char *campo, tBuqueEscenario *buque, tArbolBinBusq *contenedores)
{
    char *codigo;
    int valido = saltarEtiqueta(&campo, ETIQUETA_CONTENEDORES);

    buque->cantidadContenedores = 0;
    while(valido && campo != NULL)
    {
        codigo = siguienteCampo(&campo, SEP_LISTA);
        valido = buque->cantidadContenedores < MAX_CONT_POR_BUQUE &&
                 esCodigoValido(codigo, PREFIJO_CONTENEDOR) &&
                 registrarCodigo(contenedores, codigo);
        if(valido)
        {
            copiarCodigo(*(buque->manifiesto + buque->cantidadContenedores), codigo);
            buque->cantidadContenedores++;
        }
    }
    return valido && buque->cantidadContenedores > 0;
}

static int interpretarBuque(char *linea, tEscenario *esc, tLectura *lec)
{
    tBuqueEscenario buque;
    char *cursor = linea;
    char *codigo = siguienteCampo(&cursor, SEP_CAMPO);
    char *minuto = siguienteCampo(&cursor, SEP_CAMPO);
    char *manifiesto = siguienteCampo(&cursor, SEP_CAMPO);
    int valido;

    memset(&buque, 0, sizeof(buque));
    valido = cursor == NULL &&
             esCodigoValido(codigo, PREFIJO_BUQUE) &&
             registrarCodigo(&lec->vehiculos, codigo) &&
             interpretarMinuto(minuto, esc->duracionJornadaMinutos, &buque.minutoLlegada) &&
             interpretarManifiesto(manifiesto, &buque, &lec->contenedores);
    if(valido)
    {
        copiarCodigo(buque.codigo, codigo);
        valido = insertarEnOrden(&lec->buques, &buque, sizeof(buque), compararLlegadaBuque);
    }
    if(valido)
    {
        esc->cantidadBuques++;
        lec->totalContenedores += buque.cantidadContenedores;
    }
    return valido;
}

static int interpretarCamion(char *linea, tEscenario *esc, tLectura *lec)
{
    tCamionEscenario camion;
    char *cursor = linea;
    char *codigo = siguienteCampo(&cursor, SEP_CAMPO);
    char *minuto = siguienteCampo(&cursor, SEP_CAMPO);
    char *pedido = siguienteCampo(&cursor, SEP_CAMPO);
    int valido;

    memset(&camion, 0, sizeof(camion));
    valido = cursor == NULL &&
             esCodigoValido(codigo, PREFIJO_CAMION) &&
             registrarCodigo(&lec->vehiculos, codigo) &&
             interpretarMinuto(minuto, esc->duracionJornadaMinutos, &camion.minutoLlegada) &&
             saltarEtiqueta(&pedido, ETIQUETA_CONTENEDORES) &&
             esCodigoValido(pedido, PREFIJO_CONTENEDOR) &&
             existeCodigo(&lec->contenedores, pedido) &&
             registrarCodigo(&lec->solicitados, pedido);
    if(valido)
    {
        copiarCodigo(camion.codigo, codigo);
        copiarCodigo(camion.contenedorSolicitado, pedido);
        valido = insertarEnOrden(&lec->camiones, &camion, sizeof(camion), compararLlegadaCamion);
    }
    if(valido)
        esc->cantidadCamiones++;
    return valido;
}


static int abrirSeccionBuques(tLectura *lec)
{
    if(lec->seccion != SEC_ENCABEZADO || lec->camposCargados != CANT_CAMPOS_ENCABEZADO)
        return ERROR;
    lec->seccion = SEC_BUQUES;
    return TODO_BIEN;
}

static int abrirSeccionCamiones(tLectura *lec)
{
    if(lec->seccion != SEC_BUQUES)
        return ERROR;
    lec->seccion = SEC_CAMIONES;
    return TODO_BIEN;
}

static int procesarLinea(char *linea, tEscenario *esc, tLectura *lec)
{
    int valida;

    if(strcmp(linea, SECCION_BUQUES_TXT) == 0)
        valida = abrirSeccionBuques(lec);
    else if(strcmp(linea, SECCION_CAMIONES_TXT) == 0)
        valida = abrirSeccionCamiones(lec);
    else if(lec->seccion == SEC_ENCABEZADO)
        valida = interpretarEncabezado(linea, esc, lec);
    else if(lec->seccion == SEC_BUQUES)
        valida = interpretarBuque(linea, esc, lec);
    else
        valida = interpretarCamion(linea, esc, lec);
    return valida;
}

static int leerLineas(FILE *pf, tEscenario *esc, tLectura *lec)
{
    char linea[TAM_LINEA_ESCENARIO];
    char *texto;
    int valida = TODO_BIEN;

    while(valida && fgets(linea, TAM_LINEA_ESCENARIO, pf))
    {
        valida = strchr(linea, '\n') != NULL || feof(pf); /* linea truncada = invalida */
        texto = recortarCadena(linea);
        if(valida && *texto != '\0' && *texto != MARCA_COMENTARIO)
            valida = procesarLinea(texto, esc, lec);
    }
    return valida;
}

static int escenarioConsistente(const tEscenario *esc, const tLectura *lec)
{
    return lec->seccion != SEC_ENCABEZADO &&
           esc->cantidadBuques > 0 &&
           (lec->totalContenedores + esc->cantidadZonas - 1) / esc->cantidadZonas <= esc->capacidadPila;
}

static void inicializarEscenario(tEscenario *esc)
{
    esc->duracionJornadaMinutos = 0;
    esc->cantidadMuelles = 0;
    esc->cantidadZonas = 0;
    esc->capacidadPila = 0;
    esc->cantidadBuques = 0;
    esc->cantidadCamiones = 0;
    crearCola(&esc->buques);
    crearCola(&esc->camiones);
}

static void inicializarLectura(tLectura *lec)
{
    lec->seccion = SEC_ENCABEZADO;
    memcpy(lec->campos, CAMPOS_ENCABEZADO, sizeof(CAMPOS_ENCABEZADO));
    lec->camposCargados = 0;
    lec->totalContenedores = 0;
    crearLista(&lec->buques);
    crearLista(&lec->camiones);
    crearArbolBinBusq(&lec->contenedores);
    crearArbolBinBusq(&lec->solicitados);
    crearArbolBinBusq(&lec->vehiculos);
}

static void finalizarLectura(tLectura *lec)
{
    vaciarLista(&lec->buques);
    vaciarLista(&lec->camiones);
    vaciarArbolBinBusq(&lec->contenedores);
    vaciarArbolBinBusq(&lec->solicitados);
    vaciarArbolBinBusq(&lec->vehiculos);
}

int leerEscenario(tEscenario *esc, const char *nomArch)
{
    FILE *pf;
    tLectura lec;
    int valido;

    inicializarEscenario(esc);
    pf = fopen(nomArch, "rt");
    if(pf == NULL)
        return ERROR_ARCH;
    inicializarLectura(&lec);
    valido = leerLineas(pf, esc, &lec);
    fclose(pf);
    valido = valido &&
             escenarioConsistente(esc, &lec) &&
             pasarListaACola(&lec.buques, &esc->buques, sizeof(tBuqueEscenario)) &&
             pasarListaACola(&lec.camiones, &esc->camiones, sizeof(tCamionEscenario));
    finalizarLectura(&lec);
    if(!valido)
        liberarEscenario(esc);
    return valido ? TODO_BIEN : ERROR_ARCH;
}
