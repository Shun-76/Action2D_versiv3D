# include <Siv3D.hpp>
#include "Sprite.h"

void DrawSpriteQuad(
	AssetNameView texture,
	double x, double y, double w, double h,
	double r, double g, double b, double a,
	double u, double v, double tw, double th,
	bool reverse, double rad)
{
	if (!reverse)
	{
		TextureAsset(texture)(TextureAsset(texture).width() * u, TextureAsset(texture).height() * v, TextureAsset(texture).width() * tw, TextureAsset(texture).height() * th).
			resized(w, h).
			rotated(rad).
			drawAt(Scene::Center().x + x, Scene::Center().y + y, { r, g, b, a });
	}
	else
	{
		TextureAsset(texture)(TextureAsset(texture).width() * u, TextureAsset(texture).height() * v, TextureAsset(texture).width() * tw, TextureAsset(texture).height() * th).
			mirrored().
			resized(w, h).
			rotated(rad).
			drawAt(Scene::Center().x + x, Scene::Center().y + y, { r, g, b, a });
	}
}
