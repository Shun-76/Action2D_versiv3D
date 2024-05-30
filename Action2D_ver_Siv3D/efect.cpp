# include <Siv3D.hpp>
#include "Main.h"
#include "Sprite.h"
#include "player.h"
#include "efect.h"
#include "camera.h"
#include "explosion.h"

#define pi 3.141592f

EFECT g_Efect[EFECT_MAX];

static AssetNameView g_smoke = U"efect_smoke", g_death = U"efect_death", g_kirakira = U"efect_kirakira", g_jump = U"efect_jump";
static bool DeathEfect;
static bool DethFromFall;

static void DrawKirakira(AssetNameView texture, int index);
static void DrawJump(AssetNameView texture, int index);

void InitEfect(void)
{
	TextureAsset::Register(g_smoke, U"Action/Texture/smoke.tga");
	TextureAsset::Register(g_death, U"Action/Texture/tama.tga");
	TextureAsset::Register(g_kirakira, U"Action/Texture/kirakira.tga");
	TextureAsset::Register(g_jump, U"Action/Texture/Jump_effect.tga");

	for (int i = 0; i < EFECT_MAX; i++)
	{
		g_Efect[i].Position.x = 0.f;
		g_Efect[i].Position.y = 0.f;
		g_Efect[i].Enable = false;
		g_Efect[i].Count = 0;
	}
	DeathEfect = false;
	DethFromFall = false;
}

void UninitEfect(void)
{
	TextureAsset::Release(g_smoke);
	TextureAsset::Release(g_death);
	TextureAsset::Release(g_kirakira);
	TextureAsset::Release(g_jump);
}

void UpdateEfect(void)
{
	int CountMax = 0;
	PLAYER *player = GetPlayer();
	for (int i = 0; i < EFECT_MAX; i++)
	{
		if (g_Efect[i].Enable)
		{
			g_Efect[i].Position.x += g_Efect[i].Velocity.x;
			g_Efect[i].Position.y += g_Efect[i].Velocity.y;
			g_Efect[i].Count++;
			switch (g_Efect[i].TYPE)
			{
			case EFECT_TYPE_SMOKE:
				CountMax = EFECT_CNT_MAX;
				break;
			case EFECT_TYPE_DEATH:
				CountMax = DEATH_CNT_MAX;
				if (g_Efect[i].Count % 5 == 0)
				{
					g_Efect[i].Color.r = (float)(((int)g_Efect[i].Color.r + 1) % 2);
					g_Efect[i].Color.g = (float)(((int)g_Efect[i].Color.g + 1) % 2);
				}
				break;
			case EFECT_TYPE_KIRAKIRA:
				CountMax = EFECT_CNT_MAX * 3;
				break;
			case EFECT_TYPE_JUMP:
				CountMax = EFECT_CNT_MAX;
				break;
			}
			g_Efect[i].Velocity.x -= g_Efect[i].Velocity.x * 0.05f;
			g_Efect[i].Velocity.y -= g_Efect[i].Velocity.y * 0.05f;
			if (g_Efect[i].Count > CountMax)
			{
				g_Efect[i].Enable = false;
			}
		}
	}
	if ((DeathEfect) && (player->DeathCount % 10 == 0))
	{
		Float2 exp;
		int rad_rand = 2 + rand() % 3;
		exp.x = player->Position.x + (player->size.x / rad_rand) * cosf(2 * (pi / 5) * (player->DeathCount / 10));
		exp.y = player->Position.y + (player->size.y / rad_rand) * cosf(2 * (pi / 5) * (player->DeathCount / 10));
		CreateExplosion(player->Position, (player->size.y / 2), PLAYER_DAMAGED);
		float flag = (float)((player->DeathCount / 10) % 2);
		player->Color.r = 1.0f;
		player->Color.g = flag;
		player->Color.b = flag;
	}
	if (player->DeathCount > DEATH_CNT_MAX)
	{
		DeathEfect = false;
		DethFromFall = false;
	}
}

void DrawEfect(void)
{
	CAMERA *camera = GetCamera();
	AssetNameView texture = U"";
	for (int i = 0; i < EFECT_MAX; i++)
	{
		if (g_Efect[i].Enable)
		{
			switch (g_Efect[i].TYPE)
			{
			case EFECT_TYPE_SMOKE:
				texture = g_smoke;
				break;
			case EFECT_TYPE_DEATH:
				texture = g_death;
				break;
			case EFECT_TYPE_KIRAKIRA:
				texture = g_kirakira;
				break;
			case EFECT_TYPE_JUMP:
				texture = g_jump;
				break;
			}
			if (g_Efect[i].TYPE == EFECT_TYPE_KIRAKIRA)
			{
				DrawKirakira(texture, i);
			}
			else if (g_Efect[i].TYPE == EFECT_TYPE_JUMP)
			{
				DrawJump(texture, i);
			}
			else
			{
				if (g_Efect[i].vel == VEL_RIGHT)
				{
					DrawSpriteQuad(texture,
						g_Efect[i].Position.x - camera->Position.x, g_Efect[i].Position.y - camera->Position.y,
						g_Efect[i].size.x, g_Efect[i].size.y,
						g_Efect[i].Color.r, g_Efect[i].Color.g, g_Efect[i].Color.b, g_Efect[i].Color.a,
						0.f, 0.f, 1.f, 1.f);
				}
				else
				{
					DrawSpriteQuad(texture,
						g_Efect[i].Position.x - camera->Position.x, g_Efect[i].Position.y - camera->Position.y,
						g_Efect[i].size.x, g_Efect[i].size.y,
						g_Efect[i].Color.r, g_Efect[i].Color.g, g_Efect[i].Color.b, g_Efect[i].Color.a,
						0.f, 0.f, 1.f, 1.f,
						true);
				}
			}
		}
	}
}

void CreateSmoke(float Position_x, float Position_y, int vel, float size)
{
	for (int num = 0; num < EFECT_MAX; num++)
	{
		if (g_Efect[num].Enable == false)
		{
			g_Efect[num].TYPE = EFECT_TYPE_SMOKE;
			g_Efect[num].Enable = true;
			g_Efect[num].vel = vel;
			g_Efect[num].Position.x = Position_x + vel * size * 0.5f;
			g_Efect[num].Position.y = Position_y + PLAYER_SIZE * 0.5f - size * 0.5f;
			g_Efect[num].Velocity.x = vel * 4.f;
			g_Efect[num].Velocity.y = -0.8f;
			g_Efect[num].size.x = size;
			g_Efect[num].size.y = size;
			g_Efect[num].Count = 0;
			g_Efect[num].Color.r = 1.f;
			g_Efect[num].Color.g = 1.f;
			g_Efect[num].Color.b = 1.f;
			g_Efect[num].Color.a = 1.f;
			break;
		}
	}
}

void CreateDeathEfect(float Position_x, float Position_y, bool fall)
{
	DeathEfect = true;
	DethFromFall = fall;
	int i = 0;
	for (int num = 0; num < EFECT_MAX; num++)
	{
		if (g_Efect[num].Enable == false)
		{
			g_Efect[num].TYPE = EFECT_TYPE_DEATH;
			g_Efect[num].Enable = true;
			g_Efect[num].vel = 1;
			g_Efect[num].Position.x = Position_x;
			if (fall)
			{
				g_Efect[num].Position.y = SCREEN_HEIGHT * 0.5f;
			}
			else
			{
				g_Efect[num].Position.y = Position_y;
			}
			g_Efect[num].Velocity.x = 10.f * cosf(i * pi / 4);
			g_Efect[num].Velocity.y = 10.f * sinf(i * pi / 4);
			g_Efect[num].size.x = EFECT_SIZE;
			g_Efect[num].size.y = EFECT_SIZE;
			g_Efect[num].Count = 0;
			g_Efect[num].Color.r = 1.f;
			g_Efect[num].Color.g = 1.f;
			g_Efect[num].Color.b = 1.f;
			g_Efect[num].Color.a = 1.f;
			i++;
		}
		if (i >= 8)
		{
			break;
		}
	}
}

void CreateKirakira(float Position_x, float Position_y)
{
	for (int num = 0; num < EFECT_MAX; num++)
	{
		if (g_Efect[num].Enable == false)
		{
			g_Efect[num].TYPE = EFECT_TYPE_KIRAKIRA;
			g_Efect[num].Enable = true;
			g_Efect[num].vel = 1;
			g_Efect[num].Position.x = Position_x;
			g_Efect[num].Position.y = Position_y;
			g_Efect[num].Velocity.x = 0.f;
			g_Efect[num].Velocity.y = 0.f;
			g_Efect[num].size.x = EFECT_SIZE;
			g_Efect[num].size.y = EFECT_SIZE;
			g_Efect[num].Count = 0;
			g_Efect[num].Color.r = 1.f;
			g_Efect[num].Color.g = 1.f;
			g_Efect[num].Color.b = 1.f;
			g_Efect[num].Color.a = 1.f;
			break;
		}
	}
}

void CreateJump(float Position_x, float Position_y)
{
	for (int num = 0; num < EFECT_MAX; num++)
	{
		if (g_Efect[num].Enable == false)
		{
			g_Efect[num].TYPE = EFECT_TYPE_JUMP;
			g_Efect[num].Enable = true;
			g_Efect[num].vel = 1;
			g_Efect[num].Position.x = Position_x;
			g_Efect[num].Position.y = Position_y;
			g_Efect[num].Velocity.x = 0.f;
			g_Efect[num].Velocity.y = 0.f;
			g_Efect[num].size.x = 2 * EFECT_SIZE;
			g_Efect[num].size.y = 1.5 * EFECT_SIZE;
			g_Efect[num].Count = 0;
			g_Efect[num].Color.r = 1.f;
			g_Efect[num].Color.g = 1.f;
			g_Efect[num].Color.b = 1.f;
			g_Efect[num].Color.a = 1.f;
			break;
		}
	}
}

static void DrawKirakira(AssetNameView texture, int index)
{
	CAMERA *camera = GetCamera();

	int count = g_Efect[index].Count / 5;
	int cnt = count % 11;

	float tw, th;
	tw = 1.f / 11.f;
	th = 70.f / 360.f;

	float u = (1.f * cnt) / 11.f;
	float v = 0.f;

	DrawSpriteQuad(texture,
		g_Efect[index].Position.x - camera->Position.x, g_Efect[index].Position.y - camera->Position.y,
		g_Efect[index].size.x, g_Efect[index].size.y,
		g_Efect[index].Color.r, g_Efect[index].Color.g, g_Efect[index].Color.b, g_Efect[index].Color.a,
		u, v, tw, th);
}

static void DrawJump(AssetNameView texture, int index)
{
	CAMERA *camera = GetCamera();

	int count = g_Efect[index].Count / 6;
	int cnt = count % 11;

	float tw, th;
	tw = 240.f / 1440.f;
	th = 1.f;

	float u = (240.f * cnt) / 1440.f;
	float v = 0.f;

	DrawSpriteQuad(texture,
		g_Efect[index].Position.x - camera->Position.x, g_Efect[index].Position.y - camera->Position.y,
		g_Efect[index].size.x, g_Efect[index].size.y,
		g_Efect[index].Color.r, g_Efect[index].Color.g, g_Efect[index].Color.b, g_Efect[index].Color.a,
		u, v, tw, th);
}
