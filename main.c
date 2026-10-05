#include <stdio.h>
#include <stdlib.h>
#include "./include/comun.h"
#include "./include/cola.h"
#include "./include/pila.h"
#include "./include/lista.h"

#include "include/config.h"
#include "include/interfaz.h"

int main()
{
    tConfiguracion config;
    char nombreOperador[TAM_NOMBRE_OPERADOR];
    int opcion;

    if(!cargarConfiguracion(&config, ARCH_CONFIG))
    {
        printf("\n   [aviso] No se encontro %s. Se utilizaran los valores por defecto.\n", ARCH_CONFIG);
        if(guardarConfiguracion(&config, ARCH_CONFIG))
            printf("   Se genero %s con la configuracion por defecto.\n", ARCH_CONFIG);
        else
            printf("   [error] Tampoco fue posible crear %s (verifique que exista la carpeta Archivos/).\n", ARCH_CONFIG);
    }

    pedirNombreOperador(nombreOperador, sizeof(nombreOperador));
    mostrarBienvenida(nombreOperador);

    do{
        opcion = mostrarMenuPrincipal();
        switch(opcion)
        {
            case OPC_NUEVA_JORNADA:
                mostrarConfiguracion(&config);
                mostrarPendiente("La jornada (modulos 2 y 3)");
                break;

            case OPC_VER_RANKING:
                mostrarPendiente("El ranking de operadores (modulo 5)");
                break;

            case OPC_SALIR:
                printf("\n  Hasta luego, %s.\n", nombreOperador);
                break;
        }
    } while(opcion != OPC_SALIR);

    return EXIT_SUCCESS;
}

