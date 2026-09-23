#pragma once

#include <cstdint>

/**
 * @file Engine.h
 * @brief Declaración de la clase principal Engine para la gestión del motor gráfico.
 */

 /**
  * @class Engine
  * @brief Clase central que controla el ciclo de vida, la inicialización y el renderizado del motor 3D.
  *
  * Utiliza el patrón PImpl (Pointer to Implementation) para ocultar los detalles de bajo nivel
  * de la API gráfica y mantener la cabecera limpia de dependencias externas.
  */
class Engine {
public:
  /**
   * @brief Construye una nueva instancia del motor gráfico.
   */
  Engine() noexcept;

  /**
   * @brief Destruye la instancia del motor y asegura la liberación de todos los recursos.
   */
  ~Engine() noexcept;

  /**
   * @brief Inicializa los dispositivos, contextos y recursos necesarios para el funcionamiento del motor.
   *
   * @param nativeWindow Puntero a la ventana nativa de la plataforma (ej. HWND en Windows).
   * @param width Ancho del área de cliente en píxeles.
   * @param height Alto del área de cliente en píxeles.
   * @return true Si la inicialización se completó con éxito.
   * @return false Si ocurrió algún error durante el proceso.
   */
  bool Initialize(void* nativeWindow, std::uint32_t width, std::uint32_t height) noexcept;

  /**
   * @brief Ejecuta el ciclo de renderizado por fotograma de la escena.
   */
  void Render() noexcept;

  /**
   * @brief Apaga el motor y libera de forma segura todos los recursos gráficos y de memoria asociados.
   */
  void Shutdown() noexcept;

private:
  struct Implementation;              ///< Declaración anticipada de la estructura interna (PImpl).
  Implementation* m_implementation;   ///< Puntero a los detalles de implementación privados del motor.
};