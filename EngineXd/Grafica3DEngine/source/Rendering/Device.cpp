
#include "Rendering/Device.h"

void
Device::init()
{
  constexpr D3D_FEATURE_LEVEL featureLevels[]
  {
      D3D_FEATURE_LEVEL_11_0
  };

  HRESULT result =
    D3D11CreateDevice(
      nullptr,
      D3D_DRIVER_TYPE_HARDWARE,
      nullptr,
      0,
      featureLevels,
      ARRAYSIZE(featureLevels),
      D3D11_SDK_VERSION,
      &m_device,
      &this->featureLevels,
      nullptr
    );

  if (FAILED(result))
  {
    m_device = nullptr;
  }
}

Device::~Device()
{
  destroy();
}

void
Device::destroy()
{
  SafeRelease(m_device);
}

HRESULT
Device::CreateRenderTargetView(
  ID3D11Resource* pResource,
  const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
  ID3D11RenderTargetView** ppRTView)
{
  if (ppRTView == nullptr)
  {
    return E_POINTER;
  }

  *ppRTView = nullptr;

  if (m_device == nullptr || pResource == nullptr)
  {
    return E_INVALIDARG;
  }

  return m_device->CreateRenderTargetView(
    pResource,
    pDesc,
    ppRTView
  );
}

HRESULT
Device::CreateTexture2D(
  const D3D11_TEXTURE2D_DESC* pDesc,
  const D3D11_SUBRESOURCE_DATA* pInitialData,
  ID3D11Texture2D** ppTexture2D)
{
  if (ppTexture2D == nullptr)
  {
    return E_POINTER;
  }

  *ppTexture2D = nullptr;

  if (m_device == nullptr || pDesc == nullptr)
  {
    return E_INVALIDARG;
  }

  return m_device->CreateTexture2D(
    pDesc,
    pInitialData,
    ppTexture2D
  );
}

HRESULT
Device::CreateDepthStencilView(
  ID3D11Resource* pResource,
  const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
  ID3D11DepthStencilView** ppDepthStencilView)
{
  if (ppDepthStencilView == nullptr)
  {
    return E_POINTER;
  }

  *ppDepthStencilView = nullptr;

  if (m_device == nullptr || pResource == nullptr)
  {
    return E_INVALIDARG;
  }

  return m_device->CreateDepthStencilView(
    pResource,
    pDesc,
    ppDepthStencilView
  );
}

HRESULT
Device::CreateVertexShader(
  const void* pShaderBytecode,
  SIZE_T BytecodeLength,
  ID3D11ClassLinkage* pClassLinkage,
  ID3D11VertexShader** ppVertexShader)
{
  if (ppVertexShader == nullptr)
  {
    return E_POINTER;
  }

  *ppVertexShader = nullptr;

  if (m_device == nullptr ||
    pShaderBytecode == nullptr ||
    BytecodeLength == 0)
  {
    return E_INVALIDARG;
  }

  return m_device->CreateVertexShader(
    pShaderBytecode,
    BytecodeLength,
    pClassLinkage,
    ppVertexShader
  );
}

HRESULT
Device::CreatePixelShader(
  const void* pShaderBytecode,
  SIZE_T BytecodeLength,
  ID3D11ClassLinkage* pClassLinkage,
  ID3D11PixelShader** ppPixelShader)
{
  if (ppPixelShader == nullptr)
  {
    return E_POINTER;
  }

  *ppPixelShader = nullptr;

  if (m_device == nullptr ||
    pShaderBytecode == nullptr ||
    BytecodeLength == 0)
  {
    return E_INVALIDARG;
  }

  return m_device->CreatePixelShader(
    pShaderBytecode,
    BytecodeLength,
    pClassLinkage,
    ppPixelShader
  );
}

HRESULT
Device::CreateInputLayout(
  const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
  UINT NumElements,
  const void* pShaderBytecodeWithInputSignature,
  SIZE_T BytecodeLength,
  ID3D11InputLayout** ppInputLayout)
{
  if (ppInputLayout == nullptr)
  {
    return E_POINTER;
  }

  *ppInputLayout = nullptr;

  if (m_device == nullptr ||
    pInputElementDescs == nullptr ||
    pShaderBytecodeWithInputSignature == nullptr)
  {
    return E_INVALIDARG;
  }

  return m_device->CreateInputLayout(
    pInputElementDescs,
    NumElements,
    pShaderBytecodeWithInputSignature,
    BytecodeLength,
    ppInputLayout
  );
}

HRESULT
Device::CreateBuffer(
  const D3D11_BUFFER_DESC* pDesc,
  const D3D11_SUBRESOURCE_DATA* pInitialData,
  ID3D11Buffer** ppBuffer)
{
  if (ppBuffer == nullptr)
  {
    return E_POINTER;
  }

  *ppBuffer = nullptr;

  if (m_device == nullptr || pDesc == nullptr)
  {
    return E_INVALIDARG;
  }

  return m_device->CreateBuffer(
    pDesc,
    pInitialData,
    ppBuffer
  );
}

HRESULT
Device::CreateRasterizerState(
  const D3D11_RASTERIZER_DESC* pRasterizerDesc,
  ID3D11RasterizerState** ppRasterizerState)
{
  if (ppRasterizerState == nullptr)
  {
    return E_POINTER;
  }

  *ppRasterizerState = nullptr;

  if (m_device == nullptr || pRasterizerDesc == nullptr)
  {
    return E_INVALIDARG;
  }

  return m_device->CreateRasterizerState(
    pRasterizerDesc,
    ppRasterizerState
  );
}
