#pragma once
#include <d3d9.h>
#include <d3dx9.h>

class GraphicDevice {
public:
	explicit GraphicDevice(HWND hWnd);
	~GraphicDevice();

	HRESULT beginScene();
	HRESULT endScene();
	HRESULT present();
	LPDIRECT3DDEVICE9 getDevice() { return m_d3dDevice; }
private:
	LPDIRECT3D9 m_d3d;
	LPDIRECT3DDEVICE9 m_d3dDevice;
};
