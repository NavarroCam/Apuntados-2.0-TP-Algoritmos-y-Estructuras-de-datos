/* ============================================================
   include/config.h
   Parametros de configuracion inicial (Archivos/config.txt).
   ============================================================ */
#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#define RUTA_CONFIG "Archivos/config.txt"

#define TAM_NOMBRE_OPERADOR 31

/* Limites de saneamiento de los parametros */
#define MIN_DURACION_JORNADA        5u
#define MAX_DURACION_JORNADA    100000u
#define MIN_MUELLES                 1u
#define MAX_MUELLES                20u
#define MIN_ZONAS                   2u
#define MAX_ZONAS                  20u
#define MIN_CAPACIDAD_PILA          1u
#define MAX_CAPACIDAD_PILA         50u
#define MIN_BUQUES                  1u
#define MAX_BUQUES                 90u
#define MIN_CONT_POR_BUQUE          1u
#define MAX_CONT_POR_BUQUE         40u
#define MIN_CAMIONES                1u
#define MAX_CAMIONES              500u
#define MIN_TIEMPO_OPERACION        1u
#define MAX_TIEMPO_OPERACION      999u

typedef struct
{
    unsigned duracionJornadaMinutos;
    unsigned cantidadMuelles;
    unsigned cantidadZonasAlmacenamiento;
    unsigned capacidadPila;
    unsigned maximoBuques;
    unsigned maximoContenedoresPorBuque;
    unsigned maximoCamiones;
    unsigned tiempoDescargaContenedor;
    unsigned tiempoReubicacionContenedor;
    unsigned tiempoCargaCamion;
} tConfiguracion;

void establecerConfiguracionPorDefecto(tConfiguracion *configuracion);
int cargarConfiguracion(tConfiguracion *configuracion, const char *ruta);
int guardarConfiguracion(const tConfiguracion *configuracion, const char *ruta);
int validarConfiguracion(tConfiguracion *configuracion);
void mostrarConfiguracion(const tConfiguracion *configuracion);

#endif /* CONFIG_H_INCLUDED */
