# include <Siv3D.hpp>
#include "Main.h"
#include "Sound.h"
#include "time.h"
#include "Sprite.h"
#include "title.h"

static float g_Scroll;
static int g_time, Cnt;

static AssetNameView
g_Texture = U"title_title2",
g_Backg = U"title_backg",
g_Start = U"title_start",
g_Sky = U"title_sky",
g_mountain = U"title_mountain",
g_mountain_mid = U"title_mountain_mid",
g_hill = U"title_hill",
g_tegaki = U"title_tegaki";


#define pi 3.141592f
s3d::FilePathView un_backg;
void InitTitle(void)
{
	TextureAsset::Register(g_Texture, U"Action/Texture/title2.tga");
	TextureAsset::Register(g_Backg, U"Action/Texture/backg.tga");
	TextureAsset::Register(g_Start, U"Action/Texture/PushToStart.tga");
	TextureAsset::Register(g_Sky, U"Action/Texture/sky.tga");
	TextureAsset::Register(g_mountain, U"Action/Texture/mountain1.tga");
	TextureAsset::Register(g_mountain_mid, U"Action/Texture/mountain_mid.tga");
	TextureAsset::Register(g_hill, U"Action/Texture/hill.tga");
	TextureAsset::Register(g_tegaki, U"Action/Texture/tegaki.tga");

	g_time = 0;
	Cnt = -1;

	SetVolumeBGM(0.1);
	SetVolumeSE(0.05);

	PlayBGM(STRM_BLOOD_BLAST);
}

void UninitTitle(void)
{
	TextureAsset::Release(g_Texture);
	TextureAsset::Release(g_Backg);
	TextureAsset::Release(g_Start);
	TextureAsset::Release(g_Sky);
	TextureAsset::Release(g_mountain);
	TextureAsset::Release(g_mountain_mid);
	TextureAsset::Release(g_hill);
	TextureAsset::Release(g_tegaki);

	StopBGM();
}

void UpdateTitle(void)
{
	g_Scroll += 0.015f;
	g_time++;
	if (g_Scroll > 1.0f)
	{
		g_Scroll -= 1.0f;
	}

	if (g_time > 60)
	{
		if (KeySpace.down())
		{
			PlaySE(WSD_SELECT01);
			_SetScene(SCENE_GAME);
		}
		Cnt = (g_time / 30) % 2;
	}
}

void DrawTitle(void)
{
	DrawSpriteQuad(g_Sky, 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.9f, 0.9f, 0.9f, 1.f, 0.f, 0.f, 1.0f, 1.0f);
	DrawSpriteQuad(g_mountain, 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.9f, 0.9f, 0.9f, 1.f, 0.f, 0.f, 1.0f, 1.0f);
	DrawSpriteQuad(g_mountain_mid, 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.9f, 0.9f, 0.9f, 1.f, 0.3f, 0.f, 1.0f, 1.0f);
	DrawSpriteQuad(g_hill, 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.9f, 0.72f, 0.9f, 1.f, 0.6f, 0.f, 1.0f, 1.0f);
	DrawSpriteQuad(g_Texture, 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.0f, 1.0f);
	DrawSpriteQuad(g_tegaki, -600.f, -250.f, 512.f, 108.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.0f, 1.0f, false, -pi / 8);
	if (g_time > 60)
	{
		switch (Cnt)
		{
		case 0:
			DrawSpriteQuad(g_Start, -460.f, 370.f, (SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.0f, 1.0f);
			break;
		case 1:
			DrawSpriteQuad(g_Start, -460.f, 380.f, (SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.5f, 0.5f, 0.5f, 0.5f, 0.f, 0.f, 1.0f, 1.0f);
			break;
		default:
			break;
		}
	}
}
