[![✗](https://img.shields.io/badge/Release-v2.0.0-ffb600.svg?style=for-the-badge)](https://github.com/SrMathew/Propogate)

[![✗](https://github.com/SrMathew/Propogate/actions/workflows/pipeline.yaml/badge.svg?branch=production)](https://github.com/SrMathew/Propogate/actions/workflows/pipeline.yaml)

# Propogate

Un generador de diagramas de compuertas lógicas a partir de fórmulas de primer orden mediante archivos LaTeX, desarrollado con Flex y Bison.

* [Notas de la versión](#notas-de-la-versión)
* [Requisitos](#requisitos)
* [Configuración](#configuración)
* [Commands](#commands)
* [CI/CD](#cicd)
* [Recommended Extensions](#recommended-extensions)

## Notas de la versión

Respecto a la segunda entrega, se solucionaron todos los errores marcados:
- Warnings al construir: se eliminó la calculadora del proyecto original de CMakeLists.txt.
- Detalles de instalación y configuración: se restauraron y tradujeron parcialmente para ser consistentes en el idioma del README.
- Parseo de LaTeX: se redefinieron los patrones y la gramática para parsear LaTeX de forma apropiada. Cabe destacar que estos cambios fueron los protagonistas de esta última entrega.
- Formateador: estábamos usándolo pero la entrega no tenía todos los archivos formateados. Esta vez se aplicó a todos los archivos que modificamos, y al final del README se encuentra el utilizado y su configuración.
- Se corrigió el uso de booleanos (fue una confusión con el TP de Sistemas Operativos)
- Se desabreviaron los nombres de algunas variables
- Se corrigió la precedencia entre AND y OR
- Se corrigió el uso de \__FUNCTION__ en los logueadores.

## Requisitos

* [Docker v28.3.2](https://www.docker.com/)

## Configuración

(Créditos a [Agustín Golmar](https://github.com/agustin-golmar) por esta sección)
Configurar las siguientes variables de entorno para controlar el comportamiento del proyecto:

| Nombre                | Valor por defecto | Descripción                                                                                                                                                           |
| :-------------------- | :---------------: | :-------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `ENVIRONMENT`         | `Local`           | The active environment name. The available environments are: `Local`, `Development` and `Production`.                                                                 |
| `LOG_IGNORED_LEXEMES` | `true`            | When `true`, logs all of the ignored lexemes found with Flex at `DEBUGGING` level. To remove those logs from the console output set it to `false`.                    |
| `LOGGING_LEVEL`       | `ALL`             | The minimum level to log in the console output. From lower to higher, the available levels are: `ALL`, `DEBUGGING`, `INFORMATION`, `WARNING`, `ERROR` and `CRITICAL`. |

_Docker Compose_ can read the variables from an `.env` file too (see `compose.yaml` file).

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

### Docker

(Créditos a [Agustín Golmar](https://github.com/agustin-golmar) por esta sección)

| Comando                                 | Descripción                                             |
| :-------------------------------------- | :------------------------------------------------------ |
| `docker builder prune --all`            | Removes all builds and complete build cache.            |
| `docker compose --progress=plain build` | Forces a build or rebuild of the images in the cluster. |
| `docker image prune`                    | Removes all of the dangling images from Docker.         |
| `docker network prune`                  | Removes unused networks from Docker.                    |
| `docker volume prune`                   | Removes unused volumes from Docker.                     |

## CI/CD

(Créditos a [Agustín Golmar](https://github.com/agustin-golmar) por esta sección)
Para disparar una integración automática en cada push o PR (_Pull Request_), activar _GitHub Actions_ en la pestaña _Settings_. Usar la siguiente configuración:

| Clave                                                      | Valor                                               |
| :--------------------------------------------------------- | :-------------------------------------------------- |
| `Actions permissions`                                      | `Allow all actions and reusable workflows`          |
| `Allow GitHub Actions to create and approve pull requests` | `false`                                             |
| `Artifact and log retention`                               | `30 days`                                           |
| `Fork pull request workflows from outside collaborators`   | `Require approval for all outside collaborators`    |
| `Workflow permissions`                                     | `Read repository contents and packages permissions` |

## Extensiones recomendadas

* [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
* [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
* [Yash](https://marketplace.visualstudio.com/items?itemName=daohong-emilio.yash)
* [Prettier](https://marketplace.visualstudio.com/items?itemName=esbenp.prettier-vscode) (el proyecto utiliza la configuración predeterminada de formateo)