# Puerto de Contenedores — Operación Contrarreloj

Trabajo Práctico de **Algoritmos y Estructuras de Datos (3640)** — UNLaM, comisiones 02-3300 y 03-3600.

Simulación interactiva en C de un puerto de contenedores: el usuario asume el rol de Operador y decide, minuto a minuto, cómo descargar buques, reorganizar zonas de almacenamiento y entregar contenedores a camiones, contrarreloj.

## Grupo: Apuntados 2.0

| Apellido y Nombre |
|---|
| Aristimuño, Iara Belén |
| Navarro Ojeda, Camila Micaela |
| Schiaffino, Lautaro Leonardo Nicolás |
| Venier, Agustín |
| Puma Ocampo, Florencia |

## Estructura del repositorio

```text
.
├── Archivos/          # Persistencia: config.txt, puerto.txt, .bin, .idx
├── include/            # Headers (.h)
└── src/                 # Código fuente (.c)
```

## Requisitos

- GCC compatible con C99
- Code::Blocks (opcional, para abrir el `.cbp`)

## Ejecución

Al iniciar se solicita el nombre del operador (se da de alta automáticamente si no existe) y se presenta el menú principal:

```text
1. Iniciar nueva jornada
2. Ver ranking de operadores
3. Salir
```

Durante una jornada, los comandos disponibles son:

| Comando | Sintaxis | Acción |
|---|---|---|
| `DES` | `DES <muelle> <zona>` | Descargar el próximo contenedor del buque atracado en `<muelle>` hacia `<zona>` |
| `REU` | `REU <zona_origen> <zona_destino>` | Reubicar el contenedor en el tope de `<zona_origen>` hacia `<zona_destino>` |
| `ENT` | `ENT` | Entregar el contenedor solicitado al camión al frente de la cola |
| `VER` | `VER` | Ver el estado actual (no consume tiempo) |
| `ESP` | `ESP` | Esperar un minuto |

## Persistencia

- `Archivos/config.txt` — parámetros de configuración de la simulación.
- `Archivos/puerto.txt` — escenario de la jornada (buques, camiones, manifiestos).
- `Archivos/operadores.bin` + `Archivos/operadores.idx` — registro de operadores, indexado con un Árbol Binario de Búsqueda persistido.
- `Archivos/jornadas.bin` — historial de jornadas jugadas.

## Entrega

- **Fecha límite:** 11/11/2026 23:59, vía plataforma MIeL.
- Formato: `TP_ALGORITMOS_2026_C2_0X-3Y00_{NOMBRE_DEL_GRUPO}.zip` + URL de este repositorio (público).
