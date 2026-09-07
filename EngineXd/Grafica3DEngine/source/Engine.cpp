#include <Engine/Engine.h>

#include <d3d11.h>
#include <Windows.h>
#include <d3dcompiler.h>
#include <cstddef>
#include <cstdint>
#include <new>
// MACROS
#define SAFE_RELEASE(x) if(x != nullptr) x->Release(); x = nullptr;

#define MESSAGE( classObj, method, state )   \
{                                            \
   std::wostringstream os_;                  \
   os_ << classObj << "::" << method << " : " << "[CREATION OF RESOURCE " << ": " << state << "] \n"; \
   OutputDebugStringW( os_.str().c_str() );  \
}

#define ERROR(classObj, method, errorMSG)                     \
{                                                             \
    try {                                                     \
        std::wostringstream os_;                              \
        os_ << L"ERROR : " << classObj << L"::" << method     \
            << L" : " << errorMSG << L"\n";                   \
        OutputDebugStringW(os_.str().c_str());                \
    } catch (...) {                                           \
        OutputDebugStringW(L"Failed to log error message.\n");\
    }                                                         \
}
template<typename T>
void SafeRelease(T * &object) noexcept
{
	if (object != nullptr)
	{
		object->Release();
		object = nullptr;
	}
}
struct Engine::Implementation
{
  struct Vertex {
    float position[3];
    float color[4];
};
  HWND window = nullptr;
	std::uint32_t width = 0;
	std::uint32_t height = 0;

	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;
	IDXGISwapChain* swapChain = nullptr;
	ID3D11RenderTargetView* renderTarget = nullptr;

	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D11InputLayout* inputLayout = nullptr;
	ID3D11Buffer* vertexBuffer = nullptr;
};
static bool compileShaders(const wchar_t* filename, const char* entryPoint,
	const char* shaderModel, ID3DBlob** shaderblob)noexcept {
	if (!filename || !entryPoint || !shaderModel || !shaderblob) {
		return false;
	}
	*shaderblob = nullptr;
	UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	compileFlags |= D3DCOMPILE_DEBUG;
	compileFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif
	ID3DBlob* errorBlob = nullptr;
	const HRESULT result = D3DCompileFromFile(
		filename, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint,
		shaderModel,
		compileFlags,
		0,
		shaderblob,
		&errorBlob
	);
	if (errorBlob) {
		OutputDebugStringA(static_cast<const char*>(errorBlob->GetBufferPointer()));
		SAFE_RELEASE(errorBlob);

	}
	if (FAILED(result)) {
		SafeRelease(*shaderblob);
		return false;
	}
}
