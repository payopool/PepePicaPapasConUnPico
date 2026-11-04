/**
 * @file Texture2D.h
 * @brief Declaración de la clase Texture2D para la gestión y creación de texturas bidimensionales y cubemap en DirectX 11.
 */

#pragma once

#include "Engine/Prerequisitos.h"

class Device;
class DeviceContext;

/**
 * @class Texture2D
 * @brief Administra la creación, carga desde archivos, vistas de recursos y renderizado de texturas 2D y mapas cúbicos.
 */
class Texture2D
{
public:
  /**
   * @brief Construye una nueva instancia por defecto de Texture2D.
   */
  Texture2D() noexcept = default;

  /**
   * @brief Destruye la instancia y libera los recursos gráficos asociados.
   */
  ~Texture2D() noexcept;

  // Eliminación de constructores de copia y operadores de asignación
  Texture2D(const Texture2D&) = delete;
  Texture2D& operator=(const Texture2D&) = delete;

  // Eliminación de constructores de movimiento y operadores de asignación por movimiento
  Texture2D(Texture2D&&) = delete;
  Texture2D& operator=(Texture2D&&) = delete;

  /**
   * @brief Inicializa y carga una textura 2D desde un archivo de imagen.
   *
   * @param device Referencia al dispositivo gráfico de DirectX 11.
   * @param textureName Ruta o nombre del archivo de imagen a cargar.
   * @param extencionType Tipo de extensión de la imagen.
   * @return HRESULT S_OK si la carga fue exitosa, o un código de error en caso contrario.
   */
  HRESULT
    init(
      Device& device,
      const std::string& textureName,
      ExtensionType extencionType
    );

  /**
   * @brief Inicializa una textura 2D vacía con dimensiones y formatos específicos para renderizado.
   *
   * @param device Referencia al dispositivo gráfico de DirectX 11.
   * @param width Ancho de la textura en píxeles.
   * @param height Alto de la textura en píxeles.
   * @param Format Formato DXGI de la textura.
   * @param BindFlags Banderas de enlace (ej. Shader Resource, Render Target).
   * @param sampleCount Conteo de muestras para antialiasing (por defecto 1).
   * @param qualityLevels Niveles de calidad para el muestreo (por defecto 0).
   * @return HRESULT S_OK si la creación fue exitosa, o un código de error en caso contrario.
   */
  HRESULT
    init(
      Device& device,
      unsigned int width,
      unsigned int height,
      DXGI_FORMAT Format,
      unsigned int BindFlags,
      unsigned int sampleCount = 1,
      unsigned int qualityLevels = 0
    );

  /**
   * @brief Inicializa una textura basada en otra textura de referencia existente.
   *
   * @param device Referencia al dispositivo gráfico de DirectX 11.
   * @param textureRef Referencia a la textura base.
   * @param Format Formato DXGI a aplicar.
   * @return HRESULT S_OK si la inicialización fue exitosa, o un código de error en caso contrario.
   */
  HRESULT
    init(
      Device& device,
      Texture2D& textureRef,
      DXGI_FORMAT Format
    );

  /**
   * @brief Actualiza los datos de la textura por fotograma si es necesario.
   */
  void
    update();

  /**
   * @brief Vincula la vista de la textura a los sombreadores.
   *
   * @param deviceContext Contexto del dispositivo gráfico.
   * @param StartSlot Ranura inicial de vinculación.
   * @param NumViews Número de vistas a enlazar.
   */
  void
    render(
      DeviceContext& deviceContext,
      unsigned int StartSlot,
      unsigned int NumViews
    );

  /**
   * @brief Destruye y libera de forma segura los recursos de la textura en la GPU.
   */
  void
    destroy();

  /**
   * @brief Crea un mapa cúbico (CubeMap) a partir de seis imágenes independientes.
   *
   * @param device Referencia al dispositivo gráfico de DirectX 11.
   * @param deviceContext Contexto del dispositivo gráfico.
   * @param FacePaths Arreglo con las rutas de las 6 caras del cubo.
   * @param generateMipMaps Indica si se deben generar MipMaps automáticamente.
   * @return HRESULT S_OK si el CubeMap se creó con éxito, o un código de error.
   */
  HRESULT
    CreateCubeMap(
      Device& device,
      DeviceContext& deviceContext,
      const std::array<std::string, 6>& FacePaths,
      bool generateMipMaps
    );

  /**
   * @brief Crea una vista de recurso de sombreador (SRV) para una cara específica de un CubeMap.
   *
   * @param device Puntero al dispositivo de DirectX 11.
   * @param cubeMapTexture Puntero a la textura principal del CubeMap.
   * @param format Formato DXGI de la textura.
   * @param faceIndex Índice de la cara del cubo a procesar.
   * @param mipLevel Nivel de MipMap (por defecto 1).
   * @return ID3D11ShaderResourceView* Puntero a la vista de recurso de sombreador creada.
   */
  ID3D11ShaderResourceView*
    CreateCubemapFaceSRV(
      ID3D11Device* device,
      ID3D11Texture2D* cubeMapTexture,
      DXGI_FORMAT format,
      unsigned int faceIndex,
      unsigned int mipLevel = 1
    );

public:
  ID3D11Texture2D* m_texture = nullptr;                 ///< Puntero al recurso de textura subyacente en DirectX 11.
  ID3D11ShaderResourceView* m_TextureFromImg = nullptr; ///< Vista de recurso de sombreador para la textura cargada desde imagen.
  std::string m_textureName;                            ///< Nombre identificador de la textura.

private:
  /**
   * @brief Función auxiliar interna para cargar y configurar los datos de una textura a partir de un archivo de imagen.
   *
   * @param device Referencia al dispositivo gráfico.
   * @param textureName Ruta del archivo de imagen.
   * @param m_texture Referencia a la textura donde se almacenarán los datos.
   * @return HRESULT S_OK si se cargó correctamente, o un código de error.
   */
  HRESULT
    InitTextureFromImage(
      Device& device,
      const std::string& textureName,
      Texture2D& m_texture
    );
};