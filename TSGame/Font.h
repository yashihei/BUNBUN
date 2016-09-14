#pragma once
#include <d3dx9.h>
#include <string>
#include "Point.h"

class Font {
public:
	Font(int size, std::string fontName, bool bold, LPDIRECT3DDEVICE9 device) {
		HRESULT hr = D3DXCreateFont(device,
			size, 0, bold ? FW_BOLD : FW_NORMAL, 0, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
			fontName.c_str(),
			&m_font);
		if (FAILED(hr))
			throw std::runtime_error("Error createfont");
	}
	~Font() {
		if (m_font)
			m_font->Release();
	}
	void drawStr(std::string str, Point pos, D3DCOLOR color = 0xFFFFFFFF) {
		RECT rect = { pos.x, pos.y, 0, 0 };
		m_font->DrawText(NULL, str.c_str(), -1, &rect, DT_NOCLIP, color);
	}
	void drawStrToCenter(std::string str, RECT rect, D3DCOLOR color = 0xFFFFFFFF) {
		m_font->DrawText(NULL, str.c_str(), -1, &rect, DT_CENTER, color);
	}

	static void addFont(std::string filePath) {
		if (AddFontResourceEx(filePath.c_str(), FR_PRIVATE, NULL) == 0) {
			throw std::runtime_error("Failed load " + filePath);
		}
	}
private:
	ID3DXFont* m_font;
};
