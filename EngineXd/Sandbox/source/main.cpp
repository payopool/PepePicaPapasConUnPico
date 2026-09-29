
/**
 * @file main.cpp
 * @brief Punto de entrada de la aplicación y ciclo principal del motor gráfico 3D.
 *
 * Este archivo inicializa la ventana, prepara el motor gráfico y ejecuta
 * el ciclo principal de la aplicación hasta que se recibe WM_QUIT.
 *
 * @author Payo
 */

#include <Windows.h>
#include <cstdint>

#include "Window.h"
#include <Engine/Engine.h>

 /**
  * @brief Punto de entrada de una aplicación Win32 con soporte Unicode.
  *
  * Crea la ventana principal, inicializa el motor gráfico y ejecuta
  * el ciclo de actualización y renderizado.
  *
  * @param hInstance Identificador de la instancia actual de la aplicación.
  * @param hPrevInstance Parámetro heredado, sin uso en Win32 moderno.
  * @param lpCmdLine Argumentos de la línea de comandos.
  * @param nCmdShow Indica cómo debe mostrarse la ventana inicialmente.
  *
  * @return Código de salida de la aplicación.
  */
int APIENTRY wWinMain(
  _In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPWSTR lpCmdLine,
  _In_ int nCmdShow
)
{
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  constexpr UINT windowWidth = 1280;
  constexpr UINT windowHeight = 720;

  Window window;

  if (!window.Create(
    hInstance,
    L"OOH-Yea",
    windowWidth,
    windowHeight))
  {
    return -1;
  }

  // Mostrar la ventana utilizando la clase Window
  window.Show(nCmdShow);

  Engine graphicsEngine;

  if (!graphicsEngine.Initialize(
    window.GetNativeWindow(),
    windowWidth,
    windowHeight))
  {
    graphicsEngine.Shutdown();
    return -1;
  }

  while
    (window.ProcessMessages())
  {
    // Evitar renderizar mientras la ventana está minimizada
    if (window.IsMinimized())
    {
      continue;
    }

    // Renderizar el fotograma actual
    graphicsEngine.Render();
  }

  graphicsEngine.Shutdown();

  return 0;
}