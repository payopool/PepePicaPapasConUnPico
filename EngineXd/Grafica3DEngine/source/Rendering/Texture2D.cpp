#include "Rendering/Texture2D.h"
#include "Rendering/Device.h"
#include "Rendering/DeviceContext.h"

Texture2D::~Texture2D() noexcept
{
  destroy();
}

HRESULT Texture2D::init(
  Device& device,
  const std::string& textureName,
  ExtensionType extencionType)
{
  if (!device.getDevice())
  {
    ERROR("Texture2D", "init", "Device is not initialized.");
    return E_POINTER;
  }

  if (textureName.empty())
  {
    ERROR("Texture2D", "init", "Texture name is empty.");
    return E_INVALIDARG;
  }

  switch (extencionType)
  {
  case DDS:
    ERROR("Texture2D", "init", "DDS loading is not implemented.");
    return E_NOTIMPL;

  case PNG:
    m_textureName = textureName + ".png";
    return InitTextureFromImage(device, m_textureName, *this);

  case JPG:
    m_textureName = textureName + ".jpg";
    return InitTextureFromImage(device, m_textureName, *this);

  default:
    ERROR("Texture2D", "init", "Unsupported extension type.");
    return E_INVALIDARG;
  }
}

void Texture2D::render(
  DeviceContext& deviceContext,
  unsigned int StartSlot,
  unsigned int NumViews)
{
  if (!deviceContext.getDeviceContext())
  {
    ERROR("Texture2D", "render", "Device context is not initialized.");
    return;
  }

  if (!m_TextureFromImg)
  {
    ERROR("Texture2D", "render", "Shader resource view is not initialized.");
    return;
  }

  deviceContext.getDeviceContext()->PSSetShaderResources(
    StartSlot,
    NumViews,
    &m_TextureFromImg
  );
}

void Texture2D::destroy()
{
  if (m_texture != nullptr)
  {
    SAFE_RELEASE(m_texture);
  }

  if (m_TextureFromImg != nullptr)
  {
    SAFE_RELEASE(m_TextureFromImg);
  }

  m_textureName.clear();
}

HRESULT Texture2D::InitTextureFromImage(
  Device& device,
  const std::string& textureName,
  Texture2D& texture)
{
  if (!device.getDevice())
  {
    ERROR(
      "Texture2D",
      "InitTextureFromImage",
      "Device is not initialized."
    );

    return E_POINTER;
  }
}