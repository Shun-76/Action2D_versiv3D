#pragma once
#ifndef _SPRITE_H_
#define _SPRITE_H_
// 座標(x, y)、スプライトのサイズ(w, h)
// 色(r, g, b, a)
// テクスチャ座標
void DrawSpriteQuad(
	AssetNameView texture,
	double pos_x, double pos_y, double size_w, double size_h,
	double r, double g, double b, double a,
	double u, double v, double tw, double th,
	bool reverse = false, double rad = 0);

#endif
