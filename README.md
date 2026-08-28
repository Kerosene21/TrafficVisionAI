# TrafficVisionAI

Sistema de visión artificial orientado al análisis y comprensión inteligente de escenas de tráfico.

## Objetivo general

TrafficVisionAI es un proyecto universitario cuyo objetivo es construir, de forma progresiva, una
aplicación de escritorio en C++ capaz de procesar imágenes y vídeo de tráfico y extraer información
útil de la escena. El desarrollo avanza por checkpoints: cada etapa incorpora una capacidad nueva
sobre una base ya verificada, empezando por el procesamiento clásico de imagen y avanzando después
hacia técnicas de detección y análisis más elaboradas.

Este repositorio contiene únicamente lo que ya está implementado y comprobado. Las capacidades
listadas en el [Roadmap](#roadmap) todavía no existen en el código.

## Estado actual

**Checkpoint 0 — bootstrap técnico.**

Lo único implementado en este momento es la base del proyecto y una aplicación de consola mínima
que verifica que la cadena de compilación (CMake + vcpkg + MSVC) y la integración con OpenCV
funcionan correctamente. La aplicación permite cargar y visualizar tres tipos de entrada —imagen,
vídeo y webcam— e imprimir sus propiedades básicas.

No hay todavía ningún procesamiento de imagen, detección, seguimiento ni análisis de tráfico.

## Tecnologías actuales

| Componente | Uso en el proyecto |
|---|---|
| C++17 | Lenguaje de la aplicación |
| Visual Studio (MSVC, x64) | Compilador y entorno de desarrollo |
| CMake | Sistema de construcción |
| vcpkg (modo manifiesto) | Gestión de dependencias |
| OpenCV (`opencv4`) | Lectura y visualización de imagen y vídeo |

## Requisitos

- Windows de 64 bits.
- Visual Studio con la carga de trabajo **Desarrollo para el escritorio con C++**, que incluye:
  - el compilador MSVC para x64,
  - CMake,
  - Ninja,
  - vcpkg.
- Git.
- Variable de entorno `VCPKG_ROOT` apuntando a la instalación de vcpkg. El *Developer Command
  Prompt* / *Developer PowerShell* de Visual Studio la define automáticamente.
- Conexión a internet la primera vez, para que vcpkg descargue y compile OpenCV.
- Espacio libre en disco: la compilación de OpenCV desde código fuente necesita varios GB
  temporales.

> No es necesario instalar OpenCV manualmente ni descargar binarios: vcpkg lo resuelve a partir de
> `vcpkg.json`.

## Configuración

Las dependencias se declaran en [`vcpkg.json`](vcpkg.json) y se resuelven automáticamente durante la
configuración de CMake. La ruta al *toolchain* de vcpkg se toma de la variable de entorno
`VCPKG_ROOT` desde [`CMakePresets.json`](CMakePresets.json), de modo que el repositorio no contiene
ninguna ruta absoluta de una máquina concreta.

El triplet utilizado es `x64-windows`.

Todos los comandos deben ejecutarse desde una consola con el entorno de Visual Studio cargado
(**Developer PowerShell for VS** o **Developer Command Prompt for VS**), que es la que expone
`cl.exe`, `cmake`, `ninja` y `VCPKG_ROOT`.

### Opción A — Visual Studio

1. Clonar el repositorio.
2. Abrir la carpeta del repositorio con **Archivo → Abrir → Carpeta**.
3. Visual Studio detecta `CMakePresets.json` y ofrece las configuraciones `x64-debug` y
   `x64-release`.
4. Seleccionar una configuración y compilar.

### Opción B — Línea de comandos

```powershell
git clone https://github.com/Kerosene21/TrafficVisionAI.git
cd TrafficVisionAI
```

## Compilación

Desde una **Developer PowerShell for VS**, en la raíz del repositorio:

```powershell
cmake --preset x64-release
cmake --build --preset x64-release
```

Para una compilación de depuración:

```powershell
cmake --preset x64-debug
cmake --build --preset x64-debug
```

La primera configuración tarda considerablemente, porque vcpkg descarga y compila OpenCV y sus
dependencias. Las siguientes reutilizan lo ya construido.

El ejecutable se genera en:

```text
build/x64-release/TrafficVisionAI.exe
```

Los directorios de compilación (`build/`) y las dependencias instaladas por vcpkg
(`vcpkg_installed/`) están excluidos del control de versiones.

## Ejecución

La aplicación es una herramienta de consola con tres modos de entrada:

```text
TrafficVisionAI image <ruta>
TrafficVisionAI video <ruta>
TrafficVisionAI camera [indice]
```

### Modo imagen

Carga la imagen indicada, la muestra en una ventana e imprime su ancho, alto y número de canales.
La ventana se cierra al pulsar cualquier tecla.

```powershell
.\build\x64-release\TrafficVisionAI.exe image C:\ruta\a\imagen.jpg
```

### Modo vídeo

Abre el archivo de vídeo, imprime los FPS y la resolución declarados por la fuente y reproduce los
frames en una ventana. Termina al acabar el archivo o al pulsar `ESC`.

```powershell
.\build\x64-release\TrafficVisionAI.exe video C:\ruta\a\video.mp4
```

### Modo cámara

Abre una webcam, imprime su resolución y muestra los frames de forma continua. Se cierra con `ESC`.
Si no se indica índice, se usa el `0`.

```powershell
.\build\x64-release\TrafficVisionAI.exe camera
```

```powershell
.\build\x64-release\TrafficVisionAI.exe camera 1
```

## Roadmap

Las siguientes capacidades **todavía no están implementadas**. Se incorporarán progresivamente en
checkpoints posteriores:

- procesamiento avanzado de imagen y vídeo;
- representación e iluminación;
- transformaciones afines y de perspectiva, vista cenital;
- modelo y calibración de cámara;
- filtrado, histogramas, umbralización y binarización;
- morfología matemática;
- segmentación y caracterización;
- detección clásica de vehículos y personas;
- detección mediante Deep Learning (YOLO), exportación a ONNX e inferencia desde C++;
- seguimiento con identificadores persistentes y trayectorias;
- análisis vial: conteo, velocidad, dirección, densidad y congestión;
- análisis en condiciones difíciles de iluminación y meteorología;
- detección de eventos y anomalías viales;
- comparación experimental entre visión clásica y Deep Learning;
- interfaz gráfica.

## Estructura del repositorio

```text
TrafficVisionAI/
├── CMakeLists.txt       Definición del proyecto y del ejecutable
├── CMakePresets.json    Configuraciones de compilación x64 (toolchain de vcpkg)
├── vcpkg.json           Manifiesto de dependencias (OpenCV)
├── .gitignore           Exclusión de artefactos locales de compilación
├── README.md
└── src/
    └── main.cpp         Aplicación de consola: modos imagen, vídeo y cámara
```
