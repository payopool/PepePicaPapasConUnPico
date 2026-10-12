#pragma once
#include"Engine/Prerequisitos.h"
class
	DeviceContext {
public:
	DeviceContext() = default;
	~DeviceContext();

	ID3D11DeviceContext*
		getDeviceContext() const noexcept
	{
		return m_deviceContext;
	}
	ID3D11DeviceContext**
		getDeviceContext() noexcept
	{
		return &m_deviceContext;
	}

	void
		RSSetViewports(unsigned int NumViewports,
			const D3D11_VIEWPORT* pViewports);

	void
		VSSetShaders(ID3D11VertexShader* pVertexShader,
			ID3D11ClassInstance* const* ppClassInstances = nullptr,
			unsigned int NumClassInstances = 0)noexcept;

	void
		PSSetShaders(ID3D11PixelShader* pPixelShader,
			ID3D11ClassInstance* const* ppClassInstances = nullptr,
			unsigned int NumClassInstances = 0)noexcept;

	void
		VSSetConstantBuffers(unsigned int StartSlot,
			unsigned int NumBuffers,
			ID3D11Buffer* const* ppConstantBuffers)noexcept;

	void
		UpdateSubresourses(ID3D11Resource* pDstResource,
			unsigned int DstSubresource,
			const D3D11_BOX* pDstBox,
			const void* pSrcData,
			unsigned int SrcRowPitch,
			unsigned int SrcDepthPitch);

	void
		DrawIndexed(unsigned int IndexCount,
			unsigned int StartIndexLocation,
			int BaseVertexLocation);



private:
	ID3D11DeviceContext* m_deviceContext = nullptr;
};