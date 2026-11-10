#ifndef PANTALLA_H_INCLUDED
#define PANTALLA_H_INCLUDED

#include "comun.h"

#define INTERFAZ_ANSI   1

#if INTERFAZ_ANSI
#define RESET       "\033[0m"
#define NEGRITA     "\033[1m"
#define ROJO        "\033[31m"
#define VERDE       "\033[32m"
#define AMARILLO    "\033[33m"
#define AZUL        "\033[34m"
#define MAGENTA     "\033[35m"
#define CIAN        "\033[36m"
#define GRIS        "\033[37m"
#else
#define RESET       ""
#define NEGRITA     ""
#define ROJO        ""
#define VERDE       ""
#define AMARILLO    ""
#define AZUL        ""
#define MAGENTA     ""
#define CIAN        ""
#define GRIS        ""
#endif

#define ANCHO_PANTALLA      67
#define TAM_LINEA_PANTALLA  128

void limpiarPantalla();
void pausarPantalla();
void mostrarBorde(char relleno, const char *color);
void mostrarTitulo(const char *titulo, const char *color);
void mostrarSeccion(const char *nombre);

#endif /* PANTALLA_H_INCLUDED */
