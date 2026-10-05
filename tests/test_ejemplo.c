/* TEST DEL MODULO 4: Historial y Puntuacion
 ------------------------------------------------------------------
 Reproduce el ejemplo de la consigna (pags. 12 a 17): carga a mano
 las 11 operaciones de la partida y verifica que el puntaje final
 sea 60 (5 entregas x 10 + 2 buques x 5 - 0 camiones pendientes).

 IMPORTANTE: este archivo tiene su propio main(), por eso NO debe
 agregarse al proyecto puertoContenedores.cbp (daria error
 "multiple definition of main"). Si se agrego por error:
 clic derecho sobre el archivo > "Remove file from project".

 COMO EJECUTARLO
 1. Abrir Git Bash en la carpeta raiz del repo (donde esta main.c),
    NO dentro de la carpeta tests.

 2. Compilar y ejecutar con este comando:
 gcc -Wall -Wextra tests/test_ejemplo.c src/historial.c src/puntuacion.c src/lista.c src/comun.c -o test_ejemplo && ./test_ejemplo

 Si aparece "gcc: command not found", usar el gcc de Code::Blocks con la ruta completa:
"/c/Program Files/CodeBlocks/MinGW/bin/gcc.exe" -Wall -Wextra tests/test_ejemplo.c src/historial.c src/puntuacion.c src/lista.c src/comun.c -o test_ejemplo && ./test_ejemplo

 3. Resultado esperado: se muestra el historial de operaciones, el resumen de la jornada y al final la linea:
[OK] puntaje obtenido 60, esperado 60
 Si dice [ERROR], algo cambio en historial.c o puntuacion.c.

NOTA: el comando genera test_ejemplo.exe en la raiz del repo.
No se sube a GitHub porque el .gitignore ignora los .exe.
 */

#include "../include/historial.h"
#include "../include/puntuacion.h"

#define PUNTAJE_ESPERADO 60

int main(void)
{
    tHistorial historial;
    tPuntuacion puntos;
    int puntaje;

    crearHistorial(&historial);
    iniciarPuntuacion(&puntos, 30);

    registrarOperacion(&historial,  0,  2, "DES", "C101", "M1", "Z1");
    registrarOperacion(&historial,  2,  4, "DES", "C102", "M1", "Z1");
    registrarOperacion(&historial,  4,  5, "REU", "C102", "Z1", "Z2");
    registrarReorganizacion(&puntos);
    registrarOperacion(&historial,  5,  7, "ENT", "C101", "Z1", "K001");
    registrarEntrega(&puntos);
    registrarOperacion(&historial,  7,  9, "DES", "C103", "M1", "Z3");
    registrarBuqueDescargado(&puntos);              // B001 completo
    registrarOperacion(&historial,  9, 11, "ENT", "C103", "Z3", "K002");
    registrarEntrega(&puntos);
    registrarOperacion(&historial, 11, 13, "ENT", "C102", "Z2", "K003");
    registrarEntrega(&puntos);
    registrarOperacion(&historial, 13, 15, "DES", "C201", "M1", "Z1");
    registrarOperacion(&historial, 15, 17, "ENT", "C201", "Z1", "K004");
    registrarEntrega(&puntos);
    registrarOperacion(&historial, 17, 19, "DES", "C202", "M1", "Z1");
    registrarBuqueDescargado(&puntos);              // B002 completo
    registrarOperacion(&historial, 19, 21, "ENT", "C202", "Z1", "K005");
    registrarEntrega(&puntos);

    cerrarPuntuacion(&puntos, 21, 0);

    mostrarHistorial(&historial, stdout);
    mostrarResumenFinal(&puntos, stdout);

    puntaje = calcularPuntajeFinal(&puntos);
    printf("\n  [%s] puntaje obtenido %d, esperado %d\n", puntaje == PUNTAJE_ESPERADO ? "OK" : "ERROR", puntaje, PUNTAJE_ESPERADO);

    vaciarHistorial(&historial);

    return puntaje == PUNTAJE_ESPERADO ? 0 : 1;
}
