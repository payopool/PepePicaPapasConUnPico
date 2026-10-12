#include"Rendering/DeviceContext.h"

ID3D11DeviceContext&
DeviceContext::requireDeviceContext() const noexcept
{
	if (!DeviceContext) 
	{
		ERROR("DeviceContext", "requireDeviceContext", "El contexto del dispositivo no está inicializado.");

	}
	return *m_deviceContext;
}

void
DeviceContext::RSSetViewports(
	unsigned int NumViewports,
	const D3D11_VIEWPORT* pViewports)noexcept
{
	if (!pViewports || NumViewports == 0)
	{
		ERROR("DeviceContext", "RSSetViewports", "Parámetros inválidos: pViewports es nulo o NumViewports es cero.");
		return;
	}
	requireDeviceContext().RSSetViewports(
		NumViewports,
		pViewports
	);
}