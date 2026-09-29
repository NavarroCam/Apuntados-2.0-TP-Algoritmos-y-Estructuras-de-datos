#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#define ARCH_CONFIG "Archivos/config.txt"

#define TAM_NOMBRE_OPERADOR 31
#define TAM_LINEA_CONFIG 256

#define ERROR_APERTURA 0
#define TODO_OK 1

#define ES_DIGITO(c) (((c) >= '0') && ((c) <= '9'))

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
int cargarConfiguracion(tConfiguracion *config, const char *nomArch);
int guardarConfiguracion(const tConfiguracion *config, const char *nomArch);
void mostrarConfiguracion(const tConfiguracion *configuracion);

char* recortarCadena(char *cadena);
void convertMinusCadena(char *cadena);
void asignarParametro(tConfiguracion *configuracion, const char *clave, unsigned valor);
int esNumeroPos(const char* cad);
#endif // CONFIG_H_INCLUDED
