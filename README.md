[![✗](https://img.shields.io/badge/Release-v2.0.0-ffb600.svg?style=for-the-badge)](https://github.com/SrMathew/Propogate)

[![✗](https://github.com/SrMathew/Propogate/actions/workflows/pipeline.yaml/badge.svg?branch=production)](https://github.com/SrMathew/Propogate/actions/workflows/pipeline.yaml)

# Propogate

Un generador de diagramas lógicos a partir de fórmulas de primer orden mediante archivos LaTeX, desarrollado con Flex y Bison.

## Notas de la versión

El proyecto cuenta con varios errores, a notar:
- No están implementadas las funcionalidades de:
    - definir fórmulas (caso de uso del test 8)
    - asignar valores
    - definir múltiples fórmulas o variables individualmente (caso de uso de los tests 7 y 2 respectivamente)
    - utilizar un conector unario con una variable sin el uso de paréntesis (caso de uso el test 3, probablemente sea descartado)
    - indicar un archivo LaTeX donde no se usa el comando especificado
    - exigir el uso del comando (\newcommand{...} en el archivo LaTeX) 
- Hay un problema de memory leak que causa que la ejecución de todos los tests fallen, aunque sean exitosos.
- Los tests de rechazo 1, 2, 3, 9 rechazan por motivos explicitados anteriormente y no por su definición.

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
