
# Graficacomputacional3D

Motor gráfico 3D desarrollado en C++ utilizando DirectX 11, como parte del aprendizaje y desarrollo de proyectos de gráficos computacionales.

## Descripción

Este proyecto consiste en el desarrollo de un motor gráfico 3D desde cero, utilizando C++ y DirectX 11 para comprender los fundamentos del renderizado, la creación de ventanas y la representación de objetos tridimensionales.

El proyecto se encuentra en desarrollo y tiene como objetivo implementar progresivamente diferentes funcionalidades gráficas.

## Tecnologías utilizadas

- **C++** — Lenguaje principal del proyecto.
- **DirectX 11** — API gráfica para el renderizado 3D.
- **Visual Studio** — Entorno de desarrollo y compilación.
- **HLSL** — Lenguaje utilizado para desarrollar shaders gráficos.

## Estructura del proyecto

```text
Graficacomputacional3D/
│
├── Engine/
│   ├── API.h
│   ├── Engine.h
│   └── Engine.cpp
│
├── Sandbox/
│   └── main.cpp
│
├── Window.h
├── Window.cpp
│
├── shaders/
│   └── Cube.hlsl
│
└── README.md
```

*La estructura puede cambiar conforme avance el desarrollo del motor.*

## Características

- Creación y administración de ventanas mediante Win32.
- Inicialización de DirectX 11.
- Configuración de dispositivos y contexto gráfico.
- Renderizado de objetos tridimensionales.
- Uso de shaders mediante HLSL.
- Transformaciones de objetos 3D y rotación de un cubo.

## Requisitos

- Windows.
- Visual Studio con herramientas de desarrollo de C++.
- Windows SDK compatible con DirectX 11.
- GPU compatible con DirectX 11.

## Instalación y ejecución

1. Clonar el repositorio:

   ```bash
   git clone <URL_DEL_REPOSITORIO>
   ```

2. Abrir la solución del proyecto en Visual Studio.

3. Seleccionar la configuración de compilación correspondiente, por ejemplo, `Debug` y `x64`.

4. Compilar la solución.

5. Ejecutar el proyecto `Sandbox`.

**Nota:** Los archivos de shaders deben estar disponibles en la ruta de ejecución que utiliza el motor.

## Estado del proyecto

Proyecto académico en desarrollo.

Actualmente se trabaja en la configuración de la ventana, la inicialización de DirectX 11 y el renderizado de objetos 3D.

Se agregarán nuevas funcionalidades conforme avance el desarrollo.

## Autor

Desarrollado como proyecto de aprendizaje de gráficos computacionales 3D.
