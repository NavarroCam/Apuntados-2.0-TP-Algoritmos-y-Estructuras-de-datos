#include <time.h>
#include "./include/escenario.h"
#include "./include/operadores.h"
#include "./include/simulacion.h"

#define PATH_CONFIG "Archivos/config.txt"
#define PATH_PUERTO "Archivos/puerto.txt"
#define TITULO      "PUERTO DE CONTENEDORES - OPERACION CONTRARRELOJ"

/* Pruebas: TEST en 1 juega con el puerto.txt existente sin regenerarlo.
   SEMILLA distinta de 0 hace que cada jornada genere siempre el mismo
   puerto.txt; en 0 cada jornada es aleatoria. Para la entrega: 0 y 0. */
#define TEST        1
#define SEMILLA     0

void nuevaJornada(unsigned posicion, const char *nombre)
{
    tConfig config;
    tEstadoSimulacion estado;

    mostrarTitulo("NUEVA JORNADA", AZUL);
    if (TEST)
        printf("%s  MODO TEST: se usa %s sin regenerarlo (solo se genera si no existe)%s\n",
               MAGENTA, PATH_PUERTO, RESET);
    if (cargarConfiguracion(&config, PATH_CONFIG) != TODO_BIEN || !generarEscenario(PATH_PUERTO, &config, TEST, SEMILLA))
    {
        printf("%s  Error: %s no se pudo generar o supera los maximos, o no se pudo generar %s%s\n",
               ROJO, PATH_CONFIG, PATH_PUERTO, RESET);
        return;
    }
    mostrarConfiguracion(&config);
    printf("\n");
    pausarPantalla();
    limpiarPantalla();
    mostrarTitulo("JORNADA EN CURSO", AZUL);
    crearEstadoSimulacion(&estado, &config);
    if (!cargarEscenario(PATH_PUERTO, &estado))
        printf("%s  Error: %s tiene un formato invalido%s\n", ROJO, PATH_PUERTO, RESET);
    else
    {
        jugarJornada(&estado);
        if (!registrarJornada(posicion, nombre, &estado))
            printf("%s  Error: no se pudo registrar la jornada%s\n", ROJO, RESET);
    }
    vaciarEstadoSimulacion(&estado);
}

/* Redibuja el menu desde cero; el aviso es el resultado de la accion anterior. */
void mostrarMenuPrincipal(const char *nombre, const char *aviso)
{
    limpiarPantalla();
    mostrarTitulo(TITULO, CIAN);
    printf("\n  Operador: %s%s%s\n\n", NEGRITA, nombre, RESET);
    mostrarBorde('-', GRIS);
    printf("    %s[1]%s Iniciar nueva jornada\n", AMARILLO, RESET);
    printf("    %s[2]%s Ver ranking de operadores\n", AMARILLO, RESET);
    printf("    %s[3]%s Salir\n", AMARILLO, RESET);
    mostrarBorde('-', GRIS);
    if (aviso)
        printf("  %s\n", aviso);
    printf("\n  %sOpcion:%s ", NEGRITA, RESET);
}

int main()
{
    tArbolBinBusq indice;
    char nombre[TAM_LINEA], opcion[TAM_LINEA];
    const char *aviso;
    unsigned posicion;
    int resultado, salir = 0;

    srand(time(NULL));
    crearArbolBinBusq(&indice);
    cargarArchivoBinOrdenadoArbolBinBusq(&indice, PATH_INDICE, sizeof(tIndiceOperador));
    limpiarPantalla();
    mostrarTitulo(TITULO, CIAN);
    do
    {
        printf("\n  %sNombre del operador:%s ", NEGRITA, RESET);
        if (!fgets(nombre, sizeof(nombre), stdin))
            salir = 1;
        else
            *(nombre + strcspn(nombre, "\r\n")) = '\0';
    } while (!salir && !*nombre);

    resultado = salir ? OPERADOR_ERROR : ingresarOperador(&indice, nombre, &posicion);
    if (!salir && resultado == OPERADOR_ERROR)
        printf("%s  Error: no se pudo acceder a los archivos de operadores%s\n", ROJO, RESET);
    aviso = resultado == OPERADOR_NUEVO ? VERDE "Operador dado de alta." RESET : VERDE "Bienvenido nuevamente." RESET;
    while (resultado != OPERADOR_ERROR && !salir)
    {
        mostrarMenuPrincipal(nombre, aviso);
        aviso = NULL;
        if (!fgets(opcion, sizeof(opcion), stdin) || !strcmp(opcion, "3\n"))
            salir = 1;
        else if (!strcmp(opcion, "1\n") || !strcmp(opcion, "2\n"))
        {
            limpiarPantalla();
            if (*opcion == '1')
                nuevaJornada(posicion, nombre);
            else
                mostrarRanking(&indice);
            salir = !esperarEnter();
        }
        else
            aviso = ROJO "Opcion invalida: elija 1, 2 o 3." RESET;
    }
    vaciarArbolBinBusq(&indice);
    printf("\n%s  Hasta luego.%s\n", GRIS, RESET);
    return 0;
}
