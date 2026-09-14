#pragma once

#include <cstdint>

class Engine
{
public:
  Engine() noexcept;
  ~Engine() noexcept;

  bool Initialize(void* nativeWindow, std::uint32_t width, std::uint32_t height) noexcept;
  void Render() noexcept;
  void Shutdown() noexcept;

private:
  struct Implementation;
  Implementation* m_implementation;
};