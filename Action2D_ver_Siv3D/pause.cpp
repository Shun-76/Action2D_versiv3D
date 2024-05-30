# include <Siv3D.hpp>
#include "Main.h"
#include "player.h"
#include "Sprite.h"
#include "sound.h"
#include "pause.h"
#include "camera.h"

static AssetNameView
g_shadow = U"pause_shadow",
g_cursur = U"pause_cusur",
g_Start = U"pause_start",
g_kanban = U"pause_kanban",
g_pause_tutorial_1 = U"pause_tutorial_1",
g_pause_tutorial_2 = U"pause_tutorial_2",
g_pause_tutorial_3 = U"pause_tutorial_3",
g_pause_tutorial_4 = U"pause_tutorial_4";
PAUSE g_tutorial[TUTORIAL_TYPE_NONE];
TUTORIAL g_Tutorial[TUTORIAL_TYPE_NONE];

static int tutorial, cnt, Cnt;
static float scroll, rad;
static bool isPause, Pushable;

#define pi 3.141592f

static void DrawTutorialKanban(int index);

void InitPause(void)
{
	g_tutorial[TUTORIAL_TYPE_1].Tex = &g_pause_tutorial_1;
	g_tutorial[TUTORIAL_TYPE_2].Tex = &g_pause_tutorial_2;
	g_tutorial[TUTORIAL_TYPE_3].Tex = &g_pause_tutorial_3;
	g_tutorial[TUTORIAL_TYPE_4].Tex = &g_pause_tutorial_4;

	TextureAsset::Register(g_shadow, U"Action/Texture/Pause_shadow.tga");
	TextureAsset::Register(g_cursur, U"Action/Texture/Pause_cursor.tga");
	TextureAsset::Register(g_Start, U"Action/Texture/PushToStart.tga");
	TextureAsset::Register(g_kanban, U"Action/Texture/kanban.tga");
	TextureAsset::Register(*g_tutorial[TUTORIAL_TYPE_1].Tex, U"Action/Texture/Pause_tutorial_1.tga");
	TextureAsset::Register(*g_tutorial[TUTORIAL_TYPE_2].Tex, U"Action/Texture/Pause_tutorial_2.tga");
	TextureAsset::Register(*g_tutorial[TUTORIAL_TYPE_3].Tex, U"Action/Texture/Pause_tutorial_3.tga");
	TextureAsset::Register(*g_tutorial[TUTORIAL_TYPE_4].Tex, U"Action/Texture/Pause_tutorial_4.tga");

	tutorial = 1;
	cnt = 0;
	Cnt = 0;
	Pushable = true;
	isPause = false;
	rad = pi / 2;

	for (int i = 0; i < 4; i++)
	{
		g_tutorial[i].Position.x = -SCREEN_WIDTH * cosf((i + tutorial) * rad);
		g_tutorial[i].NextPosition.x = -SCREEN_WIDTH * cosf((i + tutorial) * rad);
		if ((i + tutorial) % 2 == 1)
		{
			g_tutorial[i].Position.y = -2 * SCREEN_HEIGHT + 2 * SCREEN_HEIGHT * sinf((i + tutorial) * rad);
			g_tutorial[i].NextPosition.y = -2 * SCREEN_HEIGHT + 2 * SCREEN_HEIGHT * sinf((i + tutorial) * rad);
		}
		else
		{
			g_tutorial[i].Position.y = 0.f;
			g_tutorial[i].NextPosition.y = 0.f;
		}
		if ((i + tutorial) % 4 == 3)
		{
			g_tutorial[i].fill = 0.f;
		}
		else
		{
			g_tutorial[i].fill = 1.f;
		}
		g_Tutorial[i].Enable = false;
	}
}

void UninitPause(void)
{
	TextureAsset::Release(g_shadow);
	TextureAsset::Release(g_cursur);
	TextureAsset::Release(g_Start);
	TextureAsset::Release(g_kanban);
	for (int i = 0; i < TUTORIAL_TYPE_NONE; i++)
	{
		TextureAsset::Release(*g_tutorial[i].Tex);
	}
}

void UpdatePause(void)
{
	for (int i = 0; i < 4; i++)
	{
		g_tutorial[i].Position.x += (g_tutorial[i].NextPosition.x - g_tutorial[i].Position.x) * 0.1f;
		g_tutorial[i].Position.y += (g_tutorial[i].NextPosition.y - g_tutorial[i].Position.y) * 0.1f;
		if ((i + tutorial) % 4 == 3)
		{
			g_tutorial[i].fill = 0.f;
		}
		else
		{
			g_tutorial[i].fill = 1.f;
		}
	}
	if (isPause)
	{
		if (MouseL.down() || KeyTab.down())
		{
			isPause = false;
			PlaySE(WSD_SELECT01);
		}
		if (Pushable)
		{
			if (KeyD.down())
			{
				tutorial--;
				Pushable = false;
			}
			if (KeyA.down())
			{
				tutorial++;
				Pushable = false;
			}
			for (int i = 0; i < 4; i++)
			{
				g_tutorial[i].NextPosition.x = -SCREEN_WIDTH * cosf((i + tutorial) * rad);
				if ((i + tutorial) % 2 == 1)
				{
					g_tutorial[i].NextPosition.y = -2 * SCREEN_HEIGHT + 2 * SCREEN_HEIGHT * sinf((i + tutorial) * rad);
				}
				else
				{
					g_tutorial[i].NextPosition.y = 0.f;
				}
			}
		}
		else
		{
			if ((g_tutorial[0].NextPosition.x - g_tutorial[0].Position.x) < 150.f)
			{
				Pushable = true;
			}
		}
		cnt++;
		Cnt = (cnt / 30) % 2;
		if (tutorial < 0) tutorial = 3;
	}
	else
	{
		if (KeyTab.down())
		{
			SetPause();
		}
	}
}

void DrawPause(void)
{
	if (isPause)
	{
		DrawSpriteQuad(g_shadow, 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.0f, 1.0f);
		DrawSpriteQuad(g_cursur, 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.0f, 1.0f);
		for (int i = 0; i < TUTORIAL_TYPE_NONE; i++)
		{
			DrawSpriteQuad(*g_tutorial[i].Tex,
				g_tutorial[i].Position.x, g_tutorial[i].Position.y, SCREEN_WIDTH, SCREEN_HEIGHT,
				1.f, 1.f, 1.f, 1.f,
				0.f, 0.f, g_tutorial[i].fill, 1.0f);
		}
		switch (Cnt)
		{
		case 0:
			DrawSpriteQuad(g_Start, 460.f, 370.f, (SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.0f, 1.0f);
			break;
		case 1:
			DrawSpriteQuad(g_Start, 460.f, 380.f, (SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.5f, 0.5f, 0.5f, 0.5f, 0.f, 0.f, 1.0f, 1.0f);
			break;
		}
	}
}

bool GetPause(void)
{
	return isPause;
}

void SetPause(void)
{
	isPause = true;
	tutorial = 1;
}

void DrawTutorial(void)
{
	for (int i = 0; i < TUTORIAL_TYPE_NONE; i++) DrawTutorialKanban(i);
}

static void DrawTutorialKanban(int index)
{
	if (!g_Tutorial[index].Enable) return;
	CAMERA *camera = GetCamera();

	float tw, th;
	tw = 1.f;
	th = 1.f;

	float u = 0.f;
	float v = 0.f;
	float yoko = 2 * 255.f / 400.f;
	float tate = yoko * SCREEN_HEIGHT / SCREEN_WIDTH;
	float chushin = g_Tutorial[index].SIZE * 32.5f / 374.f;
	DrawSpriteQuad(g_kanban,
		g_Tutorial[index].Position.x - camera->Position.x, g_Tutorial[index].Position.y - camera->Position.y,
		g_Tutorial[index].SIZE, g_Tutorial[index].SIZE,
		g_Tutorial[index].color.r, g_Tutorial[index].color.g, g_Tutorial[index].color.b, g_Tutorial[index].color.a,
		u, v, tw, th);
	DrawSpriteQuad(*g_tutorial[g_Tutorial[index].Tex].Tex,
		g_Tutorial[index].Position.x - camera->Position.x, g_Tutorial[index].Position.y - chushin - camera->Position.y,
		g_Tutorial[index].SIZE * yoko, g_Tutorial[index].SIZE * tate,
		g_Tutorial[index].color.r, g_Tutorial[index].color.g, g_Tutorial[index].color.b, g_Tutorial[index].color.a,
		u, v, tw, th);
}

void CreateTutorial(ENUM_TUTORIAL en_tutorial, float pos_x, float pos_y, float SIZE)
{
	for (int i = 0; i < TUTORIAL_TYPE_NONE; i++)
	{
		if (g_Tutorial[i].Enable == false)
		{
			g_Tutorial[i].Tex = en_tutorial;
			g_Tutorial[i].Enable = true;
			g_Tutorial[i].Position.x = pos_x;
			g_Tutorial[i].Position.y = pos_y;
			g_Tutorial[i].SIZE = SIZE;
			g_Tutorial[i].color.r = 1.f;
			g_Tutorial[i].color.g = 1.f;
			g_Tutorial[i].color.b = 1.f;
			g_Tutorial[i].color.a = 1.f;
			break;
		}
	}
}
