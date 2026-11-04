/**
 * @file Prerequisitos.h
 * @brief Archivo de inclusión centralizado, definición de estructuras base, macros de depuración y tipos enumerados del motor.
 */

#pragma once

#include "API.h"
#include <cstdint>
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <sstream>
#include <cstddef>
#include <chrono>
#include <new>

 // ============================================================
 // MACROS DE DEPURACIÓN Y GESTIÓN DE RECURSOS
 // ============================================================

 /**
  * @brief Macro para liberar de forma segura una interfaz de DirectX comprobando que no sea nula.
  */
#define SAFE_RELEASE(x) \
{                       \
  if (x != nullptr)     \
  {                     \
    x->Release();       \
    x = nullptr;        \
  }                     \
}

  /**
   * @brief Macro para registrar mensajes de estado en la salida de depuración de Windows.
   */
#define MESSAGE(classObj, method, state)                      \
{                                                             \
   std::wostringstream os_;                                   \
   os_ << classObj << "::" << method << " : "                 \
       << "[CREATION OF RESOURCE : " << state << "] \n";      \
   OutputDebugStringW(os_.str().c_str());                     \
}

   /**
    * @brief Macro para registrar mensajes de error detallados con manejo de excepciones en la salida de depuración.
    */
#define ERROR(classObj, method, errorMSG)                     \
{                                                             \
    try                                                       \
    {                                                         \
        std::wostringstream os_;                              \
        os_ << L"ERROR : " << classObj << L"::" << method     \
            << L" : " << errorMSG << L"\n";                   \
        OutputDebugStringW(os_.str().c_str());                \
    }                                                         \
    catch (...)                                               \
    {                                                         \
        OutputDebugStringW(L"Failed to log error message.\n");\
    }                                                         \
}

    // ============================================================
    // ESTRUCTURAS BÁSICAS DE GEOMETRÍA
    // ============================================================

    /**
     * @struct Vertex
     * @brief Define la estructura básica de un vértice con posición espacial y color asociadas.
     */
struct Vertex
{
  float position[3]; ///< Coordenadas espaciales (X, Y, Z).
  float color[4];    ///< Componentes de color RGBA (R, G, B, A).
};

// ============================================================
// ENUMERACIONES
// ============================================================

/**
 * @enum ExtensionType
 * @brief Define los tipos de extensiones e formatos de imagen soportados para la carga de texturas.
 */
enum ExtensionType
{
  DDS = 0, ///< Formato DirectDraw Surface.
  PNG = 1, ///< Formato Portable Network Graphics.
  JPG = 2, ///< Formato Joint Photographic Experts Group.
  BMP = 3, ///< Formato Bitmap.
  TGA = 4  ///< Formato Truevision TGA.
};

// ============================================================
// FUNCIONES UTILITARIAS TEMPLADAS
// ============================================================

/**
 * @brief Función plantilla para liberar de manera segura un objeto de DirectX mediante referencia a puntero.
 *
 * @tparam T Tipo de la interfaz de DirectX.
 * @param object Referencia al puntero del objeto que se va a liberar.
 */
template<typename T>
void
SafeRelease(
  T*& object
) noexcept
{
  if (object != nullptr)
  {
    object->Release();
    object = nullptr;
  }
}

// ============================================================
// INTERFAZ C EXPORTADA (DLL)
// ============================================================

extern "C"
{
  /**
   * @brief Inicializa el motor gráfico a través de la interfaz externa en C.
   *
   * @param hwnd Identificador de la ventana nativa.
   * @param width Ancho de la ventana de renderizado.
   * @param height Alto de la ventana de renderizado.
   * @return true si la inicialización fue exitosa, de lo contrario false.
   */
  ENGINE_API bool
    Engine_Initialize(
      HWND hwnd,
      int width,
      int height
    ) noexcept;

  /**
   * @brief Actualiza la lógica interna del motor en cada fotograma.
   */
  ENGINE_API void
    Engine_Update() noexcept;

  /**
   * @brief Ejecuta el proceso de renderizado de la escena gráfica.
   */
  ENGINE_API void
    Engine_Render() noexcept;

  /**
   * @brief Apaga el motor y libera todos los recursos globales asignados.
   */
  ENGINE_API void
    Engine_Shutdown() noexcept;
}