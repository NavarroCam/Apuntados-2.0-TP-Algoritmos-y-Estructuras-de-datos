#include <stdio.h>
#include <stdlib.h>
#include "./include/comun.h"
#include "./include/cola.h"
#include "./include/pila.h"
#include "./include/lista.h"

#include "include/config.h"

int main()
{
    tConfiguracion config;
    if(!cargarConfiguracion(&config, ARCH_CONFIG))
    {
        printf("\n   [aviso] No se encontro %s. Se utilizaran los valores por defecto.\n", ARCH_CONFIG);
        if(guardarConfiguracion(&config, ARCH_CONFIG))
            printf("   Se genero %s con la configuracion por defecto.\n", ARCH_CONFIG);
        else
            printf("   [error] Tampoco fue posible crear %s (verifique que exista la carpeta Archivos/).\n",
                   ARCH_CONFIG);
    }

    mostrarConfiguracion(&config);

    return 0;
}

