# include <Siv3D.hpp>
#include "Main.h"
#include "Sprite.h"
#include "fade.h"

static AssetNameView g_Fader = U"fade_fader";
static float g_fade_cnt, fade_v;
struct SCREEN_FADER
{
	Float2 pos;
	float w;
	float h;
};
SCREEN_FADER g_fader;

static FADE g_fade_mode;

static bool isFade, isFadeEnd;

void InitFade(void)
{
	TextureAsset::Register(g_Fader, U"Action/Texture/Lording.tga");
	g_fade_cnt = 0;
	fade_v = 1920.f / (60.f * 0.2f);
	g_fade_mode = FADE_NONE;
	g_fader.pos.x = 0.f;
	g_fader.pos.y = 0.f;
	g_fader.w = 0.f;
	g_fader.h = 0;
	isFade = false;
	isFadeEnd = false;
}

void UninitFade(void)
{
	TextureAsset::Release(g_Fader);
}

void UpdateFade(void)
{
	if (g_fade_cnt > SCREEN_WIDTH)
	{
		//g_fade_mode = FADE_NONE;
		isFade = false;
		isFadeEnd = true;
		g_fade_cnt = 0;
	}

	if ((isFadeEnd == true) && (g_fade_mode == FADE_IN))
	{
		g_fade_mode = FADE_NONE;
		isFadeEnd = false;
		SceneChange();
	}
	else if ((isFadeEnd == true) && (g_fade_mode == FADE_OUT))
	{
		g_fade_mode = FADE_NONE;
		isFadeEnd = false;
	}

	if (g_fade_mode != FADE_NONE)
	{
		g_fade_cnt += fade_v;
	}
	else
	{
		isFade = false;
	}

	switch (g_fade_mode)
	{
	case FADE_IN:
		if (g_fade_cnt <= SCREEN_WIDTH)
		{
			g_fader.pos.x -= fade_v / 2;
			g_fader.w += fade_v;
		}
		break;
	case FADE_OUT:
		if (g_fade_cnt <= SCREEN_WIDTH)
		{
			g_fader.pos.x -= fade_v / 2;
			g_fader.w -= fade_v;
		}
		break;
	default:
		break;
	}
}

void DrawFade(void)
{
	DrawSpriteQuad(g_Fader,
		g_fader.pos.x, g_fader.pos.y, g_fader.w, g_fader.h,
		1.f, 1.f, 1.f, 1.f,
		0.f, 0.f, 1.0f, 1.0f);
}

void FadeIn(void)
{
	g_fade_cnt = 0;
	g_fade_mode = FADE_IN;

	g_fader.pos.x = SCREEN_WIDTH / 2;
	g_fader.pos.y = 0.f;
	g_fader.w = 0.f;
	g_fader.h = SCREEN_HEIGHT;

	isFade = true;
	isFadeEnd = false;
}

void FadeOut(void)
{
	g_fade_cnt = 0;
	g_fade_mode = FADE_OUT;

	g_fader.pos.x = 0;
	g_fader.pos.y = 0.f;
	g_fader.w = SCREEN_WIDTH;
	g_fader.h = SCREEN_HEIGHT;

	isFade = true;
	isFadeEnd = false;
}

bool isFadeCheck(void)
{
	return isFade;
}

FADE GetFadeMode(void)
{
	return g_fade_mode;
}

bool *GetFadeEnd(void)
{
	return &isFadeEnd;
}
