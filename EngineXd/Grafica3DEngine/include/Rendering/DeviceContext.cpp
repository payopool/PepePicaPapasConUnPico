/**
 * @file DeviceContext.cpp
 * @brief Implementación de la clase DeviceContext para la gestión y ejecución de comandos en el contexto de DirectX 11.
 */

#include "Rendering/DeviceContext.h"

 /**
  * @brief Destructor de la clase DeviceContext que libera los recursos asociados.
  */
DeviceContext::~DeviceContext()
{
  destroy();
}

/**
 * @brief Inicializa el contexto del dispositivo estableciendo su puntero a nulo.
 */
void
DeviceContext::init()
{
  m_deviceContext = nullptr;
}

/**
 * @brief Libera de forma segura el contexto del dispositivo DirectX 11.
 */
void
DeviceContext::destroy()
{
  SafeRelease(m_deviceContext);
}

/**
 * @brief Devuelve una referencia al contexto del dispositivo DirectX 11. Lanza un error si no está inicializado.
 *
 * @return ID3D11DeviceContext& Referencia al contexto de DirectX 11.
 */
ID3D11DeviceContext&
DeviceContext::requireContext() const noexcept
{
  return *m_deviceContext;
}

/**
 * @brief Limpia la vista del objetivo de renderizado con el color especificado.
 *
 * @param renderTargetView Puntero a la vista del objetivo de renderizado a limpiar.
 * @param colorRGBA Arreglo de cuatro flotantes que representan el color RGBA de limpieza.
 */
void
DeviceContext::ClearRenderTargetView(
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

/**
 * @brief Limpia la vista de profundidad-plantilla con los valores de profundidad y plantilla especificados.
 *
 * @param depthStencilView Puntero a la vista de profundidad-plantilla a limpiar.
 * @param clearFlags Banderas que indican qué partes limpiar (D3D11_CLEAR_DEPTH, D3D11_CLEAR_STENCIL).
 * @param depth Valor de profundidad para limpiar.
 * @param stencil Valor de plantilla para limpiar.
 */
void
DeviceContext::ClearDepthStencilView(
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

/**
 * @brief Establece los objetivos de renderizado para la etapa de fusión de salida (Output-Merger).
 *
 * @param numViews Número de vistas de objetivos de renderizado a establecer.
 * @param renderTargetViews Puntero a un arreglo de punteros a vistas de objetivos de renderizado.
 * @param depthStencilView Puntero a la vista de profundidad-plantilla a establecer.
 */
void
DeviceContext::OMSetRenderTargets(
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

/**
 * @brief Establece las ventanas de visualización (viewports) para la etapa del rasterizador.
 *
 * @param numViewports Número de viewports a establecer.
 * @param viewports Puntero a un arreglo de estructuras D3D11_VIEWPORT que describen las ventanas de visualización.
 */
void
DeviceContext::RSSetViewports(
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

/**
 * @brief Establece el estado del rasterizador para la etapa correspondiente.
 *
 * @param rasterizerState Puntero a la interfaz ID3D11RasterizerState que describe el estado del rasterizador.
 */
void
DeviceContext::RSSetState(
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

/**
 * @brief Establece el diseño de entrada para la etapa de ensamblaje de entrada (Input-Assembler).
 *
 * @param inputLayout Puntero a la interfaz ID3D11InputLayout que describe el diseño de entrada.
 */
void
DeviceContext::IASetInputLayout(
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

/**
 * @brief Establece la topología de primitivas para la etapa de ensamblaje de entrada.
 *
 * @param topology Topología de primitivas a establecer.
 */
void
DeviceContext::IASetPrimitiveTopology(
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

/**
 * @brief Establece los búferes de vértices para la etapa de ensamblaje de entrada.
 *
 * @param startSlot Primera ranura de entrada para el enlace.
 * @param numBuffers Número de búferes de vértices a enlazar.
 * @param vertexBuffers Puntero a un arreglo de punteros a búferes de vértices.
 * @param strides Puntero a un arreglo de valores de paso (tamaño de cada vértice).
 * @param offsets Puntero a un arreglo de valores de desplazamiento.
 */
void
DeviceContext::IASetVertexBuffers(
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

/**
 * @brief Establece el búfer de índices para la etapa de ensamblaje de entrada.
 *
 * @param indexBuffer Puntero al búfer de índices a establecer.
 * @param format Formato del búfer de índices.
 * @param offset Desplazamiento en el búfer de índices.
 */
void
DeviceContext::IASetIndexBuffer(
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

/**
 * @brief Establece el sombreador de vértices para la etapa de vertex-shader.
 *
 * @param vertexShader Puntero a la interfaz ID3D11VertexShader.
 * @param classInstances Puntero a un arreglo de instancias de clases.
 * @param numClassInstances Número de instancias de clases.
 */
void
DeviceContext::VSSetShader(
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

/**
 * @brief Establece el sombreador de píxeles para la etapa de pixel-shader.
 *
 * @param pixelShader Puntero a la interfaz ID3D11PixelShader.
 * @param classInstances Puntero a un arreglo de instancias de clases.
 * @param numClassInstances Número de instancias de clases.
 */
void
DeviceContext::PSSetShader(
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

/**
 * @brief Establece los búferes constantes para la etapa de vertex-shader.
 *
 * @param startSlot Primera ranura de búfer constante para el enlace.
 * @param numBuffers Número de búferes constantes a enlazar.
 * @param constantBuffers Puntero a un arreglo de punteros a búferes constantes.
 */
void
DeviceContext::VSSetConstantBuffers(
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

/**
 * @brief Actualiza un subrecurso de la GPU.
 *
 * @param resource Puntero al recurso de destino.
 * @param dstSubresource Índice del subrecurso de destino.
 * @param box Puntero a la caja delimitadora para el recurso de destino.
 * @param srcData Puntero a los datos de origen.
 * @param srcRowPitch Paso de fila de origen.
 * @param srcDepthPitch Paso de profundidad de origen.
 */
void
DeviceContext::UpdateSubresource(
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

/**
 * @brief Dibuja geometría indexada.
 *
 * @param indexCount Número de índices a dibujar.
 * @param startIndexLocation Ubicación del primer índice a dibujar.
 * @param baseVertexLocation Ubicación del primer vértice a utilizar.
 */
void
DeviceContext::DrawIndexed(
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