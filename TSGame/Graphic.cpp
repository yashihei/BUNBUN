#include "Graphic.h"

#include <vector>
#include <stdexcept>
#include "Texture.h"

GraphicDevice::GraphicDevice(HWND hWnd, bool fullScreen) : m_d3d(NULL), m_d3dDevice(NULL) {
	if ((m_d3d = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) {
		throw std::runtime_error("Error initialize Direct3D");
	}

	D3DPRESENT_PARAMETERS d3dpp = {};
	//windowの場合、width, heightは自動で設定される
	if (fullScreen) {
		d3dpp.BackBufferWidth = 640;
		d3dpp.BackBufferHeight = 480;
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	} else {
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	}
	d3dpp.Windowed = !fullScreen;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//垂直同期無視
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(m_d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_d3dDevice)))
	{
		throw std::runtime_error("Error creating Direct3D device");
	}

	//zバッファ有効
	m_d3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	//αブレンドに関する設定
	m_d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//ライト無効
	m_d3dDevice->LightEnable(0, FALSE);
}

GraphicDevice::~GraphicDevice() {
	if (m_d3dDevice)
		m_d3dDevice->Release();
	if (m_d3d)
		m_d3d->Release();
}

HRESULT GraphicDevice::beginScene() {
	m_d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(10, 20, 30), 1.0f, 0);
	return m_d3dDevice->BeginScene();
}

HRESULT GraphicDevice::endScene() {
	return m_d3dDevice->EndScene();
}

HRESULT GraphicDevice::present() {
	return m_d3dDevice->Present(NULL, NULL, NULL, NULL);
}
