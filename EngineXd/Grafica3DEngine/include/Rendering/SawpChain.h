/**
 * @file SwapChain.h
 * @brief Declaración de la clase SwapChain para la gestión de la cadena de intercambio de DirectX 11.
 */

#pragma once

#include "Engine/Prerequisitos.h"

 // Declaraciones adelantadas de clases utilizadas en la clase SwapChain
class Window;
class Device;
class Texture2D;

/**
 * @class SwapChain
 * @brief Encapsula y gestiona la cadena de intercambio (`IDXGISwapChain`) de DirectX 11.
 */
class SwapChain
{
public:
  /**
   * @brief Construye una nueva instancia por defecto de SwapChain.
   */
  SwapChain() = default;

  /**
   * @brief Destruye la instancia y libera los recursos de la cadena de intercambio.
   */
  ~SwapChain();

  // Eliminación de constructores de copia y operadores de asignación
  SwapChain(const SwapChain&) = delete;
  SwapChain& operator=(const SwapChain&) = delete;

  // Eliminación de constructores de movimiento y operadores de asignación por movimiento
  SwapChain(SwapChain&&) = delete;
  SwapChain& operator=(SwapChain&&) = delete;

  /**
   * @brief Inicializa la cadena de intercambio con el dispositivo, la ventana, el ancho y el alto especificados.
   *
   * @param device Referencia al dispositivo de DirectX utilizado para crear la cadena de intercambio.
   * @param window Referencia a la ventana asociada con la cadena de intercambio.
   * @param width Ancho del búfer trasero (back buffer) de la cadena de intercambio.
   * @param height Alto del búfer trasero de la cadena de intercambio.
   * @return HRESULT S_OK si la inicialización fue exitosa, o un código de error en caso contrario.
   */
  HRESULT
    init(
      Device& device,
      Window& window,
      unsigned int width,
      unsigned int height
    );

  /**
   * @brief Destruye la cadena de intercambio y libera los recursos asociados.
   */
  void
    destroy();

  /**
   * @brief Presenta el contenido del búfer trasero en la pantalla.
   *
   * @param syncInterval Número de intervalos verticales a esperar antes de presentar (por defecto es 1).
   * @param flags Opciones de presentación (por defecto es 0).
   * @return HRESULT S_OK si la presentación fue exitosa, o un código de error.
   */
  HRESULT
    Present(
      unsigned int syncInterval = 1,
      unsigned int flags = 0
    );

  /**
   * @brief Obtiene un búfer trasero de la cadena de intercambio.
   *
   * @param bufferIndex Índice del búfer trasero a recuperar (típicamente 0).
   * @param backBuffer Referencia al objeto Texture2D que recibirá el búfer trasero.
   * @return HRESULT S_OK si la operación fue exitosa, o un código de error.
   */
  HRESULT
    GetBuffer(
      unsigned int bufferIndex,
      Texture2D& backBuffer
    );

  /**
   * @brief Redimensiona los búferes de la cadena de intercambio al ancho y alto especificados.
   *
   * @param width Nuevo ancho para el búfer trasero de la cadena de intercambio.
   * @param height Nuevo alto para el búfer trasero de la cadena de intercambio.
   * @return HRESULT S_OK si el redimensionamiento fue exitoso, o un código de error.
   */
  HRESULT
    ResizeBuffers(
      unsigned int width,
      unsigned int height
    );

  /**
   * @brief Obtiene el puntero subyacente a IDXGISwapChain.
   *
   * @return IDXGISwapChain* Puntero a la interfaz IDXGISwapChain.
   */
  IDXGISwapChain*
    getSwapChain()
  {
    return m_swapChain;
  }

private:
  IDXGISwapChain* m_swapChain = nullptr; ///< Puntero a la interfaz IDXGISwapChain subyacente.
};