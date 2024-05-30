# include <Siv3D.hpp>
#include "Main.h"
#include "Sprite.h"
#include "explosion.h"
#include "camera.h"
#include "action_enemy.h"

static AssetNameView g_Explosion = U"Explosion", g_Explosion_2 = U"Explosion_2";

#define EXPLOSION_MAX ENEMY_MAX
#define EXPLOSION_CNT_MAX 15

static EXPLOSION EXP[EXPLOSION_MAX];


void InitExplosion(void)
{
	TextureAsset::Register(g_Explosion, U"Action/Texture/explosion2.tga");
	TextureAsset::Register(g_Explosion_2, U"Action/Texture/efect_explosion.tga");

	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		EXP[i].Enable = false;
		EXP[i].Position.x = 0.f;
		EXP[i].Position.y = 0.f;
		EXP[i].Size = 0.f;
		EXP[i].Count = 0;
	}
}

void UninitExplosion(void)
{
	TextureAsset::Release(g_Explosion);
	TextureAsset::Release(g_Explosion_2);
}

void UpdateExplosion(void)
{
	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		if (EXP[i].Enable == true)
		{
			EXP[i].Count++;
			if (EXP[i].Count > EXPLOSION_CNT_MAX)
			{
				EXP[i].Enable = false;
			}
		}
	}
}

void DrawExplosion(void)
{
	CAMERA *camera = GetCamera();
	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		float tw, th;
		switch (EXP[i].type)
		{
		case ENEMY_DAMAGED:
			tw = 1.0f / 8.0f;
			th = 1.0f / 4.0f;

			if (EXP[i].Enable == true)
			{
				float u = (float)(EXP[i].Count % 8) / 8.f;
				float v = (float)((EXP[i].Count / 8) + 1) / 4.f;
				DrawSpriteQuad(g_Explosion_2,
					EXP[i].Position.x - camera->Position.x, EXP[i].Position.y - camera->Position.y, EXP[i].Size, EXP[i].Size,
					1.f, 1.f, 1.f, 1.f, u, v, tw, th);
			}
			break;
		case PLAYER_DAMAGED:
			tw = 1.0f / 4.0f;
			th = 1.0f / 4.0f;

			if (EXP[i].Enable == true)
			{
				float u = (float)(EXP[i].Count % 4) / 4.f;
				float v = (float)(EXP[i].Count / 4) / 4.f;
				DrawSpriteQuad(g_Explosion,
					EXP[i].Position.x - camera->Position.x, EXP[i].Position.y - camera->Position.y, EXP[i].Size, EXP[i].Size,
					1.f, 1.f, 1.f, 1.f, u, v, tw, th);
			}
			break;
		}
	}
}

void CreateExplosion(Float2 pos, float size, int type)
{
	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		if (EXP[i].Enable == false)
		{
			EXP[i].Position = pos;
			EXP[i].Size = size + 10;
			EXP[i].Enable = true;
			EXP[i].Count = 0;
			EXP[i].type = type;
			break;
		}
	}
}
