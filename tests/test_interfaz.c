/* TEST DE LA INTERFAZ (modulo 4): normalizacion de ordenes
------------------------------------------------------------------
Verifica que normalizarOrden deje las ordenes listas para el
modulo 3: sin espacios al principio ni al final, un solo espacio
entre palabras y todo en mayusculas.

IMPORTANTE: tiene su propio main(), NO agregarlo al proyecto puertoContenedores.cbp.

COMO EJECUTARLO (Git Bash, en la carpeta raiz del repo):
gcc -Wall -Wextra tests/test_interfaz.c src/interfaz.c src/config.c src/historial.c src/puntuacion.c src/lista.c src/comun.c -o test_interfaz && ./test_interfaz

Resultado esperado: todas las lineas con [OK] y al final
"Pasaron 6 de 6 pruebas".*/


#include "../include/interfaz.h"

#define CANT_PRUEBAS 6

int main(void)
{
    const char *entradas[CANT_PRUEBAS] =
    {
        "DES M1 Z1",
        "des m1 z1",
        "   reu   z1    z2   ",
        "\tEnt\t",
        "ver",
        "      "
    };
    const char *esperadas[CANT_PRUEBAS] =
    {
        "DES M1 Z1",
        "DES M1 Z1",
        "REU Z1 Z2",
        "ENT",
        "VER",
        ""
    };
    char orden[TAM_ORDEN];
    int i;
    int correctas = 0;

    printf("\n  TEST normalizarOrden\n");
    printf("  ---------------------------------------------\n");
    for(i = 0; i < CANT_PRUEBAS; i++)
    {
        strcpy(orden, entradas[i]);
        normalizarOrden(orden);
        if(strcmp(orden, esperadas[i]) == 0)
        {
            correctas++;
            printf("  [OK]    \"%s\"\n", orden);
        }
        else
            printf("  [ERROR] se obtuvo \"%s\", se esperaba \"%s\"\n",
                   orden, esperadas[i]);
    }
    printf("  ---------------------------------------------\n");
    printf("  Pasaron %d de %d pruebas\n", correctas, CANT_PRUEBAS);

    return correctas == CANT_PRUEBAS ? EXIT_SUCCESS : EXIT_FAILURE;
}
