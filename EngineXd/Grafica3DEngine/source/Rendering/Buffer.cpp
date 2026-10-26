#include "Rendering/Buffer.h"
#include "Rendering/Device.h"
#include "Rendering/DeviceContext.h"

HRESULT
Buffer::init(
  Device& device,
  const MeshComponent& mesh,
  unsigned int bindFlag)
{
  if (!device.getDevice())
  {
    ERROR("Buffer", "init", "Device is not initialized.");
    return E_POINTER;
  }

  if ((bindFlag & D3D11_BIND_VERTEX_BUFFER) == 0 &&
    (bindFlag & D3D11_BIND_INDEX_BUFFER) == 0)
  {
    ERROR(
      "Buffer",
      "init",
      "Bind flag must be either D3D11_BIND_VERTEX_BUFFER or D3D11_BIND_INDEX_BUFFER."
    );

    return E_INVALIDARG;
  }

  if ((bindFlag & D3D11_BIND_VERTEX_BUFFER) &&
    (bindFlag & D3D11_BIND_INDEX_BUFFER))
  {
    ERROR(
      "Buffer",
      "init",
      "Bind flag cannot be both D3D11_BIND_VERTEX_BUFFER and D3D11_BIND_INDEX_BUFFER."
    );

    return E_INVALIDARG;
  }

  D3D11_BUFFER_DESC desc = {};
  D3D11_SUBRESOURCE_DATA data = {};

  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.CPUAccessFlags = 0;

  m_bindFlag = bindFlag;
  desc.BindFlags = static_cast<D3D11_BIND_FLAG>(bindFlag);

  if (bindFlag & D3D11_BIND_VERTEX_BUFFER)
  {
    if (mesh.m_vertex.size() == 0)
    {
      ERROR(
        "Buffer",
        "init",
        "Mesh does not contain vertices."
      );

      return E_INVALIDARG;
    }

    m_numVertex = static_cast<unsigned int>(mesh.m_vertex.size());
    m_stride = sizeof(Vertex);

    desc.ByteWidth =
      static_cast<UINT>(mesh.m_vertex.size() * sizeof(Vertex));

    data.pSysMem = mesh.m_vertex.data();
  }
  else if (bindFlag & D3D11_BIND_INDEX_BUFFER)
  {
    if (mesh.m_index.size() == 0)
    {
      ERROR(
        "Buffer",
        "init",
        "Mesh does not contain indices."
      );

      return E_INVALIDARG;
    }

    m_numIndex = static_cast<unsigned int>(mesh.m_index.size());

    desc.ByteWidth =
      static_cast<UINT>(mesh.m_index.size() * sizeof(unsigned int));

    data.pSysMem = mesh.m_index.data();
  }

  return createBuffer(device, desc, &data);
}