#pragma once
#include <d3dx9.h>

struct Color {
	Color(float r_ = 0.0f, float g_ = 0.0f, float b_ = 0.0f, float a_ = 0.0f) : a(a_), r(r_), g(g_), b(b_) {}
	float a, r, g, b;

	D3DCOLOR toD3Dcolor() {
		DWORD a_ = static_cast<DWORD>(a * 255) << 24;
		DWORD r_ = static_cast<DWORD>(r * 255) << 16;
		DWORD g_ = static_cast<DWORD>(g * 255) << 8;
		DWORD b_ = static_cast<DWORD>(b * 255);
		return a_ | r_ | g_ | b_;
	}
};

struct HSV {
	HSV(float h_, float s_, float v_) : h(h_), s(s_), v(v_) {}
	float h;// ... 0Åã==0.0f, 360Åã==1.0f ...
	float s, v;// [0.0f, 1.0f]

	Color toColor(float alpha) {
		const float h_ = h - floor(h);
		const float s_ = s;
		const float v_ = v;
		const float hueF = h_ * 6.0f;
		const int hueI = static_cast<int>(hueF);
		const float fr = hueF - hueI;
		const float m = v_ * (1.0f - s_);
		const float n = v_ * (1.0f - s_*fr);
		const float p = v_ * (1.0f - s_*(1.0f - fr));

		Color color;
		color.a = alpha;

		switch (hueI) {
			case 0: color.r = v; color.g = p; color.b = m; break;
			case 1: color.r = n; color.g = v; color.b = m; break;
			case 2: color.r = m; color.g = v; color.b = p; break;
			case 3: color.r = m; color.g = n; color.b = v; break;
			case 4: color.r = p; color.g = m; color.b = v; break;
			default: color.r = v; color.g = m; color.b = n; break;
		}

		return color;
	}
};
