/**
 * @file Window.cpp
 * @brief Implementación de la clase Window para la creación y gestión de la ventana Win32.
 */

#include "Window.h"

/**
 * @brief Destruye la instancia de la ventana invocando internamente a Destroy().
 */
Window::~Window(){
	Destroy();
}

/**
 * @brief Registra la clase de ventana y crea la ventana nativa de Win32 con las dimensiones especificadas.
 * 
 * @param Instance Instancia actual de la aplicación Win32.
 * @param title Título que figurará en la barra superior de la ventana.
 * @param clientWidth Ancho deseado para el área cliente en píxeles.
 * @param clientHeight Alto deseado para el área cliente en píxeles.
 * @return true Si el registro y la creación de la ventana fueron exitosos.
 * @return false Si alguno de los parámetros es inválido o falla la API de Windows.
 */
bool Window::Create(
	HINSTANCE Instance,
	const wchar_t* title,
	UINT clientWidth,
	UINT clientHeight) noexcept{
	if (m_handle ||
		!Instance ||
		!title ||
		clientWidth == 0 ||
		clientHeight == 0)
	{
		return false;
	}

	// Configuración de la clase de ventana
	WNDCLASSEXW windowClass{};

	windowClass.cbSize = sizeof(WNDCLASSEXW);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = Instance;
	windowClass.lpszClassName = ClassName;

	if (!RegisterClassExW(&windowClass))
	{
		return false;
	}

	m_instance = Instance;
	m_classRegistered = true;

	// Ventana fija hasta implementar rendimiento,
	// swap chain y depth buffer
	constexpr DWORD Style =
		WS_OVERLAPPED |
		WS_CAPTION |
		WS_SYSMENU |
		WS_MINIMIZEBOX;

	// Tamaño del área cliente
	RECT windowRect{
		0,
		0,
		static_cast<LONG>(clientWidth),
		static_cast<LONG>(clientHeight)
	};

	if (!AdjustWindowRectEx(
		&windowRect,
		Style,
		FALSE,
		0))
	{
		Destroy();
		return false;
	}

	const int windowWidth =
		windowRect.right - windowRect.left;

	const int windowHeight =
		windowRect.bottom - windowRect.top;

	// Crear la ventana
	m_handle = CreateWindowExW(
		0,
		ClassName,
		title,
		Style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowWidth,
		windowHeight,
		nullptr,
		nullptr,
		Instance,
		this
	);

	if (!m_handle)
	{
		Destroy();
		return false;
	}

	return true;
}

/**
 * @brief Muestra y actualiza el estado visual de la ventana en pantalla.
 * 
 * @param showCommand Bandera de visibilidad (ej. SW_SHOW, SW_MINIMIZE).
 */
void Window::Show(int showCommand) noexcept{
	if (m_handle)
	{
		ShowWindow(m_handle, showCommand);
		UpdateWindow(m_handle);
	}
}

/**
 * @brief Extrae y despacha los mensajes pendientes en la cola del sistema de Windows.
 * 
 * @return true Si la ejecución de la aplicación debe continuar.
 * @return false Si se interceptó el mensaje WM_QUIT para cerrar la aplicación.
 */
bool Window::ProcessMessages() noexcept{
	MSG message{};

	while (PeekMessageW(
		&message,
		nullptr,
		0,
		0,
		PM_REMOVE))
	{
		if (message.message == WM_QUIT)
		{
			return false;
		}

		TranslateMessage(&message);
		DispatchMessageW(&message);
	}

	return true;
}

/**
 * @brief Comprueba si la ventana se encuentra actualmente minimizada.
 * 
 * @return true Si la ventana existe y está minimizada (iconizada).
 * @return false En caso contrario.
 */
bool Window::IsMinimized() const noexcept{
	return m_handle && IsIconic(m_handle);
}

/**
 * @brief Libera el identificador de la ventana y desregistra la clase de ventana de Windows.
 */
void Window::Destroy() noexcept{
	if (m_handle)
	{
		DestroyWindow(m_handle);
		m_handle = nullptr;
	}

	if (m_classRegistered && m_instance)
	{
		UnregisterClassW(
			ClassName,
			m_instance
		);

		m_classRegistered = false;
		m_instance = nullptr;
	}
}

/**
 * @brief Procedimiento de ventana encargado de gestionar los mensajes enviados por el sistema operativo.
 * 
 * @param handle Identificador de la ventana receptora del mensaje.
 * @param message Código identificador del evento o mensaje.
 * @param wParam Primer parámetro contextual del mensaje.
 * @param lParam Segundo parámetro contextual del mensaje (puede contener el puntero `this` en WM_NCCREATE).
 * @return LRESULT Resultado del procesamiento del mensaje.
 */
LRESULT CALLBACK Window::WindowProc(
	HWND handle,
	UINT message,
	WPARAM wParam,
	LPARAM lParam){
	Window* window = nullptr;

	if (message == WM_NCCREATE)
	{
		const auto createStruct =
			reinterpret_cast<CREATESTRUCTW*>(lParam);

		window =
			static_cast<Window*>(
				createStruct->lpCreateParams
				);

		SetWindowLongPtrW(
			handle,
			GWLP_USERDATA,
			reinterpret_cast<LONG_PTR>(window)
		);

		window->m_handle = handle;
	}
	else
	{
		window =
			reinterpret_cast<Window*>(
				GetWindowLongPtrW(
					handle,
					GWLP_USERDATA
				)
				);
	}

	if (window)
	{
		switch (message)
		{
		case WM_CLOSE:
			DestroyWindow(handle);
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
	}

	return DefWindowProcW(
		handle,
		message,
		wParam,
		lParam
	);
}