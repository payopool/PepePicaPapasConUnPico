#pragma once
#include "Engine/Prerequisitos.h"

class
  Device {
public:
  Device() = default;
  ~Device();

  void
    init();

  void
    destroy();

  ID3D11Device*
    getDevice() const noexcept
  {
    return m_device;
  }

  ID3D11Device**
    getDevice() noexcept
  {
    return &m_device;
  }

  HRESULT
    CreateRenderTargetView(ID3D11Resource* pResource,
      const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
      ID3D11RenderTargetView** ppRTView);

  HRESULT
    CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc,
      const D3D11_SUBRESOURCE_DATA* pInitialData,
      ID3D11Texture2D** ppTexture2D);

  HRESULT
    CreateDepthStencilView(ID3D11Resource* pResource,
      const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
      ID3D11DepthStencilView** ppDepthStencilView);

  HRESULT
    CreateVertexShader(const void* pShaderBytecode,
      SIZE_T BytecodeLength,
      ID3D11ClassLinkage* pClassLinkage,
      ID3D11VertexShader** ppVertexShader);

  HRESULT
    CreatePixelShader(const void* pShaderBytecode,
      SIZE_T BytecodeLength,
      ID3D11ClassLinkage* pClassLinkage,
      ID3D11PixelShader** ppPixelShader);

  HRESULT
    CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
      UINT NumElements,
      const void* pShaderBytecodeWithInputSignature,
      SIZE_T BytecodeLength,
      ID3D11InputLayout** ppInputLayout);

  HRESULT
    CreateBuffer(const D3D11_BUFFER_DESC* pDesc,
      const D3D11_SUBRESOURCE_DATA* pInitialData,
      ID3D11Buffer** ppBuffer);

  HRESULT
    CreateRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerDesc,
      ID3D11RasterizerState** ppRasterizerState);

private:
  ID3D11Device* m_device = nullptr;
  D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_11_0;
};