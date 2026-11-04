/**
 * @file Engine.cpp
 * @brief Implementación de la clase Engine para la gestión del ciclo de vida, inicialización y renderizado del motor gráfico 3D en DirectX 11.
 */

#include <Engine/Engine.h>
#include <Rendering/Device.h>
#include <Rendering/DeviceContext.h>
#include <Rendering/Texture2D.h>
#include <Engine/Prerequisitos.h>

 /**
  * @struct Engine::Implementation
  * @brief Estructura interna (patrón PImpl) que encapsula todos los recursos, dispositivos, búferes y estados de DirectX 11 del motor.
  */
struct Engine::Implementation
{
  /**
   * @struct Vertex
   * @brief Define la estructura de un vértice con posición espacial y color para el motor.
   */
  struct Vertex
  {
    float position[3]; ///< Coordenadas espaciales (X, Y, Z).
    float color[4];    ///< Componentes de color RGBA.
  };

  /**
   * @struct TransformBuffer
   * @brief Búfer constante alineado a 16 bytes para la matriz de transformación World-View-Projection.
   */
  struct alignas(16) TransformBuffer
  {
    DirectX::XMFLOAT4X4 worldViewProjection; ///< Matriz WVP combinada.
  };

  HWND window = nullptr;                 ///< Identificador de la ventana nativa de la aplicación.

  std::uint32_t width = 0;               ///< Ancho actual del área de renderizado.
  std::uint32_t height = 0;              ///< Alto actual del área de renderizado.

  Device device;                         ///< Instancia del dispositivo gráfico encapsulado.
  DeviceContext context;                 ///< Instancia del contexto del dispositivo encapsulado.
  Texture2D texture;                     ///< Textura bidimensional cargada para el motor.

  IDXGISwapChain* swapChain = nullptr;   ///< Cadena de intercambio de DirectX.

  ID3D11RenderTargetView* renderTarget = nullptr; ///< Vista de objetivo de renderizado principal.

  ID3D11Texture2D* depthStencilBuffer = nullptr;       ///< Búfer de textura para profundidad y plantilla.
  ID3D11DepthStencilView* depthStencilView = nullptr; ///< Vista del búfer de profundidad y plantilla.

  ID3D11Buffer* vertexBuffer = nullptr;    ///< Búfer de vértices de la geometría.
  ID3D11Buffer* indexBuffer = nullptr;     ///< Búfer de índices de la geometría.
  ID3D11Buffer* transformBuffer = nullptr; ///< Búfer constante de transformaciones.

  ID3D11RasterizerState* rasterizerState = nullptr; ///< Estado del rasterizador (culling, fill mode).

  ID3D11VertexShader* vertexShader = nullptr;   ///< Sombreador de vértices compilado.
  ID3D11PixelShader* pixelShader = nullptr;     ///< Sombreador de píxeles compilado.
  ID3D11InputLayout* inputLayout = nullptr;     ///< Diseño de entrada de los vértices.

  std::chrono::steady_clock::time_point startTime{}; ///< Tiempo de inicio para calcular animaciones basadas en tiempo.

  /**
   * @brief Compila un archivo de sombreador (HLSL) en tiempo de ejecución generando su respectivo Blob.
   *
   * @param filename Ruta del archivo de código fuente HLSL.
   * @param entryPoint Nombre de la función de entrada en el sombreador (ej. "VSMain", "PSMain").
   * @param shaderModel Versión del modelo de sombreador (ej. "vs_5_0", "ps_5_0").
   * @param shaderBlob Puntero donde se almacenará el objeto ID3DBlob resultante con el bytecode.
   * @return true si la compilación fue exitosa, de lo contrario false.
   */
  static bool
    CompileShader(
      const wchar_t* filename,
      const char* entryPoint,
      const char* shaderModel,
      ID3DBlob** shaderBlob
    ) noexcept
  {
    if (!filename ||
      !entryPoint ||
      !shaderModel ||
      !shaderBlob)
    {
      return false;
    }

    *shaderBlob = nullptr;

    UINT compileFlags =
      D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
    compileFlags |= D3DCOMPILE_DEBUG;
    compileFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

    ID3DBlob* errors = nullptr;

    const HRESULT result =
      D3DCompileFromFile(
        filename,
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entryPoint,
        shaderModel,
        compileFlags,
        0,
        shaderBlob,
        &errors
      );

    if (errors)
    {
      OutputDebugStringA(
        static_cast<const char*>(
          errors->GetBufferPointer()
          )
      );

      SafeRelease(errors);
    }

    if (FAILED(result))
    {
      SafeRelease(*shaderBlob);
      return false;
    }

    return true;
  }

  /**
   * @brief Libera de forma segura todos los recursos, sombreadores, búferes y vistas asignados en la GPU.
   */
  void
    ReleaseResources() noexcept
  {
    SafeRelease(rasterizerState);

    SafeRelease(transformBuffer);
    SafeRelease(indexBuffer);
    SafeRelease(vertexBuffer);

    SafeRelease(inputLayout);
    SafeRelease(pixelShader);
    SafeRelease(vertexShader);

    SafeRelease(depthStencilView);
    SafeRelease(depthStencilBuffer);

    SafeRelease(renderTarget);

    SafeRelease(swapChain);

    texture.destroy();

    context.destroy();
    device.destroy();

    window = nullptr;
    width = 0;
    height = 0;
  }
};

/**
 * @brief Construye una nueva instancia del motor gráfico inicializando el contenedor PImpl.
 */
Engine::Engine() noexcept
  : m_implementation(
    new (std::nothrow) Implementation{}
  )
{}

/**
 * @brief Destructor de la clase Engine que asegura la liberación de recursos.
 */
Engine::~Engine() noexcept
{
  Shutdown();

  delete m_implementation;
  m_implementation = nullptr;
}

/**
 * @brief Inicializa el motor gráfico, la ventana de intercambio, los búferes, las texturas y compila los sombreadores.
 *
 * @param nativeWindow Identificador de la ventana nativa (HWND).
 * @param width Ancho inicial de la ventana.
 * @param height Alto inicial de la ventana.
 * @return true si la inicialización fue completamente exitosa, de lo contrario false.
 */
bool
Engine::Initialize(
  void* nativeWindow,
  std::uint32_t width,
  std::uint32_t height
) noexcept
{
  if (!m_implementation ||
    !nativeWindow ||
    width == 0 ||
    height == 0)
  {
    return false;
  }

  Implementation& engine =
    *m_implementation;

  engine.ReleaseResources();

  engine.window =
    static_cast<HWND>(nativeWindow);

  engine.width = width;
  engine.height = height;

  /** @brief Configura la cadena de intercambio que presenta los fotogramas en pantalla. */
// ========================================================
// SWAP CHAIN
// ========================================================

  DXGI_SWAP_CHAIN_DESC swapChainDescription{};

  swapChainDescription.BufferCount = 2;

  swapChainDescription.BufferDesc.Width =
    engine.width;

  swapChainDescription.BufferDesc.Height =
    engine.height;

  swapChainDescription.BufferDesc.Format =
    DXGI_FORMAT_R8G8B8A8_UNORM;

  swapChainDescription.BufferDesc.RefreshRate.Numerator =
    60;

  swapChainDescription.BufferDesc.RefreshRate.Denominator =
    1;

  swapChainDescription.BufferUsage =
    DXGI_USAGE_RENDER_TARGET_OUTPUT;

  swapChainDescription.OutputWindow =
    engine.window;

  swapChainDescription.SampleDesc.Count =
    1;

  swapChainDescription.SampleDesc.Quality =
    0;

  swapChainDescription.Windowed =
    TRUE;

  swapChainDescription.SwapEffect =
    DXGI_SWAP_EFFECT_DISCARD;

  /** @brief Crea el dispositivo y el contexto de Direct3D 11, con una alternativa WARP si falla el hardware. */
// ========================================================
// DIRECT3D 11
// ========================================================

  constexpr D3D_FEATURE_LEVEL featureLevels[]
  {
      D3D_FEATURE_LEVEL_11_0
  };

  D3D_FEATURE_LEVEL selectedFeatureLevel{};

  HRESULT result =
    D3D11CreateDeviceAndSwapChain(
      nullptr,
      D3D_DRIVER_TYPE_HARDWARE,
      nullptr,
      0,
      featureLevels,
      ARRAYSIZE(featureLevels),
      D3D11_SDK_VERSION,
      &swapChainDescription,
      &engine.swapChain,
      engine.device.getDeviceAddress(),
      &selectedFeatureLevel,
      engine.context.getDeviceContextAddress()
    );

  if (FAILED(result))
  {
    SafeRelease(engine.swapChain);

    engine.context.destroy();
    engine.device.destroy();

    result =
      D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_WARP,
        nullptr,
        0,
        featureLevels,
        ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION,
        &swapChainDescription,
        &engine.swapChain,
        engine.device.getDeviceAddress(),
        &selectedFeatureLevel,
        engine.context.getDeviceContextAddress()
      );
  }

  if (FAILED(result))
  {
    engine.ReleaseResources();
    return false;
  }

  /** @brief Inicializa la textura que utilizará la geometría durante el renderizado. */
// ========================================================
// TEXTURA
// ========================================================

  result =
    engine.texture.init(
      engine.device,
      "textures\\cubo",
      PNG
    );

  if (FAILED(result))
  {
    engine.ReleaseResources();
    return false;
  }

  /** @brief Obtiene el búfer trasero de la cadena de intercambio. */
// ========================================================
// BACK BUFFER
// ========================================================

  ID3D11Texture2D* backBuffer = nullptr;

  result =
    engine.swapChain->GetBuffer(
      0,
      __uuidof(ID3D11Texture2D),
      reinterpret_cast<void**>(
        &backBuffer
        )
    );

  if (FAILED(result))
  {
    engine.ReleaseResources();
    return false;
  }

  /** @brief Crea la vista que permite utilizar el búfer trasero como destino de renderizado. */
// ========================================================
// RENDER TARGET
// ========================================================

  result =
    engine.device.CreateRenderTargetView(
      backBuffer,
      nullptr,
      &engine.renderTarget
    );

  SafeRelease(backBuffer);

  if (FAILED(result))
  {
    engine.ReleaseResources();
    return false;
  }

  /** @brief Define el área de la ventana donde se dibujará la escena. */
// ========================================================
// VIEWPORT
// ========================================================

  D3D11_VIEWPORT viewport{};

  viewport.TopLeftX = 0.0f;
  viewport.TopLeftY = 0.0f;

  viewport.Width =
    static_cast<float>(
      engine.width
      );

  viewport.Height =
    static_cast<float>(
      engine.height
      );

  viewport.MinDepth = 0.0f;
  viewport.MaxDepth = 1.0f;

  engine.context.RSSetViewports(
    1,
    &viewport
  );

  /** @brief Crea el búfer utilizado para almacenar la profundidad de los fragmentos. */
// ========================================================
// DEPTH BUFFER
// ========================================================

  D3D11_TEXTURE2D_DESC depthBufferDescription{};

  depthBufferDescription.Width =
    engine.width;

  depthBufferDescription.Height =
    engine.height;

  depthBufferDescription.MipLevels =
    1;

  depthBufferDescription.ArraySize =
    1;

  depthBufferDescription.Format =
    DXGI_FORMAT_D24_UNORM_S8_UINT;

  depthBufferDescription.SampleDesc.Count =
    1;

  depthBufferDescription.SampleDesc.Quality =
    0;

  depthBufferDescription.Usage =
    D3D11_USAGE_DEFAULT;

  depthBufferDescription.BindFlags =
    D3D11_BIND_DEPTH_STENCIL;

  result =
    engine.device.CreateTexture2D(
      &depthBufferDescription,
      nullptr,
      &engine.depthStencilBuffer
    );

  if (FAILED(result))
  {
    engine.ReleaseResources();
    return false;
  }

  /** @brief Crea la vista que permite utilizar el búfer de profundidad en el pipeline. */
// ========================================================
// DEPTH STENCIL VIEW
// ========================================================

  result =
    engine.device.CreateDepthStencilView(
      engine.depthStencilBuffer,
      nullptr,
      &engine.depthStencilView
    );

  if (FAILED(result))
  {
    engine.ReleaseResources();
    return false;
  }

  /** @brief Compila los programas HLSL de vértices y píxeles. */
// ========================================================
// SHADERS
// ========================================================

  ID3DBlob* vertexShaderBlob = nullptr;
  ID3DBlob* pixelShaderBlob = nullptr;

  if (!Implementation::CompileShader(
    L"shaders\\Cube.hlsl",
    "VSMain",
    "vs_5_0",
    &vertexShaderBlob
  ))
  {
    engine.ReleaseResources();
    return false;
  }

  if (!Implementation::CompileShader(
    L"shaders\\Cube.hlsl",
    "PSMain",
    "ps_5_0",
    &pixelShaderBlob
  ))
  {
    SafeRelease(vertexShaderBlob);

    engine.ReleaseResources();
    return false;
  }

  /** @brief Crea el objeto Direct3D correspondiente al vertex shader compilado. */
// ========================================================
// VERTEX SHADER
// ========================================================

  result =
    engine.device.CreateVertexShader(
      vertexShaderBlob->GetBufferPointer(),
      vertexShaderBlob->GetBufferSize(),
      nullptr,
      &engine.vertexShader
    );

  if (FAILED(result))
  {
    SafeRelease(pixelShaderBlob);
    SafeRelease(vertexShaderBlob);

    engine.ReleaseResources();
    return false;
  }

  /** @brief Crea el objeto Direct3D correspondiente al pixel shader compilado. */
// ========================================================
// PIXEL SHADER
// ========================================================

  result =
    engine.device.CreatePixelShader(
      pixelShaderBlob->GetBufferPointer(),
      pixelShaderBlob->GetBufferSize(),
      nullptr,
      &engine.pixelShader
    );

  if (FAILED(result))
  {
    SafeRelease(pixelShaderBlob);
    SafeRelease(vertexShaderBlob);

    engine.ReleaseResources();
    return false;
  }

  /** @brief Describe la organización de los atributos de cada vértice para el vertex shader. */
// ========================================================
// INPUT LAYOUT
// ========================================================

  constexpr D3D11_INPUT_ELEMENT_DESC inputElements[]
  {
      {
          "POSITION",
          0,
          DXGI_FORMAT_R32G32B32_FLOAT,
          0,
          static_cast<UINT>(
              offsetof(
                  Implementation::Vertex,
                  position
              )
          ),
          D3D11_INPUT_PER_VERTEX_DATA,
          0
      },

      {
          "COLOR",
          0,
          DXGI_FORMAT_R32G32B32A32_FLOAT,
          0,
          static_cast<UINT>(
              offsetof(
                  Implementation::Vertex,
                  color
              )
          ),
          D3D11_INPUT_PER_VERTEX_DATA,
          0
      }
  };

  result =
    engine.device.CreateInputLayout(
      inputElements,
      ARRAYSIZE(inputElements),
      vertexShaderBlob->GetBufferPointer(),
      vertexShaderBlob->GetBufferSize(),
      &engine.inputLayout
    );

  SafeRelease(pixelShaderBlob);
  SafeRelease(vertexShaderBlob);

  if (FAILED(result))
  {
    engine.ReleaseResources();
    return false;
  }

  /** @brief Define las posiciones y los colores de los ocho vértices del cubo. */
// ========================================================
// CUBE VERTICES
// ========================================================

  constexpr Implementation::Vertex vertices[]
  {
      {
          { -0.5f, -0.5f, -0.5f },
          { 1.0f, 0.0f, 0.0f, 1.0f }
      },

      {
          { -0.5f,  0.5f, -0.5f },
          { 0.0f, 1.0f, 0.0f, 1.0f }
      },

      {
          {  0.5f,  0.5f, -0.5f },
          { 0.0f, 0.0f, 1.0f, 1.0f }
      },

      {
          {  0.5f, -0.5f, -0.5f },
          { 1.0f, 1.0f, 0.0f, 1.0f }
      },

      {
          { -0.5f, -0.5f,  0.5f },
          { 0.0f, 1.0f, 1.0f, 1.0f }
      },

      {
          { -0.5f,  0.5f,  0.5f },
          { 1.0f, 0.0f, 1.0f, 1.0f }
      },

      {
          {  0.5f,  0.5f,  0.5f },
          { 1.0f, 1.0f, 1.0f, 1.0f }
      },

      {
          {  0.5f, -0.5f,  0.5f },
          { 1.0f, 0.3f, 0.0f, 1.0f }
      }
  };

  /** @brief Crea el búfer de GPU que almacena los vértices del cubo. */
// ========================================================
// VERTEX BUFFER
// ========================================================

  D3D11_BUFFER_DESC vertexBufferDescription{};

  vertexBufferDescription.ByteWidth =
    static_cast<UINT>(
      sizeof(vertices)
      );

  vertexBufferDescription.Usage =
    D3D11_USAGE_IMMUTABLE;

  vertexBufferDescription.BindFlags =
    D3D11_BIND_VERTEX_BUFFER;

  D3D11_SUBRESOURCE_DATA initialVertexData{};

  initialVertexData.pSysMem =
    vertices;

  result =
    engine.device.CreateBuffer(
      &vertexBufferDescription,
      &initialVertexData,
      &engine.vertexBuffer
    );

  if (FAILED(result))
  {
    engine.ReleaseResources();
    return false;
  }

  /** @brief Crea el búfer de índices que indica cómo formar los triángulos del cubo. */
// ========================================================
// INDEX BUFFER
// ========================================================

  constexpr std::uint16_t indices[]
  {
      0, 1, 2,
      0, 2, 3,

      5, 4, 7,
      5, 7, 6,

      4, 0, 3,
      4, 3, 7,

      1, 5, 6,
      1, 6, 2,

      4, 5, 1,
      4, 1, 0,

      3, 2, 6,
      3, 6, 7
  };

  D3D11_BUFFER_DESC indexBufferDescription{};

  indexBufferDescription.ByteWidth =
    static_cast<UINT>(
      sizeof(indices)
      );

  indexBufferDescription.Usage =
    D3D11_USAGE_IMMUTABLE;

  indexBufferDescription.BindFlags =
    D3D11_BIND_INDEX_BUFFER;

  D3D11_SUBRESOURCE_DATA indexData{};

  indexData.pSysMem =
    indices;

  result =
    engine.device.CreateBuffer(
      &indexBufferDescription,
      &indexData,
      &engine.indexBuffer
    );

  if (FAILED(result))
  {
    engine.ReleaseResources();
    return false;
  }

  /** @brief Crea el búfer constante que recibirá la matriz World-View-Projection. */
// ========================================================
// TRANSFORM BUFFER
// ========================================================

  D3D11_BUFFER_DESC transformBufferDescription{};

  transformBufferDescription.ByteWidth =
    sizeof(
      Implementation::TransformBuffer
      );

  transformBufferDescription.Usage =
    D3D11_USAGE_DEFAULT;

  transformBufferDescription.BindFlags =
    D3D11_BIND_CONSTANT_BUFFER;

  result =
    engine.device.CreateBuffer(
      &transformBufferDescription,
      nullptr,
      &engine.transformBuffer
    );

  if (FAILED(result))
  {
    engine.ReleaseResources();
    return false;
  }

  /** @brief Configura el rasterizador, incluyendo el modo de relleno y descarte de caras. */
// ========================================================
// RASTERIZER STATE
// ========================================================

  D3D11_RASTERIZER_DESC rasterizerDescription{};

  rasterizerDescription.FillMode =
    D3D11_FILL_SOLID;

  rasterizerDescription.CullMode =
    D3D11_CULL_NONE;

  rasterizerDescription.DepthClipEnable =
    TRUE;

  result =
    engine.device.CreateRasterizerState(
      &rasterizerDescription,
      &engine.rasterizerState
    );

  if (FAILED(result))
  {
    engine.ReleaseResources();
    return false;
  }

  /** @brief Guarda el instante inicial utilizado para calcular la animación del cubo. */
// ========================================================
// START TIME
// ========================================================

  engine.startTime =
    std::chrono::steady_clock::now();

  return true;
}

/**
 * @brief Actualiza la lógica interna del motor en cada iteración del bucle principal.
 *
 * @details Actualmente no contiene operaciones, pero sirve como punto de entrada
 * para incorporar futuras actualizaciones de la escena y de sus componentes.
 */
void
Engine::Update() noexcept
{}

/**
 * @brief Ejecuta el pipeline de renderizado y dibujo del cubo 3D.
 *
 * @details Comprueba los recursos gráficos, limpia los búferes, calcula las
 * transformaciones, enlaza los recursos necesarios, dibuja el cubo y presenta
 * el resultado mediante la cadena de intercambio.
 */
void
Engine::Render() noexcept
{
  if (!m_implementation)
  {
    return;
  }

  Implementation& engine =
    *m_implementation;

  if (!engine.context.getDeviceContext() ||
    !engine.swapChain ||
    !engine.renderTarget ||
    !engine.depthStencilView ||
    !engine.vertexBuffer ||
    !engine.indexBuffer ||
    !engine.transformBuffer ||
    !engine.inputLayout ||
    !engine.vertexShader ||
    !engine.pixelShader ||
    !engine.rasterizerState)
  {
    return;
  }

  /** @brief Define el color con el que se limpia la pantalla antes de dibujar. */
// ========================================================
// CLEAR COLOR
// ========================================================

  constexpr float clearColor[]
  {
      0.03f,
      0.04f,
      0.08f,
      1.0f
  };

  /** @brief Establece el destino de renderizado y limpia los búferes de color y profundidad. */
// ========================================================
// RENDER TARGET + DEPTH
// ========================================================

  engine.context.OMSetRenderTargets(
    1,
    &engine.renderTarget,
    engine.depthStencilView
  );

  engine.context.ClearRenderTargetView(
    engine.renderTarget,
    clearColor
  );

  engine.context.ClearDepthStencilView(
    engine.depthStencilView,
    D3D11_CLEAR_DEPTH |
    D3D11_CLEAR_STENCIL,
    1.0f,
    0
  );

  /** @brief Calcula el tiempo transcurrido desde la inicialización del motor. */
// ========================================================
// TIME
// ========================================================

  const auto currentTime =
    std::chrono::steady_clock::now();

  const float elapsedSeconds =
    std::chrono::duration<float>(
      currentTime -
      engine.startTime
    ).count();

  /** @brief Calcula la transformación del objeto en el espacio del mundo. */
// ========================================================
// WORLD
// ========================================================

  using namespace DirectX;

  const XMMATRIX world =
    XMMatrixRotationX(
      elapsedSeconds * 0.4f
    ) *
    XMMatrixRotationY(
      elapsedSeconds * 0.8f
    );

  /** @brief Define la posición, el objetivo y la orientación de la cámara. */
// ========================================================
// CAMERA
// ========================================================

  const XMVECTOR cameraPosition =
    XMVectorSet(
      0.0f,
      1.5f,
      -5.0f,
      1.0f
    );

  const XMVECTOR cameraTarget =
    XMVectorSet(
      0.0f,
      0.0f,
      0.0f,
      1.0f
    );

  const XMVECTOR cameraUp =
    XMVectorSet(
      0.0f,
      1.0f,
      0.0f,
      0.0f
    );

  const XMMATRIX view =
    XMMatrixLookAtLH(
      cameraPosition,
      cameraTarget,
      cameraUp
    );

  /** @brief Calcula la matriz de proyección en perspectiva según la proporción de la ventana. */
// ========================================================
// PROJECTION
// ========================================================

  const float aspectRatio =
    static_cast<float>(
      engine.width
      ) /
    static_cast<float>(
      engine.height
      );

  const XMMATRIX projection =
    XMMatrixPerspectiveFovLH(
      XM_PIDIV4,
      aspectRatio,
      0.1f,
      100.0f
    );

  /** @brief Combina las matrices de mundo, vista y proyección para el shader. */
// ========================================================
// TRANSFORM
// ========================================================

  Implementation::TransformBuffer transform{};

  XMStoreFloat4x4(
    &transform.worldViewProjection,

    XMMatrixTranspose(
      world *
      view *
      projection
    )
  );

  /** @brief Actualiza el búfer constante con la matriz de transformación calculada. */
// ========================================================
// UPDATE CONSTANT BUFFER
// ========================================================

  engine.context.UpdateSubresource(
    engine.transformBuffer,
    0,
    nullptr,
    &transform,
    0,
    0
  );

  /** @brief Enlaza el búfer de vértices al ensamblador de entrada. */
// ========================================================
// VERTEX BUFFER
// ========================================================

  constexpr UINT stride =
    sizeof(Implementation::Vertex);

  constexpr UINT offset = 0;

  engine.context.IASetVertexBuffers(
    0,
    1,
    &engine.vertexBuffer,
    &stride,
    &offset
  );

  /** @brief Enlaza el búfer de índices y especifica el formato de sus elementos. */
// ========================================================
// INDEX BUFFER
// ========================================================

  engine.context.IASetIndexBuffer(
    engine.indexBuffer,
    DXGI_FORMAT_R16_UINT,
    0
  );

  /** @brief Establece la descripción de los atributos de los vértices. */
// ========================================================
// INPUT LAYOUT
// ========================================================

  engine.context.IASetInputLayout(
    engine.inputLayout
  );

  /** @brief Activa el estado de rasterización configurado durante la inicialización. */
// ========================================================
// RASTERIZER
// ========================================================

  engine.context.RSSetState(
    engine.rasterizerState
  );

  /** @brief Indica que la geometría se procesa como una lista de triángulos. */
// ========================================================
// TOPOLOGY
// ========================================================

  engine.context.IASetPrimitiveTopology(
    D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
  );

  /** @brief Establece el vertex shader que procesará los vértices. */
// ========================================================
// VERTEX SHADER
// ========================================================

  engine.context.VSSetShader(
    engine.vertexShader
  );

  /** @brief Enlaza el búfer de transformaciones al vertex shader. */
// ========================================================
// TRANSFORM BUFFER
// ========================================================

  engine.context.VSSetConstantBuffers(
    0,
    1,
    &engine.transformBuffer
  );

  /** @brief Establece el pixel shader que calculará el color de los píxeles. */
// ========================================================
// PIXEL SHADER
// ========================================================

  engine.context.PSSetShader(
    engine.pixelShader
  );

  /** @brief Enlaza la textura al pipeline gráfico para utilizarla al dibujar. */
// ========================================================
// TEXTURE
// ========================================================

  engine.texture.render(
    engine.context,
    0,
    1
  );

  /** @brief Ejecuta el dibujo del cubo utilizando sus 36 índices. */
// ========================================================
// DRAW CUBE
// ========================================================

  engine.context.DrawIndexed(
    36,
    0,
    0
  );

  /** @brief Presenta en pantalla el fotograma que se acaba de renderizar. */
// ========================================================
// PRESENT
// ========================================================

  engine.swapChain->Present(
    1,
    0
  );
}

/**
 * @brief Finaliza el motor y libera todos los recursos gráficos y del sistema.
 *
 * @details Comprueba que la implementación exista y llama a ReleaseResources()
 * para liberar los objetos de DirectX 11 y restablecer los datos de la ventana.
 */
void
Engine::Shutdown() noexcept
{
  if (m_implementation)
  {
    m_implementation->ReleaseResources();
  }
}