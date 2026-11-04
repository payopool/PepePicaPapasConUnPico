/**
 * @file DeviceContext.h
 * @brief Declaración de la clase DeviceContext para la gestión del contexto del dispositivo en DirectX 11.
 */

#pragma once

#include "Engine/Prerequisitos.h"

 /**
  * @class DeviceContext
  * @brief Envuelve y gestiona el contexto de DirectX 11 (`ID3D11DeviceContext`) para realizar operaciones de renderizado y configuración de la GPU.
  */
class DeviceContext
{
public:
  /**
   * @brief Construye una nueva instancia por defecto de DeviceContext.
   */
  DeviceContext() = default;

  /**
   * @brief Destruye la instancia y libera los recursos del contexto.
   */
  ~DeviceContext();

  /**
   * @brief Inicializa el contexto del dispositivo.
   */
  void
    init();

  /**
   * @brief Destruye y libera de forma segura los recursos del contexto.
   */
  void
    destroy();

  /**
   * @brief Obtiene el puntero al contexto de DirectX 11 subyacente.
   *
   * @return ID3D11DeviceContext* Puntero al contexto de DirectX 11.
   */
  ID3D11DeviceContext*
    getDeviceContext() const noexcept
  {
    return m_deviceContext;
  }

  /**
   * @brief Obtiene la dirección del puntero al contexto de DirectX 11.
   *
   * @return ID3D11DeviceContext** Dirección del puntero al contexto.
   */
  ID3D11DeviceContext**
    getDeviceContextAddress() noexcept
  {
    return &m_deviceContext;
  }

  /**
   * @brief Devuelve una referencia al contexto del dispositivo DirectX 11. Lanza un error si no está inicializado.
   *
   * @return ID3D11DeviceContext& Referencia al contexto de DirectX 11.
   */
  ID3D11DeviceContext&
    requireContext() const noexcept;

  /**
   * @brief Limpia la vista del objetivo de renderizado con el color especificado.
   *
   * @param pRenderTargetView Puntero a la vista del objetivo de renderizado a limpiar.
   * @param colorRGBA Arreglo de cuatro flotantes que representan el color RGBA de limpieza.
   */
  void
    ClearRenderTargetView(
      ID3D11RenderTargetView* pRenderTargetView,
      const float colorRGBA[4]
    ) noexcept;

  /**
   * @brief Limpia la vista de profundidad-plantilla con los valores de profundidad y plantilla especificados.
   *
   * @param pDepthStencilView Puntero a la vista de profundidad-plantilla a limpiar.
   * @param clearFlags Banderas que indican qué partes limpiar (D3D11_CLEAR_DEPTH, D3D11_CLEAR_STENCIL).
   * @param depth Valor de profundidad para limpiar (típicamente entre 0.0f y 1.0f).
   * @param stencil Valor de plantilla para limpiar (típicamente entre 0 y 255).
   */
  void
    ClearDepthStencilView(
      ID3D11DepthStencilView* pDepthStencilView,
      unsigned int clearFlags,
      float depth,
      unsigned int stencil
    ) noexcept;

  /**
   * @brief Establece los objetivos de renderizado para la etapa de fusión de salida (Output-Merger).
   *
   * @param numViews Número de vistas de objetivos de renderizado a establecer.
   * @param ppRenderTargetViews Puntero a un arreglo de punteros a vistas de objetivos de renderizado.
   * @param pDepthStencilView Puntero a la vista de profundidad-plantilla a establecer (puede ser nullptr si no se usa).
   */
  void
    OMSetRenderTargets(
      unsigned int numViews,
      ID3D11RenderTargetView* const* ppRenderTargetViews,
      ID3D11DepthStencilView* pDepthStencilView
    ) noexcept;

  /**
   * @brief Establece las ventanas de visualización (viewports) para la etapa del rasterizador.
   *
   * @param numViewports Número de viewports a establecer.
   * @param pViewports Puntero a un arreglo de estructuras D3D11_VIEWPORT que describen las ventanas de visualización.
   */
  void
    RSSetViewports(
      unsigned int numViewports,
      const D3D11_VIEWPORT* pViewports
    ) noexcept;

  /**
   * @brief Establece el estado del rasterizador para la etapa correspondiente.
   *
   * @param pRasterizerState Puntero a la interfaz ID3D11RasterizerState que describe el estado del rasterizador.
   */
  void
    RSSetState(
      ID3D11RasterizerState* pRasterizerState
    ) noexcept;

  /**
   * @brief Establece el diseño de entrada para la etapa de ensamblaje de entrada (Input-Assembler).
   *
   * @param pInputLayout Puntero a la interfaz ID3D11InputLayout que describe el diseño de entrada.
   */
  void
    IASetInputLayout(
      ID3D11InputLayout* pInputLayout
    ) noexcept;

  /**
   * @brief Establece la topología de primitivas para la etapa de ensamblaje de entrada.
   *
   * @param topology Topología de primitivas a establecer (ej. D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST).
   */
  void
    IASetPrimitiveTopology(
      D3D11_PRIMITIVE_TOPOLOGY topology
    ) noexcept;

  /**
   * @brief Establece los búferes de vértices para la etapa de ensamblaje de entrada.
   *
   * @param startSlot Primera ranura de entrada para el enlace.
   * @param numBuffers Número de búferes de vértices a enlazar.
   * @param ppVertexBuffers Puntero a un arreglo de punteros a búferes de vértices.
   * @param pStrides Puntero a un arreglo de valores de paso (tamaño de cada vértice).
   * @param pOffsets Puntero a un arreglo de valores de desplazamiento (offset desde el inicio de cada búfer).
   */
  void
    IASetVertexBuffers(
      unsigned int startSlot,
      unsigned int numBuffers,
      ID3D11Buffer* const* ppVertexBuffers,
      const unsigned int* pStrides,
      const unsigned int* pOffsets
    ) noexcept;

  /**
   * @brief Establece el búfer de índices para la etapa de ensamblaje de entrada.
   *
   * @param pIndexBuffer Puntero al búfer de índices a establecer.
   * @param format Formato del búfer de índices (ej. DXGI_FORMAT_R32_UINT).
   * @param offset Desplazamiento en el búfer de índices.
   */
  void
    IASetIndexBuffer(
      ID3D11Buffer* pIndexBuffer,
      DXGI_FORMAT format,
      unsigned int offset
    ) noexcept;

  /**
   * @brief Establece el sombreador de vértices para la etapa de vertex-shader.
   *
   * @param pVertexShader Puntero a la interfaz ID3D11VertexShader que describe el sombreador.
   * @param ppClassInstances Puntero a un arreglo de instancias de clases (puede ser nullptr si no se usa).
   * @param numClassInstances Número de instancias de clases (puede ser 0 si no se usa).
   */
  void
    VSSetShader(
      ID3D11VertexShader* pVertexShader,
      ID3D11ClassInstance* const* ppClassInstances = nullptr,
      unsigned int numClassInstances = 0
    ) noexcept;

  /**
   * @brief Establece el sombreador de píxeles para la etapa de pixel-shader.
   *
   * @param pPixelShader Puntero a la interfaz ID3D11PixelShader que describe el sombreador.
   * @param ppClassInstances Puntero a un arreglo de instancias de clases (puede ser nullptr si no se usa).
   * @param numClassInstances Número de instancias de clases (puede ser 0 si no se usa).
   */
  void
    PSSetShader(
      ID3D11PixelShader* pPixelShader,
      ID3D11ClassInstance* const* ppClassInstances = nullptr,
      unsigned int numClassInstances = 0
    ) noexcept;

  /**
   * @brief Establece los búferes constantes para la etapa de vertex-shader.
   *
   * @param startSlot Primera ranura de búfer constante para el enlace.
   * @param numBuffers Número de búferes constantes a enlazar.
   * @param ppConstantBuffers Puntero a un arreglo de punteros a búferes constantes.
   */
  void
    VSSetConstantBuffers(
      unsigned int startSlot,
      unsigned int numBuffers,
      ID3D11Buffer* const* ppConstantBuffers
    ) noexcept;

  /**
   * @brief Actualiza un subrecurso de la GPU.
   *
   * @param pDstResource Puntero al recurso de destino.
   * @param dstSubresource Índice del subrecurso del recurso de destino.
   * @param pDstBox Puntero a la caja delimitadora para el recurso de destino.
   * @param pSrcData Puntero a los datos de origen.
   * @param srcRowPitch Paso de fila de origen.
   * @param srcDepthPitch Paso de profundidad de origen.
   */
  void
    UpdateSubresource(
      ID3D11Resource* pDstResource,
      unsigned int dstSubresource,
      const D3D11_BOX* pDstBox,
      const void* pSrcData,
      unsigned int srcRowPitch,
      unsigned int srcDepthPitch
    ) noexcept;

  /**
   * @brief Dibuja geometría indexada.
   *
   * @param indexCount Número de índices a dibujar.
   * @param startIndexLocation Ubicación del primer índice a dibujar.
   * @param baseVertexLocation Ubicación del primer vértice a utilizar.
   */
  void
    DrawIndexed(
      unsigned int indexCount,
      unsigned int startIndexLocation = 0,
      int baseVertexLocation = 0
    ) noexcept;

private:
  ID3D11DeviceContext* m_deviceContext = nullptr; ///< Puntero al contexto del dispositivo DirectX 11 subyacente.
};