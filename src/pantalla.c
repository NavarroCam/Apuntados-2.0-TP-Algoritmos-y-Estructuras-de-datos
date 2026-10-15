#include "../include/pantalla.h"

/* fflush antes de system(): si no, lo que quedo en el buffer de printf se
   muestra despues del comando. */
void limpiarPantalla(void)
{
    fflush(stdout);
    system("cls");
}

void pausarPantalla(void)
{
    fflush(stdout);
    system("pause");
}

/* +======...======+ del ancho de la pantalla, en el color indicado. */
void mostrarBorde(char relleno, const char *color)
{
    int i;

    printf("%s+", color);
    for (i = 0; i < ANCHO_PANTALLA; i++)
        putchar(relleno);
    printf("+%s\n", RESET);
}

/* Titulo centrado dentro de una caja. */
void mostrarTitulo(const char *titulo, const char *color)
{
    int largo = strlen(titulo);
    int margen = (ANCHO_PANTALLA - largo) / 2;

    mostrarBorde('=', color);
    printf("%s|%s%s%*s%s%*s%s|%s\n", color, RESET, NEGRITA, margen + largo, titulo,
           RESET, ANCHO_PANTALLA - margen - largo, "", color, RESET);
    mostrarBorde('=', color);
}

/* Encabezado de una seccion dentro de una pantalla: "-- NOMBRE ------". */
void mostrarSeccion(const char *nombre)
{
    int i, largo = strlen(nombre);

    printf("%s-- %s%s%s ", GRIS, RESET NEGRITA, nombre, RESET GRIS);
    for (i = largo + 2; i < ANCHO_PANTALLA; i++)
        putchar('-');
    printf("%s\n", RESET);
}

/* Devuelve 0 si se termino la entrada. */
int esperarEnter(void)
{
    char linea[TAM_LINEA_PANTALLA];

    printf("\n%s  Presione ENTER para volver al menu...%s", GRIS, RESET);
    return fgets(linea, sizeof(linea), stdin) != NULL;
}

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
