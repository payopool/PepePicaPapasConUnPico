/**
 * @file Texture2D.cpp
 * @brief Implementación de la clase Texture2D para administrar texturas 2D.
 *
 * Contiene las funciones para inicializar, renderizar y liberar
 * los recursos de una textura utilizada por DirectX 11.
 *
 * @author
 * @date
 */

#include "Rendering/Texture2D.h"
#include "Rendering/Device.h"
#include "Rendering/DeviceContext.h"

 /**
  * @brief Destructor de la clase Texture2D.
  *
  * Libera los recursos asociados a la textura antes de destruir el objeto.
  */
Texture2D::~Texture2D() noexcept
{
  destroy();
}

/**
 * @brief Inicializa una textura a partir de un archivo de imagen.
 *
 * Comprueba que el dispositivo esté inicializado y que el nombre
 * de la textura sea válido. Después determina la extensión del archivo
 * para iniciar su carga.
 *
 * @param device Referencia al dispositivo gráfico de DirectX 11.
 * @param textureName Nombre y ruta de la textura sin extensión.
 * @param extencionType Tipo de extensión de la imagen.
 *
 * @return HRESULT Resultado de la operación.
 * @retval S_OK Si la inicialización se realiza correctamente.
 * @retval E_POINTER Si el dispositivo no está inicializado.
 * @retval E_INVALIDARG Si el nombre o la extensión no son válidos.
 * @retval E_NOTIMPL Si la carga del formato no está implementada.
 */
HRESULT 
Texture2D::init(
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

/**
 * @brief Asigna la textura al contexto gráfico para su renderizado.
 *
 * Verifica que el contexto y el recurso de textura estén inicializados.
 * Posteriormente asigna la textura al pixel shader.
 *
 * @param deviceContext Referencia al contexto gráfico de DirectX 11.
 * @param StartSlot Posición inicial donde se asignará la textura.
 * @param NumViews Cantidad de recursos de textura que se asignarán.
 */
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

/**
 * @brief Libera los recursos utilizados por la textura.
 *
 * Comprueba si los recursos existen antes de liberarlos.
 * También limpia el nombre del archivo de textura.
 */
void 
Texture2D::destroy()
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

/**
 * @brief Inicializa una textura utilizando un archivo de imagen.
 *
 * Comprueba que el dispositivo gráfico esté disponible antes
 * de continuar con el proceso de carga.
 *
 * @param device Referencia al dispositivo gráfico de DirectX 11.
 * @param textureName Ruta y nombre del archivo de imagen.
 * @param texture Referencia al objeto Texture2D que recibirá los recursos.
 *
 * @return HRESULT Resultado de la operación.
 * @retval E_POINTER Si el dispositivo no está inicializado.
 */
HRESULT 
Texture2D::InitTextureFromImage(
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