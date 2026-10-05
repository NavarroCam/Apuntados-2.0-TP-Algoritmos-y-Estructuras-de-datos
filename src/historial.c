#include "../include/historial.h"

void copiarCampo(char *dest, const char *orig, unsigned tam)
{
    if(orig == NULL || *orig == '\0')
        orig = SIN_DATO;
    strncpy(dest, orig, tam - 1);
    dest[tam - 1] = '\0';
}

void mostrarOperacion(const void *dato, FILE *pf)
{
    const tOperacion *op = (const tOperacion*)dato;
    char tiempos[24];

    sprintf(tiempos, "T=%u -> T=%u", op->inicio, op->fin);
    fprintf(pf, "  %-16s %-5s %-11s %-8s %-8s\n", tiempos, op->codigo, op->contenedor, op->origen, op->destino);
}

void crearHistorial(tHistorial *h)
{
    crearLista(h);
}

int registrarOperacion(tHistorial *h, unsigned inicio, unsigned fin, const char *codigo, const char *contenedor, const char *origen, const char *destino)
{
    tOperacion op;

    op.inicio = inicio;
    op.fin = fin;
    copiarCampo(op.codigo, codigo, TAM_COD_OPERACION);
    copiarCampo(op.contenedor, contenedor, TAM_COD_CONTENEDOR);
    copiarCampo(op.origen, origen, TAM_RECURSO);
    copiarCampo(op.destino, destino, TAM_RECURSO);

    return ponerAlFinal(h, &op, sizeof(tOperacion)) ? TODO_BIEN : SIN_MEM;
}

void mostrarHistorial(const tHistorial *h, FILE *pf)
{
    fprintf(pf, "\n  HISTORIAL DE OPERACIONES\n");
    fprintf(pf, "  ---------------------------------------------------------\n");
    fprintf(pf, "  %-16s %-5s %-11s %-8s %-8s\n", "Tiempo", "Op", "Contenedor", "Origen", "Destino");
    fprintf(pf, "  ---------------------------------------------------------\n");
    if(listaVacia(h))
        fprintf(pf, "  (no se realizaron operaciones)\n");
    else
        mostrarLista(h, mostrarOperacion, pf);
    fprintf(pf, "  ---------------------------------------------------------\n");
}

void vaciarHistorial(tHistorial *h)
{
    vaciarLista(h);
}
