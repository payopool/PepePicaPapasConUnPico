#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>

/**
 * @file Window.h
 * @brief Declaración de la clase Window para la gestión de ventanas nativas de Win32.
 */

 /**
	* @class Window
	* @brief Clase final que encapsula la creación, gestión de mensajes y ciclo de vida de una ventana de Windows.
	*/
class Window final {
public:
	/**
	 * @brief Construye una nueva instancia de la ventana sin inicializarla.
	 */
	Window() = default;

	/**
	 * @brief Destruye la ventana y libera los recursos asociados.
	 */
	~Window();

	// Se elimina el constructor de copia para prevenir duplicados del recurso de ventana.
	Window(const Window&) = delete;

	/**
	 * @brief Crea y registra la clase de ventana de Windows, inicializando el identificador nativo.
	 *
	 * @param Instance Instancia de la aplicación Win32.
	 * @param title Título que se mostrará en la barra de la ventana.
	 * @param clientWidth Ancho del área de cliente en píxeles.
	 * @param clientHeight Alto del área de cliente en píxeles.
	 * @return true Si la ventana fue creada exitosamente.
	 * @return false Si ocurrió algún error durante el registro o creación.
	 */
	bool Create(
		HINSTANCE Instance,
		const wchar_t* title,
		UINT clientWidth,
		UINT clientHeight
	) noexcept;

	/**
	 * @brief Muestra la ventana en la pantalla según el comando de visualización especificado.
	 *
	 * @param showCommand Parámetro de visibilidad de Win32 (ej. SW_SHOW, SW_HIDE).
	 */
	void Show(int showCommand) noexcept;

	/**
	 * @brief Procesa la cola de mensajes del sistema operativo (bucle de mensajes).
	 *
	 * @return true Si la aplicación debe continuar su ejecución.
	 * @return false Cuando recibe el mensaje WM_QUIT para finalizar.
	 */
	bool ProcessMessages() noexcept;

	/**
	 * @brief Obtiene el identificador nativo de la ventana de Windows.
	 *
	 * @return HWND Identificador (handle) de la ventana.
	 */
	HWND GetNativeWindow() const noexcept {
		return m_handle;
	}

	/**
	 * @brief Comprueba si la ventana se encuentra minimizada.
	 *
	 * @return true Si la ventana está minimizada.
	 * @return false En caso contrario.
	 */
	bool IsMinimized() const noexcept;

private:

	/**
	 * @brief Destruye la ventana y desregistra la clase de Windows si corresponde.
	 */
	void Destroy() noexcept;

	/**
	 * @brief Función de devolución de llamada estática para procesar los mensajes del sistema (Window Procedure).
	 *
	 * @param handle Identificador de la ventana que recibe el mensaje.
	 * @param message Código del mensaje de Windows.
	 * @param wParam Parámetro adicional del mensaje (dependiente del mensaje).
	 * @param lParam Parámetro adicional del mensaje (dependiente del mensaje).
	 * @return LRESULT Resultado del procesamiento del mensaje.
	 */
	static LRESULT CALLBACK
		WindowProc(
			HWND handle,
			UINT message,
			WPARAM wParam,
			LPARAM lParam
		);

	/// Nombre único de la clase de ventana de Windows.
	static constexpr const wchar_t* ClassName =
		L"Grafica3DEngineWindowClass";

	HINSTANCE m_instance = nullptr; ///< Instancia de la aplicación asociada a la ventana.
	HWND m_handle = nullptr;         ///< Identificador (handle) de la ventana nativa.
	bool m_classRegistered = false;  ///< Indicador de si la clase de ventana ya fue registrada en el sistema.
};