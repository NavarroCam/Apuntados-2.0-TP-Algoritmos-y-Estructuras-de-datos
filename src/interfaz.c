#include "../include/interfaz.h"

int leerLinea(char *dest, unsigned tam)
{
    char *salto;
    int c;
    int resultado = LECTURA_OK;

    if(fgets(dest, tam, stdin) == NULL)
    {
        *dest = '\0';
        resultado = FIN_ENTRADA;
    }
    else
    {
        salto = strchr(dest, '\n');
        if(salto != NULL)
            *salto = '\0';
        else
        {
            c = getchar();
            while(c != '\n' && c != EOF)
                c = getchar();
        }
    }
    return resultado;
}

void pedirNombreOperador(char *nombre, unsigned tam)
{
    char linea[TAM_ORDEN];
    char *limpio;
    const char *origen;
    int estado;

    do{
        printf("\nIngrese su nombre de operador: ");
        estado = leerLinea(linea, sizeof(linea));
        limpio = recortarCadena(linea);

        if(estado == LECTURA_OK && *limpio == '\0')
            printf("[Error] El nombre no puede quedar vacio.\n");
    } while(estado == LECTURA_OK && *limpio == '\0');

    origen = (*limpio != '\0') ? limpio : NOMBRE_POR_DEFECTO;
    strncpy(nombre, origen, tam - 1);
    nombre[tam - 1] = '\0';
}

void mostrarBienvenida(const char *nombre)
{
    printf("\n======================================================\n");
    printf("    PUERTO DE CONTENEDORES - OPERACION CONTRARRELOJ\n");
    printf("======================================================\n");
    printf("Bienvenido/a, %s\n", nombre);
}

int mostrarMenuPrincipal()
{
    char linea[TAM_ORDEN];
    char *opcion;
    int elegida = OPC_INVALIDA;

    do{
        printf("\n  MENU PRINCIPAL\n");
        printf("------------------------------------------------------\n");
        printf("%d. Iniciar nueva jornada\n", OPC_NUEVA_JORNADA);
        printf("%d. Ver ranking de operadores\n", OPC_VER_RANKING);
        printf("%d. Salir\n", OPC_SALIR);
        printf("------------------------------------------------------\n");
        printf("Opcion: ");

        if(leerLinea(linea, sizeof(linea)) == FIN_ENTRADA)
            elegida = OPC_SALIR;
        else
        {
            opcion = recortarCadena(linea);
            if(opcion[0] >= '0' + OPC_NUEVA_JORNADA && opcion[0] <= '0' + OPC_SALIR && opcion[1] == '\0')
                elegida = opcion[0] - '0';
            else
                printf("[Error] Opcion invalida. Ingrese un numero del %d al %d.\n", OPC_NUEVA_JORNADA, OPC_SALIR);
        }
    } while(elegida == OPC_INVALIDA);

    return elegida;
}

void mostrarPendiente(const char *funcionalidad)
{
    printf("\n[Pendiente] %s todavia no esta implementado.\n", funcionalidad);
}

void normalizarOrden(char *orden)
{
    char *lect = orden;
    char *escr = orden;
    int hayEspacio = 0;

    while(*lect != '\0' && isspace((unsigned char)*lect))
        lect++;

    while(*lect != '\0')
    {
        if(isspace((unsigned char)*lect))
            hayEspacio = 1;
        else
        {
            if(hayEspacio)
            {
                *escr = ' ';
                escr++;
                hayEspacio = 0;
            }
            *escr = (char)toupper((unsigned char)*lect);
            escr++;
        }
        lect++;
    }
    *escr = '\0';
}

int leerOrden(char *orden, unsigned tam)
{
    int estado;

    printf("\n  %s", PROMPT_OPERADOR);
    estado = leerLinea(orden, tam);
    if(estado == LECTURA_OK)
        normalizarOrden(orden);
    return estado;
}

void mostrarOrdenRechazada(const char *motivo)
{
    printf("[Orden rechazada] %s\n", motivo);
    printf("El reloj no avanzo.\n");
}

void mostrarFinJornada(int motivo, const tHistorial *h, const tPuntuacion *p)
{
    printf("\n======================================================\n");
    printf("  FIN DE LA JORNADA\n");
    printf("======================================================\n");

    switch(motivo)
    {
        case FIN_POR_TIEMPO:
            printf("Se alcanzo la duracion maxima de la jornada.\n");
            break;

        case FIN_NORMAL:
            printf("Finalizacion anticipada normal: no quedan eventos,\n");
            printf("buques con carga pendiente ni camiones esperando.\n");
            break;

        case FIN_POR_BLOQUEO:
            printf("Finalizacion por bloqueo operativo: no hay operaciones\n");
            printf("validas ni eventos futuros pendientes.\n");
            break;

        default:
            printf("Finalizacion de la jornada.\n");
            break;
    }

    mostrarHistorial(h, stdout);
    mostrarResumenFinal(p, stdout);
}
