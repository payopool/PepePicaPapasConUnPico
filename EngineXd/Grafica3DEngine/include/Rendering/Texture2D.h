#pragma once
#include"Engine/Prerequisitos.h"
class Device;
class DeviceContext;
class Texture2D {
public:
	Texture2D() noexcept = default;
	~Texture2D() noexcept;

	Texture2D(const Texture2D&) = delete;
	Texture2D& operator=(const Texture2D&) = delete;

	Texture2D(Texture2D&&) = delete;
	Texture2D& operator=(Texture2D&&) = delete;

	HRESULT
		init(Device& device, const std::string& textureName,
			ExtensionType extencionType);
	HRESULT
		init(Device& device,
			unsigned int width,
			unsigned int height,
			DXGI_FORMAT Format,
			unsigned int BindFlags,
			unsigned int sampleCount = 1,
			unsigned int qualityLevels = 0);

	HRESULT
		init(Device& device, Texture2D& textureRef, DXGI_FORMAT Format);
	void
		update();
	void
		render(DeviceContext& deviceContext, unsigned int  StartSlot, unsigned int NumViews);
	void destroy() noexcept;

	HRESULT
		CreateCubeMap(Device& device, 
		DeviceContext& deviceContext,
			const std::array<std::string, 6>& FacePaths,
			bool generateMipMaps);

	ID3D11ShaderResourceView*
		CreateCubemapFaceSRV(ID3D11Device* device,
			ID3D11Texture2D* cubeMapTexture,
			DXGI_FORMAT format,
			unsigned int faceIndex,
			unsigned int mipLevel=1 );
public:
	ID3D11Texture2D* m_texture = nullptr;
	ID3D11ShaderResourceView* m_TextureFromImg = nullptr;
	std::string m_textureName;
};