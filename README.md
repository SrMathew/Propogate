[![✗](https://img.shields.io/badge/Release-v2.0.0-ffb600.svg?style=for-the-badge)](https://github.com/SrMathew/Propogate)

[![✗](https://github.com/SrMathew/Propogate/actions/workflows/pipeline.yaml/badge.svg?branch=production)](https://github.com/SrMathew/Propogate/actions/workflows/pipeline.yaml)

# Propogate

Un generador de diagramas de compuertas lógicas a partir de fórmulas de primer orden mediante archivos LaTeX, desarrollado con Flex y Bison.

## Notas de la versión

Respecto a la especificación original, el proyecto presenta los siguientes cambios:
- Se descartó el caso de aceptación 3: definir una fórmula como una variable negada sin uso de paréntesis (`\alpha = \neg p`).
    - El motivo yace en que se deben realizar dos operaciones (definir la variable como fórmula, y aplicar el operador unario) y en la lógica propocisional esta escritura es un abuso de notación, ya que correctamente debería tener paréntesis.
    - Se reemplazó por un caso de uso donde se anidan operadores unarios.
- Se descartó el caso de rechazo 9: uso de un documento LaTeX mal formado.
    - El motivo yace en que no es nuestra responsabilidad determinar si un LaTeX es válido o no.
- Se permite utilizar fórmulas anónimas y variables sin valor asignado. Por el momento, esto se considera temporal.

### Limitaciones

Por el momento, las variables y fórmulas sólo pueden llamarse con letras minúsculas latinas o griegas respectivamente, a su vez múltiples para las variables y una sola para las fórmulas. Esto se extenderá al uso de subíndices en una futura versión.
A su vez, aunque se puedan definir variables y fórmulas, estas no pueden reutilizarse aún.

### Versiones futuras

Está planificado implementar un alcance sobre las distintas secciones del documento LaTeX para las variables y fórmulas definidas (es decir, su definición puede cambiar entre secciones). Además, se planea permitir la identificación de compuertas lógicas que no sean un mapeo directo a los operadores lógicos de primer orden.

## Requisitos

* [Docker v28.3.2](https://www.docker.com/)

## Comandos

### Inicio

Inicia un contenedor temporal:

```bash
docker compose run --rm compiler
```

### Compilación

Construye el compilador:

```bash
src/main/bash/build.sh
```

### Ejecución

Compila un programa:

```bash
src/main/bash/run.sh <program>
```

donde `<program>` es el path al archivo entry-point.

### Tests

Ejecuta todos los tests disponibles en el directorio `src/test/c`:

```bash
src/main/bash/test.sh
```

### Stop

Cierra sesión, destruye los contenedores creados, y apaga el cluster:

```bash
exit
docker compose down
```
