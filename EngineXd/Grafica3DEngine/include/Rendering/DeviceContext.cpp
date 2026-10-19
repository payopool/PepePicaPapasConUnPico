#include "Rendering/DeviceContext.h"

DeviceContext::~DeviceContext()
{
  destroy();
}

void DeviceContext::init()
{
  m_deviceContext = nullptr;
}

void DeviceContext::destroy()
{
  SafeRelease(m_deviceContext);
}

ID3D11DeviceContext& DeviceContext::requireContext() const noexcept
{
  return *m_deviceContext;
}

void DeviceContext::ClearRenderTargetView(
  ID3D11RenderTargetView* renderTargetView,
  const float colorRGBA[4]
) noexcept
{
  if (!m_deviceContext)
  {
    return;
  }

  m_deviceContext->ClearRenderTargetView(
    renderTargetView,
    colorRGBA
  );
}

void DeviceContext::ClearDepthStencilView(
  ID3D11DepthStencilView* depthStencilView,
  unsigned int clearFlags,
  float depth,
  unsigned int stencil
) noexcept
{
  if (!m_deviceContext)
  {
    return;
  }

  m_deviceContext->ClearDepthStencilView(
    depthStencilView,
    clearFlags,
    depth,
    stencil
  );
}

void DeviceContext::OMSetRenderTargets(
  unsigned int numViews,
  ID3D11RenderTargetView* const* renderTargetViews,
  ID3D11DepthStencilView* depthStencilView
) noexcept
{
  if (!m_deviceContext)
  {
    return;
  }

  m_deviceContext->OMSetRenderTargets(
    numViews,
    renderTargetViews,
    depthStencilView
  );
}

void DeviceContext::RSSetViewports(
  unsigned int numViewports,
  const D3D11_VIEWPORT* viewports
) noexcept
{
  if (!m_deviceContext)
  {
    return;
  }

  m_deviceContext->RSSetViewports(
    numViewports,
    viewports
  );
}

void DeviceContext::RSSetState(
  ID3D11RasterizerState* rasterizerState
) noexcept
{
  if (!m_deviceContext)
  {
    return;
  }

  m_deviceContext->RSSetState(
    rasterizerState
  );
}

void DeviceContext::IASetInputLayout(
  ID3D11InputLayout* inputLayout
) noexcept
{
  if (!m_deviceContext)
  {
    return;
  }

  m_deviceContext->IASetInputLayout(
    inputLayout
  );
}

void DeviceContext::IASetPrimitiveTopology(
  D3D11_PRIMITIVE_TOPOLOGY topology
) noexcept
{
  if (!m_deviceContext)
  {
    return;
  }

  m_deviceContext->IASetPrimitiveTopology(
    topology
  );
}

void DeviceContext::IASetVertexBuffers(
  unsigned int startSlot,
  unsigned int numBuffers,
  ID3D11Buffer* const* vertexBuffers,
  const unsigned int* strides,
  const unsigned int* offsets
) noexcept
{
  if (!m_deviceContext)
  {
    return;
  }

  m_deviceContext->IASetVertexBuffers(
    startSlot,
    numBuffers,
    vertexBuffers,
    strides,
    offsets
  );
}

void DeviceContext::IASetIndexBuffer(
  ID3D11Buffer* indexBuffer,
  DXGI_FORMAT format,
  unsigned int offset
) noexcept
{
  if (!m_deviceContext)
  {
    return;
  }

  m_deviceContext->IASetIndexBuffer(
    indexBuffer,
    format,
    offset
  );
}

void DeviceContext::VSSetShader(
  ID3D11VertexShader* vertexShader,
  ID3D11ClassInstance* const* classInstances,
  unsigned int numClassInstances
) noexcept
{
  if (!m_deviceContext)
  {
    return;
  }

  m_deviceContext->VSSetShader(
    vertexShader,
    classInstances,
    numClassInstances
  );
}

void DeviceContext::PSSetShader(
  ID3D11PixelShader* pixelShader,
  ID3D11ClassInstance* const* classInstances,
  unsigned int numClassInstances
) noexcept
{
  if (!m_deviceContext)
  {
    return;
  }

  m_deviceContext->PSSetShader(
    pixelShader,
    classInstances,
    numClassInstances
  );
}

void DeviceContext::VSSetConstantBuffers(
  unsigned int startSlot,
  unsigned int numBuffers,
  ID3D11Buffer* const* constantBuffers
) noexcept
{
  if (!m_deviceContext)
  {
    return;
  }

  m_deviceContext->VSSetConstantBuffers(
    startSlot,
    numBuffers,
    constantBuffers
  );
}

void DeviceContext::UpdateSubresource(
  ID3D11Resource* resource,
  unsigned int dstSubresource,
  const D3D11_BOX* box,
  const void* srcData,
  unsigned int srcRowPitch,
  unsigned int srcDepthPitch
) noexcept
{
  if (!m_deviceContext)
  {
    return;
  }

  m_deviceContext->UpdateSubresource(
    resource,
    dstSubresource,
    box,
    srcData,
    srcRowPitch,
    srcDepthPitch
  );
}

void DeviceContext::DrawIndexed(
  unsigned int indexCount,
  unsigned int startIndexLocation,
  int baseVertexLocation
) noexcept
{
  if (!m_deviceContext)
  {
    return;
  }

  m_deviceContext->DrawIndexed(
    indexCount,
    startIndexLocation,
    baseVertexLocation
  );
}