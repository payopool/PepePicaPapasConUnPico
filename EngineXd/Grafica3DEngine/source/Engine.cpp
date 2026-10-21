#include <Engine/Engine.h>
#include <Rendering/Device.h>
#include <Rendering/DeviceContext.h>
#include <Rendering/Texture2D.h>
#include <Engine/Prerequisitos.h>

struct Engine::Implementation
{
  struct Vertex
  {
    float position[3];
    float color[4];
  };

  struct alignas(16) TransformBuffer
  {
    DirectX::XMFLOAT4X4 worldViewProjection;
  };

  HWND window = nullptr;

  std::uint32_t width = 0;
  std::uint32_t height = 0;

  Device device;
  DeviceContext context;
  Texture2D texture;

  IDXGISwapChain* swapChain = nullptr;

  ID3D11RenderTargetView* renderTarget = nullptr;

  ID3D11Texture2D* depthStencilBuffer = nullptr;
  ID3D11DepthStencilView* depthStencilView = nullptr;

  ID3D11Buffer* vertexBuffer = nullptr;
  ID3D11Buffer* indexBuffer = nullptr;
  ID3D11Buffer* transformBuffer = nullptr;

  ID3D11RasterizerState* rasterizerState = nullptr;

  ID3D11VertexShader* vertexShader = nullptr;
  ID3D11PixelShader* pixelShader = nullptr;
  ID3D11InputLayout* inputLayout = nullptr;

  std::chrono::steady_clock::time_point startTime{};

  static bool CompileShader(
    const wchar_t* filename,
    const char* entryPoint,
    const char* shaderModel,
    ID3DBlob** shaderBlob) noexcept
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

  void ReleaseResources() noexcept
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


Engine::Engine() noexcept
  : m_implementation(
    new (std::nothrow) Implementation{}
  )
{}


Engine::~Engine() noexcept
{
  Shutdown();

  delete m_implementation;
  m_implementation = nullptr;
}


bool Engine::Initialize(
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


  // Si falla la GPU física, utiliza WARP.

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


  // ========================================================
  // START TIME
  // ========================================================

  engine.startTime =
    std::chrono::steady_clock::now();


  return true;
}


void
Engine::Update() noexcept
{}


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


  // ========================================================
  // INDEX BUFFER
  // ========================================================

  engine.context.IASetIndexBuffer(
    engine.indexBuffer,
    DXGI_FORMAT_R16_UINT,
    0
  );


  // ========================================================
  // INPUT LAYOUT
  // ========================================================

  engine.context.IASetInputLayout(
    engine.inputLayout
  );


  // ========================================================
  // RASTERIZER
  // ========================================================

  engine.context.RSSetState(
    engine.rasterizerState
  );


  // ========================================================
  // TOPOLOGY
  // ========================================================

  engine.context.IASetPrimitiveTopology(
    D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
  );


  // ========================================================
  // VERTEX SHADER
  // ========================================================

  engine.context.VSSetShader(
    engine.vertexShader
  );


  // ========================================================
  // TRANSFORM BUFFER
  // ========================================================

  engine.context.VSSetConstantBuffers(
    0,
    1,
    &engine.transformBuffer
  );


  // ========================================================
  // PIXEL SHADER
  // ========================================================

  engine.context.PSSetShader(
    engine.pixelShader
  );


  // ========================================================
  // TEXTURE
  // ========================================================

  engine.texture.render(
    engine.context,
    0,
    1
  );


  // ========================================================
  // DRAW CUBE
  // ========================================================

  engine.context.DrawIndexed(
    36,
    0,
    0
  );


  // ========================================================
  // PRESENT
  // ========================================================

  engine.swapChain->Present(
    1,
    0
  );
}


void
Engine::Shutdown() noexcept
{
  if (m_implementation)
  {
    m_implementation->ReleaseResources();
  }
}