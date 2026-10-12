#pragma once

#include "API.h"

#include <cstdint>
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include <string>
#include <new>
#include <sstream>

template<typename T>
void SafeRelease(T*& object)
{
  if (object)
  {
    object->Release();
    object = nullptr;
  }
}

extern "C"
{
  ENGINE_API bool
    Engine_Initialize(HWND hwnd, int width, int height) noexcept;

  ENGINE_API void
    Engine_Update() noexcept;

  ENGINE_API void
    Engine_Render() noexcept;

  ENGINE_API void
    Engine_Shutdown() noexcept;
}