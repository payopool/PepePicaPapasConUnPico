/**
 * @file main.cpp
 * @brief Punto de entrada principal de la aplicación Win32 y bucle del juego (Game Loop) para el motor gráfico.
 */

#include <windows.h>
#include <cstdint> 
#include <Engine/Engine.h>

 /**
  * @brief Procedimiento de ventana principal para el manejo de mensajes del sistema operativo.
  *
  * @param hwnd Identificador de la ventana receptora del mensaje.
  * @param uMsg Código identificador del mensaje de Windows.
  * @param wParam Primer parámetro contextual del mensaje.
  * @param lParam Segundo parámetro contextual del mensaje.
  * @return LRESULT Resultado del procesamiento del mensaje.
  */
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/**
 * @brief Función de entrada principal para aplicaciones basadas en ventanas Win32 (Unicode).
 *
 * @param hInstance Identificador de la instancia actual de la aplicación.
 * @param hPrevInstance Sin uso en sistemas Win32 modernos (siempre NULL).
 * @param lpCmdLine Argumentos de la línea de comandos pasados a la aplicación.
 * @param nCmdShow Bandera que especifica cómo debe mostrarse la ventana inicialmente.
 * @return int Código de salida devuelto al sistema operativo al finalizar la aplicación.
 */
int APIENTRY wWinMain(
  _In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPWSTR lpCmdLine,
  _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // 1. Registrar la clase de la ventana con fondo negro
  const wchar_t CLASS_NAME[] = L"EngineXdWindowClass";

  WNDCLASSEX wc = {};
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hInstance;
  wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // Fondo negro
  wc.lpszClassName = CLASS_NAME;

  RegisterClassEx(&wc);

  // 2. Crear la ventana de resolución 1280x720
  const uint32_t width = 1280;
  const uint32_t height = 720;

  HWND hwnd = CreateWindowEx(
    0,
    CLASS_NAME,
    L"EngineXd - OOOH yea ._.",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    width, height,
    nullptr,
    nullptr,
    hInstance,
    nullptr
  );

  if (!hwnd) {
    return 0;
  }

  ShowWindow(hwnd, nCmdShow);

  // 3. Instanciar e inicializar el motor gráfico
  Engine graphicsEngine;
  if (!graphicsEngine.Initialize(hwnd, width, height)) {
    return 0; // Si falla la inicialización de DirectX, se sale de la aplicación de forma segura
  }

  // 4. Bucle principal de ejecución (Game Loop)
  MSG msg = {};
  bool isRunning = true;

  while (isRunning) {
    // Procesar eventos y mensajes pendientes de Windows
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT) {
        isRunning = false;
      }
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    // Renderizar el fotograma actual utilizando el motor gráfico
    graphicsEngine.Render();
  }

  // 5. Apagar y limpiar los recursos del motor antes de cerrar
  graphicsEngine.Shutdown();

  return static_cast<int>(msg.wParam);
}