#pragma once
#include"Engine/Prerequisitos.h"
#include"Engine/MeshComponent.h"

class Device;
class DeviceContext;

class Buffer {
	public:
		Buffer();
		
		~Buffer();

		HRESULT
			init(
				Device& device,
				const MeshComponent& mesh, 
				unsigned int bindFlag);

		HRESULT
			init(Device& device, unsigned int ByteWith);
		
		void
			Update(DeviceContext& deviceContext, 
				ID3D11Resource* pDstResource,
				unsigned int DstSubresource,
				const void* pSrcData,
				unsigned int SrcRowPitch,
				unsigned int SrcDepthPitch);

		void 
			render(DeviceContext& deviceContext, 
				unsigned int StartSlot,
				unsigned int NumBuffers,
				bool setPixelShader = false,
				DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN);
				
		void
			destroy();
		HRESULT
			createBuffer(Device& device,
				D3D11_BUFFER_DESC& desc,
				D3D11_SUBRESOURCE_DATA* initData);
public:
	ID3D11Buffer* m_buffer = nullptr;
	unsigned int m_numVertex;
	unsigned int m_numIndex;
	unsigned int m_stride = 0;
	unsigned int m_offset = 0;
	unsigned int m_bindFlag = 0;
};