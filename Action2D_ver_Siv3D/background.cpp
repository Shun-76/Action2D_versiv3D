# include <Siv3D.hpp>
#include "Main.h"
#include "Sprite.h"
#include "background.h"
#include "camera.h"

static AssetNameView
g_Sky = U"backg_sky",
g_mountain = U"backg_mountain",
g_mountain_mid = U"backg_mountain_mid",
g_hill = U"backg_hill";

void InitBG(void)
{
	TextureAsset::Register(g_Sky, U"Action/Texture/sky.tga");
	TextureAsset::Register(g_mountain, U"Action/Texture/mountain1.tga");
	TextureAsset::Register(g_mountain_mid, U"Action/Texture/mountain_mid.tga");
	TextureAsset::Register(g_hill, U"Action/Texture/hill.tga");
}

void UninitBG(void)
{
	TextureAsset::Release(g_Sky);
	TextureAsset::Release(g_mountain);
	TextureAsset::Release(g_mountain_mid);
	TextureAsset::Release(g_hill);
}

void UpdateBG(void)
{

}

void DrawBG(void)
{
	CAMERA *camera = GetCamera();
	double pos = camera->Position.x / SCREEN_WIDTH;

	DrawSpriteQuad(g_Sky,
		0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT,
		0.9f, 0.9f, 0.9f, 1.f,
		0.f, 0.f, 1.0f, 1.0f);

	double pos_mountain = -(pos * 0.2- (int)(pos * 0.2)) * SCREEN_WIDTH;
	DrawSpriteQuad(g_mountain,
		pos_mountain, -camera->Position.y * 0.2, SCREEN_WIDTH, SCREEN_HEIGHT,
		0.9f, 0.9f, 0.9f, 1.f,
		0.f, 0.f, 1.0f, 1.0f);
	DrawSpriteQuad(g_mountain,
		pos_mountain + SCREEN_WIDTH, -camera->Position.y * 0.2, SCREEN_WIDTH, SCREEN_HEIGHT,
		0.9f, 0.9f, 0.9f, 1.f,
		0.f, 0.f, 1.0f, 1.0f);
	DrawSpriteQuad(g_mountain,
		pos_mountain - SCREEN_WIDTH, -camera->Position.y * 0.2, SCREEN_WIDTH, SCREEN_HEIGHT,
		0.9f, 0.9f, 0.9f, 1.f,
		0.f, 0.f, 1.0f, 1.0f);

	double pos_mountain_mid = -(pos * 0.5 - (int)(pos * 0.5)) * SCREEN_WIDTH;
	DrawSpriteQuad(g_mountain_mid,
		pos_mountain_mid, -camera->Position.y * 0.5, SCREEN_WIDTH, SCREEN_HEIGHT,
		0.9f, 0.9f, 0.9f, 1.f,
		0.f, 0.f, 1.0f, 1.0f);
	DrawSpriteQuad(g_mountain_mid,
		pos_mountain_mid + SCREEN_WIDTH, -camera->Position.y * 0.5, SCREEN_WIDTH, SCREEN_HEIGHT,
		0.9f, 0.9f, 0.9f, 1.f,
		0.f, 0.f, 1.0f, 1.0f);
	DrawSpriteQuad(g_mountain_mid,
		pos_mountain_mid - SCREEN_WIDTH, -camera->Position.y * 0.5, SCREEN_WIDTH, SCREEN_HEIGHT,
		0.9f, 0.9f, 0.9f, 1.f,
		0.f, 0.f, 1.0f, 1.0f);

	double pos_hill = -(pos * 0.8 - (int)(pos * 0.8)) * SCREEN_WIDTH;
	DrawSpriteQuad(g_hill,
		pos_hill, -camera->Position.y * 0.8, SCREEN_WIDTH, SCREEN_HEIGHT,
		0.9f, 0.72f, 0.9f, 1.f,
		0.f, 0.f, 1.0f, 1.0f);
	DrawSpriteQuad(g_hill,
		pos_hill + SCREEN_WIDTH, -camera->Position.y * 0.8, SCREEN_WIDTH, SCREEN_HEIGHT,
		0.9f, 0.72f, 0.9f, 1.f,
		0.f, 0.f, 1.0f, 1.0f);
	DrawSpriteQuad(g_hill,
		pos_hill - SCREEN_WIDTH, -camera->Position.y * 0.8, SCREEN_WIDTH, SCREEN_HEIGHT,
		0.9f, 0.72f, 0.9f, 1.f,
		0.f, 0.f, 1.0f, 1.0f);
	
	Rect{ 0, (s3d::Rect::value_type)(-camera->Position.y * 0.8 + SCREEN_HEIGHT), SCREEN_WIDTH, SCREEN_HEIGHT}.draw({0.1f, 0.72f, 0.2f, 1.f});
}
