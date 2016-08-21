#include "Texture.h"

#include <vector>

namespace {
	struct TextureVertex {
		D3DXVECTOR3 p;
		float rhw;
		DWORD color;
		D3DXVECTOR2 t;
	};
}

Texture::Texture(std::string filePath, LPDIRECT3DDEVICE9 d3dDevice) :
m_d3dTex(NULL), m_d3dDevice(d3dDevice)
{
	HRESULT hr = D3DXCreateTextureFromFile(m_d3dDevice, filePath.c_str(), &m_d3dTex);
	if (FAILED(hr)) {
		throw std::runtime_error("Failed load " + filePath);
	}

	//テクスチャのサイズを取得
	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFile(filePath.c_str(), &info);
	m_size.x = static_cast<float>(info.Width);
	m_size.y = static_cast<float>(info.Height);
}

Texture::~Texture() {
	if (m_d3dTex)
		m_d3dTex->Release();
}

void Texture::draw(D3DXVECTOR2 pos, float rad, float scale, const D3DXCOLOR& color, bool mirror) {
	draw({ 0.0f, 0.0f, 1.0f, 1.0f }, pos, rad, scale, color, mirror);
}

void Texture::drawFrame(int col, int row, int index, D3DXVECTOR2 pos, float rad, float scale, const D3DXCOLOR& color, bool mirror) {
	RectF uvRect(0.0f, 0.0f, 1.0f / col, 1.0f / row);
	index %= (col * row);
	uvRect.x = uvRect.w * (index % col);
	uvRect.y = uvRect.h * (index / col);
	draw(uvRect, pos, rad, scale, color, mirror);
}

void Texture::draw(RectF uvRect, D3DXVECTOR2 pos, float rad, float scale, const D3DXCOLOR& color, bool mirror) {
	auto texSize = D3DXVECTOR2(m_size.x * uvRect.w, m_size.y * uvRect.h);
	std::vector<TextureVertex> vtx {
		{ { -texSize.x/2, -texSize.y/2, 0.0f }, 1.0f, color, { uvRect.x, uvRect.y } },
		{ { texSize.x/2, -texSize.y/2, 0.0f }, 1.0f, color, { uvRect.x + uvRect.w, uvRect.y } },
		{ { -texSize.x/2, texSize.y/2, 0.0f }, 1.0f, color, { uvRect.x, uvRect.y + uvRect.h } },
		{ { texSize.x/2, texSize.y/2, 0.0f }, 1.0f, color, { uvRect.x + uvRect.w, uvRect.y + uvRect.h } },
	};
	if (mirror) {
		std::swap(vtx[0].t, vtx[1].t);
		std::swap(vtx[2].t, vtx[3].t);
	}

	//アフィン変換
	for (int i = 0; i < 4; i++) {
		auto tPos = vtx[i].p;
		vtx[i].p.x = pos.x + scale * (tPos.x * std::cos(rad) - tPos.y * std::sin(rad));
		vtx[i].p.y = pos.y + scale * (tPos.x * std::sin(rad) + tPos.y * std::cos(rad));
	}

	m_d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	m_d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	m_d3dDevice->SetTexture(0, m_d3dTex);
	m_d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtx.data(), sizeof(TextureVertex));
	m_d3dDevice->SetTexture(0, NULL);
}

TextureManager::TextureManager(LPDIRECT3DDEVICE9 d3dDevice) :
m_d3dDevice(d3dDevice)
{}

void TextureManager::load(std::string filePath, std::string alias) {
	m_textures[alias] = std::make_shared<Texture>(filePath, m_d3dDevice);
}
