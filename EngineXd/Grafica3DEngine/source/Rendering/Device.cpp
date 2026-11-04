/**
 * @file Device.cpp
 * @brief Implementación de la clase Device para la inicialización y creación de recursos en el dispositivo de DirectX 11.
 */

#include "Rendering/Device.h"

 /**
  * @brief Inicializa el dispositivo gráfico de DirectX 11 usando el tipo de controlador por hardware.
  */
void
Device::init()
{
  constexpr D3D_FEATURE_LEVEL supportedFeatureLevels[]
  {
      D3D_FEATURE_LEVEL_11_0
  };

  HRESULT result =
    D3D11CreateDevice(
      nullptr,
      D3D_DRIVER_TYPE_HARDWARE,
      nullptr,
      0,
      supportedFeatureLevels,
      ARRAYSIZE(supportedFeatureLevels),
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

/**
 * @brief Destructor de la clase Device que invoca la liberación de recursos.
 */
Device::~Device()
{
  destroy();
}

/**
 * @brief Libera de forma segura el dispositivo DirectX 11 mediante la macro SafeRelease.
 */
void
Device::destroy()
{
  SafeRelease(m_device);
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
Device::CreateRenderTargetView(
  ID3D11Resource* pResource,
  const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
  ID3D11RenderTargetView** ppRTView
)
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

/**
 * @brief Crea una textura bidimensional (Texture2D).
 *
 * @param pDesc Puntero a la descripción de la textura 2D (D3D11_TEXTURE2D_DESC).
 * @param pInitialData Puntero a los datos de inicialización del subrecurso.
 * @param ppTexture2D Dirección del puntero donde se almacenará la interfaz ID3D11Texture2D creada.
 * @return HRESULT S_OK si se creó con éxito, o un código de error de DirectX.
 */
HRESULT
Device::CreateTexture2D(
  const D3D11_TEXTURE2D_DESC* pDesc,
  const D3D11_SUBRESOURCE_DATA* pInitialData,
  ID3D11Texture2D** ppTexture2D
)
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

/**
 * @brief Crea una vista de profundidad-plantilla para acceder a un recurso de profundidad.
 *
 * @param pResource Puntero al recurso que se utilizará como búfer de profundidad-plantilla.
 * @param pDesc Puntero a la descripción de la vista de profundidad-plantilla (D3D11_DEPTH_STENCIL_VIEW_DESC).
 * @param ppDepthStencilView Dirección del puntero donde se almacenará la interfaz ID3D11DepthStencilView creada.
 * @return HRESULT S_OK si se creó con éxito, o un código de error de DirectX.
 */
HRESULT
Device::CreateDepthStencilView(
  ID3D11Resource* pResource,
  const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
  ID3D11DepthStencilView** ppDepthStencilView
)
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
Device::CreateVertexShader(
  const void* pShaderBytecode,
  SIZE_T BytecodeLength,
  ID3D11ClassLinkage* pClassLinkage,
  ID3D11VertexShader** ppVertexShader
)
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
Device::CreatePixelShader(
  const void* pShaderBytecode,
  SIZE_T BytecodeLength,
  ID3D11ClassLinkage* pClassLinkage,
  ID3D11PixelShader** ppPixelShader
)
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
Device::CreateInputLayout(
  const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
  UINT NumElements,
  const void* pShaderBytecodeWithInputSignature,
  SIZE_T BytecodeLength,
  ID3D11InputLayout** ppInputLayout
)
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

/**
 * @brief Crea un búfer (de vértices, índices o constantes).
 *
 * @param pDesc Puntero a la descripción del búfer (D3D11_BUFFER_DESC).
 * @param pInitialData Puntero a los datos de inicialización.
 * @param ppBuffer Dirección del puntero donde se almacenará la interfaz ID3D11Buffer creada.
 * @return HRESULT S_OK si se creó con éxito, o un código de error de DirectX.
 */
HRESULT
Device::CreateBuffer(
  const D3D11_BUFFER_DESC* pDesc,
  const D3D11_SUBRESOURCE_DATA* pInitialData,
  ID3D11Buffer** ppBuffer
)
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

/**
 * @brief Crea un estado de rasterizador (Rasterizer State) para controlar la etapa de rasterización.
 *
 * @param pRasterizerDesc Puntero a la descripción del rasterizador (D3D11_RASTERIZER_DESC).
 * @param ppRasterizerState Dirección del puntero donde se almacenará la interfaz ID3D11RasterizerState creada.
 * @return HRESULT S_OK si se creó con éxito, o un código de error de DirectX.
 */
HRESULT
Device::CreateRasterizerState(
  const D3D11_RASTERIZER_DESC* pRasterizerDesc,
  ID3D11RasterizerState** ppRasterizerState
)
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