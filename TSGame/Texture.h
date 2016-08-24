#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <d3d9.h>
#include <d3dx9.h>
#include "Rect.h"
#include "Vector2.h"
#include "Point.h"

class Texture {
public:
	Texture(std::string filePath, LPDIRECT3DDEVICE9 d3dDevice);
	~Texture();
	void draw(Vector2 pos, float rad = 0.0f, float scale = 1.0f, const D3DXCOLOR& color = 0xFFFFFFFF, bool mirror = false);
	void draw(RectF uvRect, Vector2 pos, float rad = 0.0f, float scale = 1.0f, const D3DXCOLOR& color = 0xFFFFFFFF, bool mirror = false);
	void drawFrame(int col, int row, int index, Vector2 pos, float rad = 0.0f, float scale = 1.0f, const D3DXCOLOR& color = 0xFFFFFFFF, bool mirror = false);
private:
	LPDIRECT3DDEVICE9 m_d3dDevice;
	LPDIRECT3DTEXTURE9 m_d3dTex;
	Point m_size;
};

class TextureManager {
public:
	TextureManager(LPDIRECT3DDEVICE9 d3dDevice);
	void load(std::string filePath, std::string alias);
	void clear() { m_textures.clear(); }
	std::shared_ptr<Texture> getTexture(std::string alias) { return m_textures[alias]; }
private:
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
	LPDIRECT3DDEVICE9 m_d3dDevice;
};
