#pragma once
#include"Engine/Prerequisitos.h"

class DeviceContext;
class Device;
class Texture;

class DepthStencilView {
public:
	DepthStencilView() = default;
	~DepthStencilView() = default;

HRESULT
init(Device& device,
	Texture& depthStencil,
	DXGI_FORMAT format);
	
	HRESULT 
		init(Device& device, 
			Texture& depthStencil,
			DXGI_FORMAT format, 
			D3D11_DSV_DIMENSION ViewDimension);

	void update() {};

	void
		render(DeviceContext& devicecontext);

	void
		destroy();

	ID3D11DepthStencilView*
		getDepthStencilView() const {
		return m_depthStencilView;
	}

private:
	ID3D11DepthStencilView* m_depthStencilView = nullptr;
};