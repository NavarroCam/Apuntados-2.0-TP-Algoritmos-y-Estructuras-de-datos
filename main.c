#include <time.h>
#include "./include/simulacion.h"

#define PATH_CONFIG "Archivos/config.txt"
#define PATH_PUERTO "Archivos/puerto.txt"

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
        {
            normalizarNombreOperario(nombre);
        }
    }
    while (!salir && !*nombre);

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
                nuevaJornada(posicion, nombre,PATH_CONFIG,PATH_PUERTO);
            else
                mostrarRanking(&indice);
            salir = !esperarEnter();
        }
        else
            aviso = ROJO "Opcion invalida: elija alguna de las que estan en pantalla." RESET;
    }
    vaciarArbolBinBusq(&indice);
    printf("\n%s  Hasta luego.%s\n", GRIS, RESET);
    return 0;
}
