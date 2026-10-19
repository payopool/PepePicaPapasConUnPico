#include"Rendering/Texture2D.h"
#include "Rendering/Device.h"
#include"Rendering/DeviceContext.h"

HRESULT
Texture2D::init(Device& device, const std::string& textureName,
	ExtensionType extencionType)
{
	if (!device.getDevice()) {
		ERROR("Texture2D", "init", "Device is not initialized."); 
		return E_POINTER;
}
	if (textureName.empty()) {
		ERROR("Texture2D", "init", "Texture name is empty.");
		return E_INVALIDARG;
	}

	HRESULT hr = S_OK;

	switch (extencionType) 
	{
	case DDS:
		break;
	case PNG: {
		m_textureName = textureName+".png";
		hr = InitTextureFromImage(device, m_textureName,*this);
		break;
	}
	case JPG:{
		m_textureName = textureName + ".jpg";
		hr = InitTextureFromImage(device, m_textureName, *this);
		break;
	}
	default:
		ERROR("Texture2D", "init", "Unsupported extension type.");
		return E_INVALIDARG;
	}
	return hr;
}