#include <time.h>
#include "./include/simulacion.h"

#define PATH_CONFIG "Archivos/config.txt"
#define PATH_PUERTO "Archivos/puerto.txt"

int main()
{
    tArbolBinBusq indice;
    char nombre[TAM_LINEA];
    const char *aviso;
    unsigned posicion;
    int resultado, opc = 0;

    srand(time(NULL));
    crearArbolBinBusq(&indice);
    cargarArchivoBinOrdenadoArbolBinBusq(&indice, PATH_INDICE, sizeof(tIndiceOperador));
    limpiarPantalla();
    mostrarTitulo(TITULO, CIAN);
    do
    {
        printf("\n  %sNombre del operador:%s ", NEGRITA, RESET);
        if (fgets(nombre,sizeof(nombre),stdin))
            normalizarNombreOperario(nombre);
        else
            fprintf(stderr,"%s Error: el nombre no puede estar vacío.%s\n", ROJO,RESET);
    }
    while (!*nombre);

    if(*nombre)
    {
        resultado = ingresarOperador(&indice,nombre,&posicion);
        if(resultado == ERROR_OPERARIO)
            fprintf(stderr,"%s  Error: no se pudo acceder a los archivos de operadores%s\n", ROJO, RESET);
        aviso = (resultado == NUEVO_OPERARIO) ? VERDE "Alta de operario." RESET : VERDE "Bienvenido nuevamente." RESET;
    }

    while (resultado != ERROR_OPERARIO && opc !=3)
    {
        mostrarMenuPrincipal(nombre, aviso);
        aviso = NULL;
        scanf("%d", &opc);
        fflush(stdin);
        limpiarPantalla();
        switch(opc)
        {
        case 1:
        {
            nuevaJornada(posicion,nombre,PATH_CONFIG,PATH_PUERTO);
            if(!esperarEnter())
                opc = 3;
            break;
        }
        case 2:
        {
            mostrarRanking(&indice);
            if(!esperarEnter())
                opc = 3;
            break;
        }
        case 3:
        {
            break;
        }
        default:
        {
            aviso = ROJO "Opcion invalida: elija alguna de las que estan en pantalla." RESET;
            break;
        }
        }
    }

    vaciarArbolBinBusq(&indice);
    printf("\n%s  Hasta luego.%s\n", GRIS, RESET);
    return 0;
}
