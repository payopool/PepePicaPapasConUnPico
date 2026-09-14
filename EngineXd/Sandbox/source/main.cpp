#include <windows.h>
#include <cstdint> // <-- Esto define uint32_t
#include <Engine/Engine.h>

// Declaración del procedimiento de la ventana
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int APIENTRY wWinMain(
  _In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPWSTR lpCmdLine,
  _In_ int nCmdShow)
{
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

  // 2. Crear la ventana de 1280x720
  const uint32_t width = 1280;
  const uint32_t height = 720;

  HWND hwnd = CreateWindowEx(
    0,
    CLASS_NAME,
    L"EngineXd - Triángulo DirectX 11",
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

  // 3. Instanciar e inicializar tu motor gráfico
  Engine graphicsEngine;
  if (!graphicsEngine.Initialize(hwnd, width, height)) {
    return 0; // Si falla la inicialización de DirectX, salimos
  }

  // 4. Bucle principal (Game Loop)
  MSG msg = {};
  bool isRunning = true;

  while (isRunning) {
    // Procesar eventos de Windows
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT) {
        isRunning = false;
      }
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    // Renderizar el frame con tu motor (aquí se dibuja el triángulo)
    graphicsEngine.Render();
  }

  // 5. Apagar y limpiar recursos del motor al cerrar
  graphicsEngine.Shutdown();

  return (int)msg.wParam;
}