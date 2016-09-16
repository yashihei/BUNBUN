#pragma once
#include <d3dx9.h>
#include <vector>
#include <cmath>
#include "Vector2.h"

namespace Shape {
	struct ShapeVertex {
		D3DXVECTOR3 p;
		float rhw;
		DWORD color;
	};
	//D3DXLINEAd‚¢‚Ì‚Å‚ ‚Ü‚èŽg‚í‚È‚¢‚±‚Æ
	inline void drawLine(LPDIRECT3DDEVICE9 d3dDevice, Vector2 start, Vector2 end, float width = 1, D3DXCOLOR color = 0xFFFFFFFF) {
		static LPD3DXLINE line = nullptr;
		if (!line)
			D3DXCreateLine(d3dDevice, &line);
		D3DXVECTOR2 vec[] = { D3DXVECTOR2(start.x, start.y), D3DXVECTOR2(end.x, end.y) };
		line->SetWidth(width);
		line->Begin();
		line->Draw(vec, 2, color);
		line->End();
	}
	inline void drawCircle(LPDIRECT3DDEVICE9 d3dDevice, Vector2 pos, float radius, D3DXCOLOR color = 0xFFFFFFFF) {
		const int splitNum = radius > 100 ? 64 : 32;
		std::vector<ShapeVertex> vtx(splitNum, { { 0, 0, 0 }, 1, color });
		for (int i = 0; i < splitNum; i++) {
			float radian = D3DX_PI * 2 * i / splitNum;
			vtx[i].p.x = pos.x + radius * std::cos(radian);
			vtx[i].p.y = pos.y + radius * std::sin(radian);
		}
		d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, splitNum - 2, vtx.data(), sizeof(ShapeVertex));
	}
	inline void drawRectangle(LPDIRECT3DDEVICE9 d3dDevice, Vector2 topLeft, Vector2 bottomRight, D3DXCOLOR color = 0xFFFFFFFF) {
		std::vector<ShapeVertex> vtx {
			{ { topLeft.x, topLeft.y, 0 }, 1, color },
			{ { bottomRight.x, topLeft.y, 0 }, 1, color },
			{ { topLeft.x, bottomRight.y, 0 }, 1, color },
			{ { bottomRight.x, bottomRight.y, 0 }, 1, color },
		};
		d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtx.data(), sizeof(ShapeVertex));
	}
	inline void drawRectangle(LPDIRECT3DDEVICE9 d3dDevice, Vector2 pos, float width, float height, float rad, D3DXCOLOR color = 0xFFFFFFFF) {
		std::vector<ShapeVertex> vtx {
			{ { -width / 2, -height / 2, 0 }, 1, color },
			{ { width / 2, -height / 2, 0 }, 1, color },
			{ { -width / 2, height / 2, 0 }, 1, color },
			{ { width / 2, height / 2, 0 }, 1, color },
		};
		for (int i = 0; i < 4; i++) {
			auto tPos = vtx[i].p;
			vtx[i].p.x = pos.x + tPos.x * std::cos(rad) - tPos.y * std::sin(rad);
			vtx[i].p.y = pos.y + tPos.x * std::sin(rad) + tPos.y * std::cos(rad);
		}
		d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtx.data(), sizeof(ShapeVertex));
	}
	inline void drawSquare(LPDIRECT3DDEVICE9 d3dDevice, Vector2 pos, float size, float rad, D3DXCOLOR color = 0xFFFFFFFF) {
		drawRectangle(d3dDevice, pos, size, size, rad, color);
	}
	inline void drawNgon(LPDIRECT3DDEVICE9 d3dDevice, Vector2 pos, int num, float radius, float radian, D3DXCOLOR color = 0xFFFFFFFF) {
		std::vector<ShapeVertex> vtx(num, { { 0, 0, 0 }, 1, color });
		for (int i = 0; i < num; i++) {
			float tRadian = D3DX_PI * 2 * i / num;
			vtx[i].p.x = pos.x + radius * std::cos(tRadian + radian);
			vtx[i].p.y = pos.y + radius * std::sin(tRadian + radian);
		}
		d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, num - 2, vtx.data(), sizeof(ShapeVertex));
	}
};
