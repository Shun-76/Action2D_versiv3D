# include <Siv3D.hpp>
#include "Main.h"
#include "Sprite.h"
#include "block.h"
#include "camera.h"
#include "player.h"
#include "gimmick.h"
#include "efect.h"
#include "sound.h"
#include "score.h"
#include "result.h"

#define GIMMICK_TYPE_CNT GIMMICK_TYPE_NONE
#define pi 3.141592653f

static AssetNameView gimmick_coin = U"gimmick_coin", gimmick_spike = U"gimmick_spike", gimmick_flag = U"gimmick_flag", gimmick_bounce = U"gimmick_bounce", gimmick_key = U"gimmick_key", gimmick_door = U"gimmick_door";
static AssetNameView* g_gimmick[GIMMICK_TYPE_CNT];
static GIMMICK g_Gimmick[GIMMICK_MAX];
static float radv;

static void DrawCoinAnim(GIMMICK *gimmick, int index);
static void DrawSpikeAnim(GIMMICK *gimmick, int index);
static void DrawCheckPoint(GIMMICK *gimmick, int index);
static void DrawBounceBall(GIMMICK *gimmick, int index);
static void DrawKey(GIMMICK *gimmick, int index);
static void DrawDoor(GIMMICK *gimmick, int index);

void InitGimmick(void)
{
	g_gimmick[GIMMICK_TYPE_COIN] = &gimmick_coin;
	g_gimmick[GIMMICK_TYPE_SPIKE] = &gimmick_spike;
	g_gimmick[GIMMICK_TYPE_CHECK_POINT] = &gimmick_flag;
	g_gimmick[GIMMICK_TYPE_BOUNCE] = &gimmick_bounce;
	g_gimmick[GIMMICK_TYPE_KEY] = &gimmick_key;
	g_gimmick[GIMMICK_TYPE_DOOR] = &gimmick_door;

	TextureAsset::Register(*g_gimmick[GIMMICK_TYPE_COIN], U"Action/Texture/action_coin.tga");
	TextureAsset::Register(*g_gimmick[GIMMICK_TYPE_SPIKE], U"Action/Texture/spike_ball2.tga");
	TextureAsset::Register(*g_gimmick[GIMMICK_TYPE_CHECK_POINT], U"Action/Texture/flag.tga");
	TextureAsset::Register(*g_gimmick[GIMMICK_TYPE_BOUNCE], U"Action/Texture/bounce_ball.tga");
	TextureAsset::Register(*g_gimmick[GIMMICK_TYPE_KEY], U"Action/Texture/Key.tga");
	TextureAsset::Register(*g_gimmick[GIMMICK_TYPE_DOOR], U"Action/Texture/Door.tga");


	for (int i = 0; i < GIMMICK_MAX; i++)
	{
		g_Gimmick[i].TYPE = GIMMICK_TYPE_NONE;
		g_Gimmick[i].Enable = false;
		g_Gimmick[i].Get = false;
		g_Gimmick[i].Position.x = 0.f;
		g_Gimmick[i].Position.y = 0.f;
		g_Gimmick[i].Velocity.x = 0.f;
		g_Gimmick[i].Velocity.y = 0.f;
		g_Gimmick[i].Scale.x = 1.f;
		g_Gimmick[i].Scale.y = 1.f;
		g_Gimmick[i].Scale_Offset = 0.f;
		g_Gimmick[i].SIZE = 0;
		g_Gimmick[i].cnt_anim = 0;
		g_Gimmick[i].cnt_get = 0;
	}

	radv = pi / 3;
}

void UninitGimmick(void)
{
	for (int i = 0; i < GIMMICK_TYPE_CNT; i++)
	{
		TextureAsset::Release(*g_gimmick[i]);
	}
}

void UpdateGimmick(void)
{
	PLAYER *player = GetPlayer();
	for (int i = 0; i < GIMMICK_MAX; i++)
	{
		if (g_Gimmick[i].Enable)
		{
			g_Gimmick[i].Position.x += g_Gimmick[i].Velocity.x;
			g_Gimmick[i].Position.y += g_Gimmick[i].Velocity.y;
			g_Gimmick[i].Velocity.x -= g_Gimmick[i].Velocity.x * 0.1f;
			if ((g_Gimmick[i].Velocity.x < 0.1f) && (g_Gimmick[i].Velocity.x > -0.1f))
			{
				g_Gimmick[i].Velocity.x = 0.0f;
			}
			g_Gimmick[i].Velocity.y -= g_Gimmick[i].Velocity.y * 0.1f;
			if ((g_Gimmick[i].Velocity.y < 0.1f) && (g_Gimmick[i].Velocity.y > -0.1f))
			{
				g_Gimmick[i].Velocity.y = 0.0f;
			}

			g_Gimmick[i].Scale.x = 1.f + 0.5f * cosf(g_Gimmick[i].cnt_anim * radv) * g_Gimmick[i].Scale_Offset;
			g_Gimmick[i].Scale.y = 1.f + 0.5f * sinf(g_Gimmick[i].cnt_anim * radv) * g_Gimmick[i].Scale_Offset;

			g_Gimmick[i].Scale_Offset -= g_Gimmick[i].Scale_Offset * 0.1f;

			g_Gimmick[i].cnt_anim++;
			if (g_Gimmick[i].Get)
			{
				switch(g_Gimmick[i].TYPE)
				{
				case GIMMICK_TYPE_COIN:
					if (g_Gimmick[i].cnt_get > 20)
					{
						DeleteGimmick(i);
						CreateKirakira(g_Gimmick[i].Position.x, g_Gimmick[i].Position.y);
					}
					g_Gimmick[i].cnt_anim++;
					break;
				case GIMMICK_TYPE_CHECK_POINT:
					g_Gimmick[i].color.r = 0.2f;
					g_Gimmick[i].color.g = 0.2f;
					g_Gimmick[i].color.b = 1.f;
					g_Gimmick[i].color.a = 1.f;
					if (g_Gimmick[i].cnt_get == 0)
					{
						CreateKirakira(g_Gimmick[i].Position.x + g_Gimmick[i].SIZE / 2, g_Gimmick[i].Position.y - g_Gimmick[i].SIZE / 2);
						CreateKirakira(g_Gimmick[i].Position.x - g_Gimmick[i].SIZE / 2, g_Gimmick[i].Position.y + g_Gimmick[i].SIZE / 2);
					}
					break;
				case GIMMICK_TYPE_KEY:
					if (g_Gimmick[i].cnt_get > 20)
					{
						DeleteGimmick(i);
						CreateKirakira(g_Gimmick[i].Position.x + g_Gimmick[i].SIZE / 2, g_Gimmick[i].Position.y - g_Gimmick[i].SIZE / 2);
						CreateKirakira(g_Gimmick[i].Position.x - g_Gimmick[i].SIZE / 2, g_Gimmick[i].Position.y + g_Gimmick[i].SIZE / 2);
					}
					g_Gimmick[i].cnt_anim++;
					break;
				}
				g_Gimmick[i].cnt_get++;
			}
			if (g_Gimmick[i].TYPE == GIMMICK_TYPE_DOOR && player->DoorOpen == true)
			{
				if (g_Gimmick[i].cnt_get < 30)
				{
					g_Gimmick[i].cnt_get++;
				}
				else
				{
					SetResult(RESULT_GAMECLEAR);
					_SetScene(SCENE_RESULT);
				}
			}
		}
	}

}

void DrawGimmick(void)
{
	CAMERA *camera = GetCamera();
	for (int i = 0; i < GIMMICK_MAX; i++)
	{
		if (g_Gimmick[i].Enable)
		{
			//DrawSpriteQuad(g_Gimmick[i].Position.x - camera->Position.x, g_Gimmick[i].Position.y - camera->Position.y, g_Gimmick[i].SIZE.x, g_Gimmick[i].SIZE.y, 1.f, 1.f, 1.f, 1.f, g_gimmick[g_Gimmick[i].TYPE], 0.f, 0.f, 1.0f, 1.0f);
			switch (g_Gimmick[i].TYPE)
			{
			case GIMMICK_TYPE_COIN:
				DrawCoinAnim(g_Gimmick, i);
				break;
			case GIMMICK_TYPE_SPIKE:
				DrawSpikeAnim(g_Gimmick, i);
				break;
			case GIMMICK_TYPE_CHECK_POINT:
				DrawCheckPoint(g_Gimmick, i);
				break;
			case GIMMICK_TYPE_BOUNCE:
				DrawBounceBall(g_Gimmick, i);
				break;
			case GIMMICK_TYPE_KEY:
				DrawKey(g_Gimmick, i);
				break;
			case GIMMICK_TYPE_DOOR:
				DrawDoor(g_Gimmick, i);
				break;
			default:
				break;
			}
		}
	}
}

static void DrawCoinAnim(GIMMICK *gimmick, int index)
{
	if (gimmick[index].TYPE != GIMMICK_TYPE_COIN) return;
	CAMERA *camera = GetCamera();

	int count = gimmick[index].cnt_anim / 5;
	int cnt = count % 6;

	float tw, th;
	tw = 12.0f / 192.f;
	th = 16.f / 32.f;

	float u = (10.f + 32.f * cnt) / 192.f;
	float v = 8.f / 32.f;
	DrawSpriteQuad(*g_gimmick[g_Gimmick[index].TYPE],
		gimmick[index].Position.x - camera->Position.x, gimmick[index].Position.y - camera->Position.y,
		gimmick[index].SIZE * gimmick[index].Scale.x, gimmick[index].SIZE * gimmick[index].Scale.y,
		gimmick[index].color.r, gimmick[index].color.g, gimmick[index].color.b, gimmick[index].color.a,
		u, v, tw, th);
}

static void DrawSpikeAnim(GIMMICK *gimmick, int index)
{
	if (gimmick[index].TYPE != GIMMICK_TYPE_SPIKE) return;
	CAMERA *camera = GetCamera();

	int count = gimmick[index].cnt_anim / 5;
	int cnt = count % 6;

	float tw, th;
	tw = 1.f;
	th = 1.f;

	float u = 0.f;
	float v = 0.f;
	DrawSpriteQuad(*g_gimmick[g_Gimmick[index].TYPE],
		gimmick[index].Position.x - camera->Position.x, gimmick[index].Position.y - camera->Position.y,
		gimmick[index].SIZE * gimmick[index].Scale.x, gimmick[index].SIZE * gimmick[index].Scale.y,
		gimmick[index].color.r, gimmick[index].color.g, gimmick[index].color.b, gimmick[index].color.a,
		u, v, tw, th);
}

static void DrawCheckPoint(GIMMICK *gimmick, int index)
{
	if (gimmick[index].TYPE != GIMMICK_TYPE_CHECK_POINT) return;
	CAMERA *camera = GetCamera();

	float tw, th;
	tw = 1.f;
	th = 1.f;

	float u = 0.f;
	float v = 0.f;
	DrawSpriteQuad(*g_gimmick[g_Gimmick[index].TYPE],
		gimmick[index].Position.x - camera->Position.x, gimmick[index].Position.y - camera->Position.y,
		gimmick[index].SIZE * gimmick[index].Scale.x, gimmick[index].SIZE * gimmick[index].Scale.y,
		gimmick[index].color.r, gimmick[index].color.g, gimmick[index].color.b, gimmick[index].color.a,
		u, v, tw, th);
}

static void DrawBounceBall(GIMMICK *gimmick, int index)
{
	if (gimmick[index].TYPE != GIMMICK_TYPE_BOUNCE) return;
	CAMERA *camera = GetCamera();

	float tw, th;
	tw = 1.f;
	th = 1.f;

	float u = 0.f;
	float v = 0.f;
	DrawSpriteQuad(*g_gimmick[g_Gimmick[index].TYPE],
		gimmick[index].Position.x - camera->Position.x, gimmick[index].Position.y - camera->Position.y,
		gimmick[index].SIZE * gimmick[index].Scale.x, gimmick[index].SIZE * gimmick[index].Scale.y,
		gimmick[index].color.r, gimmick[index].color.g, gimmick[index].color.b, gimmick[index].color.a,
		u, v, tw, th);
}

static void DrawKey(GIMMICK *gimmick, int index)
{
	if (gimmick[index].TYPE != GIMMICK_TYPE_KEY) return;
	CAMERA *camera = GetCamera();

	float tw, th;
	tw = 1.f;
	th = 1.f;

	float u = 0.f;
	float v = 0.f;
	DrawSpriteQuad(*g_gimmick[g_Gimmick[index].TYPE],
		gimmick[index].Position.x - camera->Position.x, gimmick[index].Position.y - camera->Position.y,
		gimmick[index].SIZE * gimmick[index].Scale.x, gimmick[index].SIZE * gimmick[index].Scale.y,
		gimmick[index].color.r, gimmick[index].color.g, gimmick[index].color.b, gimmick[index].color.a,
		u, v, tw, th);
}

static void DrawDoor(GIMMICK *gimmick, int index)
{
	if (gimmick[index].TYPE != GIMMICK_TYPE_DOOR) return;
	CAMERA *camera = GetCamera();
	PLAYER *player = GetPlayer();

	float tw, th;
	tw = 1.f;
	th = 1.f;

	float u = 0.f;
	float v = 0.f;

	if (player->DoorOpen)
	{

		DrawSpriteQuad(*g_gimmick[g_Gimmick[index].TYPE],
			gimmick[index].Position.x - camera->Position.x, gimmick[index].Position.y - camera->Position.y,
			gimmick[index].SIZE * gimmick[index].Scale.x, gimmick[index].SIZE * gimmick[index].Scale.y,
			0.f, 0.f, 0.f, gimmick[index].color.a,
			u, v, tw, th);

		float fade_v = 100.f / 30;
		DrawSpriteQuad(*g_gimmick[g_Gimmick[index].TYPE],
			gimmick[index].Position.x - camera->Position.x + fade_v * gimmick[index].cnt_get * 0.5f, gimmick[index].Position.y - camera->Position.y,
			gimmick[index].SIZE * gimmick[index].Scale.x - fade_v * gimmick[index].cnt_get, gimmick[index].SIZE * gimmick[index].Scale.y,
			gimmick[index].color.r, gimmick[index].color.g, gimmick[index].color.b, gimmick[index].color.a,
			u, v, tw, th);
	}
	else
	{
		DrawSpriteQuad(*g_gimmick[g_Gimmick[index].TYPE],
			gimmick[index].Position.x - camera->Position.x, gimmick[index].Position.y - camera->Position.y,
			gimmick[index].SIZE * gimmick[index].Scale.x, gimmick[index].SIZE * gimmick[index].Scale.y,
			gimmick[index].color.r, gimmick[index].color.g, gimmick[index].color.b, gimmick[index].color.a,
			u, v, tw, th);
	}
}

GIMMICK* GetGimmick(void)
{
	return g_Gimmick;
}

void CreateGimmick(GIMMICK_TYPE TYPE, float pos_x, float pos_y, float SIZE, int Shop)
{
	for (int i = 0; i < GIMMICK_MAX; i++)
	{
		if (g_Gimmick[i].Enable == false)
		{
			g_Gimmick[i].Enable = true;
			g_Gimmick[i].Get = false;
			g_Gimmick[i].Position.x = pos_x;
			g_Gimmick[i].Position.y = pos_y;
			g_Gimmick[i].Velocity.x = 0.f;
			g_Gimmick[i].Velocity.y = 0.f;
			g_Gimmick[i].Scale.x = 1.f;
			g_Gimmick[i].Scale.y = 1.f;
			g_Gimmick[i].Scale_Offset = 0.f;
			g_Gimmick[i].SIZE = SIZE;
			g_Gimmick[i].TYPE = TYPE;
			g_Gimmick[i].cnt_anim = 0;
			g_Gimmick[i].cnt_get = 0;
			if (TYPE == GIMMICK_TYPE_CHECK_POINT)
			{
				g_Gimmick[i].color.r = 1.f;
				g_Gimmick[i].color.g = 0.2f;
				g_Gimmick[i].color.b = 0.2f;
				g_Gimmick[i].color.a = 1.f;
			}
			else
			{
				g_Gimmick[i].color.r = 1.f;
				g_Gimmick[i].color.g = 1.f;
				g_Gimmick[i].color.b = 1.f;
				g_Gimmick[i].color.a = 1.f;
			}
			break;
		}
	}
}

void CreateGimmickRectangle(GIMMICK_TYPE TYPE, float pos_x, float pos_y, float SIZE, int w, int h)
{
	Float2 pos;
	pos.x = pos_x;
	pos.y = pos_y;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			CreateGimmick(TYPE, pos.x, pos.y, SIZE, false);
			pos.x += BLOCK_SIZE / 2;
		}
		pos.x = pos_x;
		pos.y += BLOCK_SIZE / 2;
	}
}

void DeleteGimmick(int index)
{
	g_Gimmick[index].Enable = false;
}

void GimmickGet(int index)
{
	PLAYER *player = GetPlayer();
	g_Gimmick[index].Get = true;
	switch (g_Gimmick[index].TYPE)
	{
	case GIMMICK_TYPE_COIN:
		g_Gimmick[index].Velocity.y = -10.f;
		g_Gimmick[index].Scale_Offset = 1.3f;
		PlaySE(WSD_COIN);
		AddScore(1);
		break;
	case GIMMICK_TYPE_KEY:
		player->Key = true;
		g_Gimmick[index].Velocity.y = -10.f;
		g_Gimmick[index].Scale_Offset = 1.3f;
		PlaySE(WSD_COIN);
		break;
	case GIMMICK_TYPE_DOOR:
		g_Gimmick[index].Get = false;
		break;
	case GIMMICK_TYPE_BOUNCE:
		g_Gimmick[index].Scale_Offset = -0.8f;
		g_Gimmick[index].Get = false;
		Float2 vel;
		vel.x = player->Position.x - g_Gimmick[index].Position.x;
		vel.y = player->Position.y - g_Gimmick[index].Position.y;
		player->Velocity.x = vel.x * 0.5f;
		if (vel.y < 0)
		{
			player->Velocity.y = vel.y * 0.35f;
		}
		else
		{
			player->Velocity.y = vel.y * 0.1f;
		}
		PlaySE(WSD_BOYON);
		break;
	case GIMMICK_TYPE_SPIKE:
		AddPlayerDamage(1);
		break;
	case GIMMICK_TYPE_CHECK_POINT:
		player->CheckPoint = g_Gimmick[index].Position;
		g_Gimmick[index].Scale_Offset = 1.3f;
		checkpointScore();
		break;
	}
}

void failedDoorOpen(int index)
{
	if (g_Gimmick[index].TYPE != GIMMICK_TYPE_DOOR) return;
	g_Gimmick[index].Scale_Offset = 1.000001f;
}
