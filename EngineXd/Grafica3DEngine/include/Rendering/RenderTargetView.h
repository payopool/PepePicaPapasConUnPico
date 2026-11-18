#pragma once
#include"Engine/Prerequisitos.h"
class DeviceContext;
class Device;
class Texture;
class DepthStencilView;

	class
	RenderTargetView {
	public:
		RenderTargetView() = default;
		~RenderTargetView() = default;
		HRESULT
			init(Device& device, Texture& backBuffer, DXGI_FORMAT );

		HRESULT
			init(Device& device,
				Texture& backinTex, 
				DXGI_FORMAT format, 
				D3D11_RTV_DIMENSION ViewDimension);

		void update();

		void
			render(DeviceContext& deviceContext, 
				DepthStencilView depthStencilView,
				unsigned int numViewa, 
				const float ClearColor[4]);

		void
			render(DeviceContext& deviceContext,
				unsigned int numViewa);

		void
			destroy();

		ID3D11RenderTargetView*
			getRenderTargetView() const {
			return m_renderTargetView;
		}

	private:
		ID3D11RenderTargetView* m_renderTargetView = nullptr;
};
