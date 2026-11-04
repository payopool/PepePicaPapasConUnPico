/**
 * @file Buffer.cpp
 * @brief Implementación de la clase Buffer para la gestión de búferes de DirectX 11 (Vértices, Índices y Constantes).
 */

#include "Rendering/Buffer.h"
#include "Rendering/Device.h"
#include "Rendering/DeviceContext.h"

 /**
  * @brief Inicializa el búfer como un búfer de vértices o de índices utilizando los datos de una malla.
  *
  * @param device Referencia al dispositivo gráfico de DirectX 11.
  * @param mesh Datos de la malla que contienen los vértices e índices.
  * @param bindFlag Indicador de tipo de enlace (ej. D3D11_BIND_VERTEX_BUFFER o D3D11_BIND_INDEX_BUFFER).
  * @return HRESULT S_OK si la inicialización fue exitosa, o un código de error en caso contrario.
  */
HRESULT
Buffer::init(
  Device& device,
  const MeshComponent& mesh,
  unsigned int bindFlag
)
{
  if (!device.getDevice())
  {
    ERROR("Buffer", "init", "Device is null.");
    return E_POINTER;
  }

  if ((bindFlag & D3D11_BIND_VERTEX_BUFFER) && mesh.m_vertex.empty())
  {
    ERROR("Buffer", "init", "Vertex buffer is empty");
    return E_INVALIDARG;
  }

  if ((bindFlag & D3D11_BIND_INDEX_BUFFER) && mesh.m_index.empty())
  {
    ERROR("Buffer", "init", "Index buffer is empty");
    return E_INVALIDARG;
  }

  D3D11_BUFFER_DESC desc = {};
  D3D11_SUBRESOURCE_DATA data = {};

  desc.Usage = D3D11_USAGE_IMMUTABLE;
  desc.CPUAccessFlags = 0;
  m_bindFlag = bindFlag;
  desc.BindFlags = (D3D11_BIND_FLAG)bindFlag;

  if (bindFlag & D3D11_BIND_VERTEX_BUFFER)
  {
    m_stride = sizeof(Vertex);

    desc.ByteWidth =
      m_stride * static_cast<unsigned int>(mesh.m_vertex.size());

    data.pSysMem = mesh.m_vertex.data();
  }
  else if (bindFlag & D3D11_BIND_INDEX_BUFFER)
  {
    m_stride = sizeof(unsigned int);

    desc.ByteWidth =
      m_stride * static_cast<unsigned int>(mesh.m_index.size());

    data.pSysMem = mesh.m_index.data();
  }

  return createBuffer(device, desc, &data);
}

/**
 * @brief Inicializa el búfer como un búfer constante con un ancho de bytes específico.
 *
 * @param device Referencia al dispositivo gráfico de DirectX 11.
 * @param ByteWidth Tamaño en bytes del búfer constante.
 * @return HRESULT S_OK si la inicialización fue exitosa, o un código de error en caso contrario.
 */
HRESULT
Buffer::init(
  Device& device,
  unsigned int ByteWidth
)
{
  if (!device.getDevice())
  {
    ERROR("ShaderProgram", "init", "Device is null.");
    return E_POINTER;
  }

  if (ByteWidth == 0)
  {
    ERROR("Buffer", "init", "ByteWidth is zero");
    return E_INVALIDARG;
  }

  m_stride = ByteWidth;

  D3D11_BUFFER_DESC desc = {};
  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.ByteWidth = ByteWidth;
  desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

  m_bindFlag = desc.BindFlags;

  return createBuffer(device, desc, nullptr);
}

/**
 * @brief Actualiza los datos de un subrecurso en la GPU.
 *
 * @param deviceContext Contexto del dispositivo gráfico utilizado para enviar la orden.
 * @param pDstResource Recurso de destino que se desea actualizar.
 * @param DstSubresource Índice del subrecurso de destino.
 * @param pDstBox Caja opcional que define la porción del recurso a actualizar.
 * @param pSrcData Puntero a los datos de origen en la memoria del sistema.
 * @param SrcRowPitch Distancia en bytes de una fila de datos.
 * @param SrcDepthPitch Distancia en bytes de una profundidad de datos.
 */
void
Buffer::update(
  DeviceContext& deviceContext,
  ID3D11Resource* pDstResource,
  unsigned int DstSubresource,
  const D3D11_BOX* pDstBox,
  const void* pSrcData,
  unsigned int SrcRowPitch,
  unsigned int SrcDepthPitch
)
{
  if (!m_buffer)
  {
    ERROR("ShaderProgram", "update", "m_buffer is null.");
    return;
  }

  if (!pSrcData)
  {
    ERROR("ShaderProgram", "update", "pSrcData is null.");
    return;
  }

  deviceContext.getDeviceContext()->UpdateSubresource(
    m_buffer,
    DstSubresource,
    pDstBox,
    pSrcData,
    SrcRowPitch,
    SrcDepthPitch
  );
}

/**
 * @brief Vincula y renderiza el búfer según su tipo (Vértices, Índices o Constantes).
 *
 * @param deviceContext Contexto del dispositivo de gráficos.
 * @param StartSlot Índice del primer búfer en la etapa de ensamblaje de entrada o sombreadores.
 * @param NumBuffers Número de búferes a enlazar.
 * @param setPixelShader Indica si el búfer constante también debe asociarse al Pixel Shader.
 * @param format Formato DXGI para los búferes de índices (ej. DXGI_FORMAT_R32_UINT).
 */
void
Buffer::render(
  DeviceContext& deviceContext,
  unsigned int StartSlot,
  unsigned int NumBuffers,
  bool setPixelShader,
  DXGI_FORMAT format
)
{
  if (!deviceContext.getDeviceContext())
  {
    ERROR("RenderTargetView", "render", "DeviceContext is nullptr.");
    return;
  }

  if (!m_buffer)
  {
    ERROR("Buffer", "render", "m_buffer is null.");
    return;
  }

  switch (m_bindFlag)
  {
  case D3D11_BIND_VERTEX_BUFFER:
    deviceContext.getDeviceContext()->IASetVertexBuffers(
      StartSlot,
      NumBuffers,
      &m_buffer,
      &m_stride,
      &m_offset
    );
    break;

  case D3D11_BIND_CONSTANT_BUFFER:
    deviceContext.getDeviceContext()->VSSetConstantBuffers(
      StartSlot,
      NumBuffers,
      &m_buffer
    );

    if (setPixelShader)
    {
      deviceContext.getDeviceContext()->PSSetConstantBuffers(
        StartSlot,
        NumBuffers,
        &m_buffer
      );
    }
    break;

  case D3D11_BIND_INDEX_BUFFER:
    deviceContext.getDeviceContext()->IASetIndexBuffer(
      m_buffer,
      format,
      m_offset
    );
    break;

  default:
    ERROR("Buffer", "render", "Unsupported BindFlag");
    break;
  }
}

/**
 * @brief Libera de forma segura el recurso de búfer de DirectX mediante la macro SAFE_RELEASE.
 */
void
Buffer::destroy()
{
  SAFE_RELEASE(m_buffer);
}

/**
 * @brief Función interna de apoyo para crear el objeto ID3D11Buffer en la GPU.
 *
 * @param device Referencia al dispositivo de DirectX 11.
 * @param desc Descripción de la estructura del búfer (D3D11_BUFFER_DESC).
 * @param initData Puntero opcional a los datos de inicialización del subrecurso.
 * @return HRESULT S_OK si el búfer fue creado con éxito, o un código de error de DirectX.
 */
HRESULT
Buffer::createBuffer(
  Device& device,
  D3D11_BUFFER_DESC& desc,
  D3D11_SUBRESOURCE_DATA* initData
)
{
  if (!device.getDevice())
  {
    ERROR("Buffer", "createBuffer", "Device is nullptr");
    return E_POINTER;
  }

  HRESULT hr =
    device.getDevice()->CreateBuffer(
      &desc,
      initData,
      &m_buffer
    );

  if (FAILED(hr))
  {
    ERROR("Buffer", "createBuffer", "Failed to create buffer");
    return hr;
  }

  return S_OK;
}