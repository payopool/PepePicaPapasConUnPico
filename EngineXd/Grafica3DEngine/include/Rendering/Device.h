/**
 * @file Device.h
 * @brief Declaración de la clase Device para la gestión del dispositivo gráfico de DirectX 11.
 */

#pragma once

#include "Engine/Prerequisitos.h"

 /**
  * @class Device
  * @brief Envuelve y gestiona el dispositivo principal de DirectX 11 (`ID3D11Device`) y la creación de recursos gráficos.
  */
class Device
{
public:
  /**
   * @brief Construye una nueva instancia por defecto de Device.
   */
  Device() = default;

  /**
   * @brief Destruye la instancia y libera los recursos del dispositivo.
   */
  ~Device();

  /**
   * @brief Inicializa el dispositivo gráfico de DirectX 11.
   */
  void
    init();

  /**
   * @brief Destruye y libera de forma segura los recursos del dispositivo.
   */
  void
    destroy();

  /**
   * @brief Obtiene el puntero al dispositivo de DirectX 11 subyacente.
   *
   * @return ID3D11Device* Puntero al dispositivo de DirectX 11.
   */
  ID3D11Device*
    getDevice() const noexcept
  {
    return m_device;
  }

  /**
   * @brief Obtiene la dirección del puntero al dispositivo de DirectX 11.
   *
   * @return ID3D11Device** Dirección del puntero al dispositivo.
   */
  ID3D11Device**
    getDeviceAddress() noexcept
  {
    return &m_device;
  }

  /**
   * @brief Crea una vista de objetivo de renderizado para acceder a los datos de un recurso.
   *
   * @param pResource Puntero al recurso que representa el objetivo de renderizado.
   * @param pDesc Puntero a una descripción de la vista de objetivo de renderizado (D3D11_RENDER_TARGET_VIEW_DESC).
   * @param ppRTView Dirección del puntero donde se almacenará la interfaz ID3D11RenderTargetView creada.
   * @return HRESULT S_OK si se creó con éxito, o un código de error de DirectX.
   */
  HRESULT
    CreateRenderTargetView(
      ID3D11Resource* pResource,
      const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
      ID3D11RenderTargetView** ppRTView
    );

  /**
   * @brief Crea una textura bidimensional (Texture2D).
   *
   * @param pDesc Puntero a la descripción de la textura 2D (D3D11_TEXTURE2D_DESC).
   * @param pInitialData Puntero a los datos de inicialización del subrecurso.
   * @param ppTexture2D Dirección del puntero donde se almacenará la interfaz ID3D11Texture2D creada.
   * @return HRESULT S_OK si se creó con éxito, o un código de error de DirectX.
   */
  HRESULT
    CreateTexture2D(
      const D3D11_TEXTURE2D_DESC* pDesc,
      const D3D11_SUBRESOURCE_DATA* pInitialData,
      ID3D11Texture2D** ppTexture2D
    );

  /**
   * @brief Crea una vista de profundidad-plantilla para acceder a un recurso de profundidad.
   *
   * @param pResource Puntero al recurso que se utilizará como búfer de profundidad-plantilla.
   * @param pDesc Puntero a la descripción de la vista de profundidad-plantilla (D3D11_DEPTH_STENCIL_VIEW_DESC).
   * @param ppDepthStencilView Dirección del puntero donde se almacenará la interfaz ID3D11DepthStencilView creada.
   * @return HRESULT S_OK si se creó con éxito, o un código de error de DirectX.
   */
  HRESULT
    CreateDepthStencilView(
      ID3D11Resource* pResource,
      const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
      ID3D11DepthStencilView** ppDepthStencilView
    );

  /**
   * @brief Crea un sombreador de vértices (Vertex Shader) a partir de un bytecode compilado.
   *
   * @param pShaderBytecode Puntero al bytecode del sombreador compilado.
   * @param BytecodeLength Tamaño en bytes del bytecode del sombreador.
   * @param pClassLinkage Puntero a una interfaz de enlace de clases (ID3D11ClassLinkage), puede ser nullptr.
   * @param ppVertexShader Dirección del puntero donde se almacenará la interfaz ID3D11VertexShader creada.
   * @return HRESULT S_OK si se creó con éxito, o un código de error de DirectX.
   */
  HRESULT
    CreateVertexShader(
      const void* pShaderBytecode,
      SIZE_T BytecodeLength,
      ID3D11ClassLinkage* pClassLinkage,
      ID3D11VertexShader** ppVertexShader
    );

  /**
   * @brief Crea un sombreador de píxeles (Pixel Shader) a partir de un bytecode compilado.
   *
   * @param pShaderBytecode Puntero al bytecode del sombreador compilado.
   * @param BytecodeLength Tamaño en bytes del bytecode del sombreador.
   * @param pClassLinkage Puntero a una interfaz de enlace de clases, puede ser nullptr.
   * @param ppPixelShader Dirección del puntero donde se almacenará la interfaz ID3D11PixelShader creada.
   * @return HRESULT S_OK si se creó con éxito, o un código de error de DirectX.
   */
  HRESULT
    CreatePixelShader(
      const void* pShaderBytecode,
      SIZE_T BytecodeLength,
      ID3D11ClassLinkage* pClassLinkage,
      ID3D11PixelShader** ppPixelShader
    );

  /**
   * @brief Crea un diseño de entrada (Input Layout) para describir los datos de los vértices a la etapa de ensamblaje.
   *
   * @param pInputElementDescs Arreglo de descripciones de elementos de entrada (D3D11_INPUT_ELEMENT_DESC).
   * @param NumElements Número de elementos de entrada en el arreglo.
   * @param pShaderBytecodeWithInputSignature Puntero al bytecode del vertex shader con la firma de entrada.
   * @param BytecodeLength Tamaño en bytes del bytecode.
   * @param ppInputLayout Dirección del puntero donde se almacenará la interfaz ID3D11InputLayout creada.
   * @return HRESULT S_OK si se creó con éxito, o un código de error de DirectX.
   */
  HRESULT
    CreateInputLayout(
      const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
      UINT NumElements,
      const void* pShaderBytecodeWithInputSignature,
      SIZE_T BytecodeLength,
      ID3D11InputLayout** ppInputLayout
    );

  /**
   * @brief Crea un búfer (de vértices, índices o constantes).
   *
   * @param pDesc Puntero a la descripción del búfer (D3D11_BUFFER_DESC).
   * @param pInitialData Puntero a los datos de inicialización.
   * @param ppBuffer Dirección del puntero donde se almacenará la interfaz ID3D11Buffer creada.
   * @return HRESULT S_OK si se creó con éxito, o un código de error de DirectX.
   */
  HRESULT
    CreateBuffer(
      const D3D11_BUFFER_DESC* pDesc,
      const D3D11_SUBRESOURCE_DATA* pInitialData,
      ID3D11Buffer** ppBuffer
    );

  /**
   * @brief Crea un estado de rasterizador (Rasterizer State) para controlar la etapa de rasterización.
   *
   * @param pRasterizerDesc Puntero a la descripción del rasterizador (D3D11_RASTERIZER_DESC).
   * @param ppRasterizerState Dirección del puntero donde se almacenará la interfaz ID3D11RasterizerState creada.
   * @return HRESULT S_OK si se creó con éxito, o un código de error de DirectX.
   */
  HRESULT
    CreateRasterizerState(
      const D3D11_RASTERIZER_DESC* pRasterizerDesc,
      ID3D11RasterizerState** ppRasterizerState
    );

private:
  ID3D11Device* m_device = nullptr;                 ///< Puntero al dispositivo de DirectX 11 subyacente.
  D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_11_0; ///< Nivel de características de DirectX compatible.
};