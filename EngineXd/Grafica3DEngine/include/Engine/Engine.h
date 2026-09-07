#pragma once

#include "API.h"
#include<Windows.h>

extern "C" {
	ENGINE_API bool Engine_Initialize(HWND hwnd, int width, int height) noexcept;
	ENGINE_API void Engine_Update() noexcept;
	ENGINE_API void Engine_Render() noexcept;
	ENGINE_API void Engine_Shutdown() noexcept;

}
class ENGINE_API Engine final
{
public:
	Engine() noexcept;
	~Engine() noexcept;


	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	bool Initialize(HWND hwnd, int width, int height) noexcept;

	void Render() noexcept;
	void Shutdown() noexcept;
private :
	struct Implementation;
	Implementation* m_impl = nullptr;
};