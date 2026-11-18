#pragma once
#include"Engine/Prerequisitos.h"

class DeviceContext;
class Device;

class
	RasterizerState {
public:
	RasterizerState() = default;
	~RasterizerState() = default;

	HRESULT
	init(Device& device);

	HRESULT
		init(Device device,
			D3D11_FILL_MODE fill, 
			D3D11_CULL_MODE cull, 
			bool frontCCW, 
			bool depthClip);

	void update();

	void
		render(DeviceContext& devicecontext);

	void
		destroy();

	ID3D11RasterizerState*
		getRasterizerState() const {
		return m_rasterizerState;
	}

private:
	ID3D11RasterizerState* m_rasterizerState = nullptr;
};