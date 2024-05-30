# include <Siv3D.hpp>
#include "Main.h"
#include "Sprite.h"
#include "score.h"
#include "player.h"
#include "gimmick.h"
#include "sound.h"

static AssetNameView
g_score = U"score_num",
g_player = U"score_player",
g_coin = U"score_coin",
g_continue = U"score_continue",
g_vignette = U"score_vignette",
g_key = U"score_key",
g_shadow = U"score_shadow";

static SCORE g_Score;

static bool isContinue;

static GIMMICK Coin;

static void Score_Point(void);
static void Score(void);
static void Life(void);
static void Zanki(void);

void InitScore(void)
{
	TextureAsset::Register(g_player, U"Action/Texture/action_ninja.tga");
	TextureAsset::Register(g_score, U"Action/Texture/number.tga");
	TextureAsset::Register(g_coin, U"Action/Texture/action_coin.tga");
	TextureAsset::Register(g_continue, U"Action/Texture/Continue.tga");
	TextureAsset::Register(g_vignette, U"Action/Texture/vignette.tga");
	TextureAsset::Register(g_key, U"Action/Texture/Key.tga");
	TextureAsset::Register(g_shadow, U"Action/Texture/Pause_shadow.tga");

	g_Score.size = 100.f;
	g_Score.count = 0;
	g_Score.point = 0;
	g_Score.distance = 0;

	Coin.TYPE = GIMMICK_TYPE_COIN;
	Coin.SIZE = g_Score.size;
	Coin.Enable = true;
	Coin.cnt_anim = 0;

	isContinue = false;
}

void UninitScore(void)
{
	TextureAsset::Release(g_player);
	TextureAsset::Release(g_score);
	TextureAsset::Release(g_coin);
	TextureAsset::Release(g_continue);
	TextureAsset::Release(g_vignette);
	TextureAsset::Release(g_key);
	TextureAsset::Release(g_shadow);
}

void UpdateScore(void)
{
	PLAYER *player = GetPlayer();
	if (player->Alive == true)
	{
		
	}
	if (player->Enable || player->Zanki <= 0)
	{
		isContinue = false;
	}
	else
	{
		isContinue = true;
	}
	Coin.cnt_anim++;
	if (g_Score.display_point != g_Score.point)
	{
		int diff = g_Score.point - g_Score.display_point;
		if (diff > 10 || diff < -10)
		{
			g_Score.display_point += (int)(diff * 0.1f);
		}
		else
		{
			if (diff > 0)
			{
				g_Score.display_point++;
			}
			else
			{
				g_Score.display_point--;
			}
		}
	}
}

void DrawScore(void)
{
	//ポストプロセスエフェクト
	DrawSpriteQuad(g_vignette, 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT, 1.f, 1.f, 1.f, 0.3f, 0.f, 0.f, 1.0f, 1.0f);
	
	Score_Point();
	Score();
	Life();
	Zanki();
	if (isContinue)
	{
		DrawSpriteQuad(g_shadow, 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.0f, 1.0f);
		DrawSpriteQuad(g_continue, 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.0f, 1.0f);
	}
}

static void Score_Point(void)
{
	int point = g_Score.display_point, _point;
	Float2 pos;
	float tw, th, u, v;
	pos.x = SCREEN_WIDTH / 2 - g_Score.size * 0.75f;
	pos.y = -1 * SCREEN_HEIGHT / 2 + g_Score.size * 0.75f;
	tw = 1.f / 5.f;
	th = 1.f / 5.f;

	do
	{
		_point = point % 10;
		u = (float)(_point % 5) / 5.0f;
		v = (float)(_point / 5) / 5.0f;
		DrawSpriteQuad(g_score, pos.x, pos.y, g_Score.size, g_Score.size, 1.f, 1.f, 1.f, 1.f, u, v, tw, th);
		pos.x -= g_Score.size * 0.75f;
		point /= 10;
	} while (point > 0);
	PLAYER *player = GetPlayer();
	if (player->Key)
	{
		DrawSpriteQuad(g_key, pos.x, pos.y, g_Score.size, g_Score.size, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f);
	}
}

static void Score(void)
{
	int count = g_Score.count, _count;
	Float2 pos;
	float tw, th, u, v;
	pos.x = SCREEN_WIDTH / 2 - g_Score.size * 0.75f;
	pos.y = -1 * SCREEN_HEIGHT / 2 + 2 * g_Score.size * 0.85f;
	tw = 1.f / 5.f;
	th = 1.f / 5.f;

	do
	{
		_count = count % 10;
		u = (float)(_count % 5) / 5.0f;
		v = (float)(_count / 5) / 5.0f;
		DrawSpriteQuad(g_score, pos.x, pos.y, g_Score.size, g_Score.size, 1.f, 1.f, 1.f, 1.f, u, v, tw, th);
		pos.x -= g_Score.size * 0.75f;
		count /= 10;
	} while (count > 0);
	u = 1.f / 5.f;
	v = 2.f / 5.f;
	Coin.Position.x = pos.x;
	Coin.Position.y = pos.y;

	int count_coin = Coin.cnt_anim / 5;
	int cnt_coin = count_coin % 6;

	float tw_coin, th_coin;
	tw_coin = 12.0f / 192.f;
	th_coin = 16.f / 32.f;

	float u_coin = (10.f + 32.f * cnt_coin) / 192.f;
	float v_coin = 8.f / 32.f;
	DrawSpriteQuad(g_coin, Coin.Position.x, Coin.Position.y, Coin.SIZE, Coin.SIZE, 1.f, 1.f, 1.f, 1.f, u_coin, v_coin, tw_coin, th_coin);
}

static void Life(void)
{
	PLAYER *player = GetPlayer();
	int life = player->Life, _life;
	int count = 0, num = life;
	if (num <= 0)
	{
		life = 0;
		count = 1;
	}

	while (num > 0)
	{
		num /= 10;
		count++;
	}
	Float2 pos;
	float tw, th, u, v;
	pos.x = -1 * SCREEN_WIDTH / 2 + g_Score.size * (1 + count) * 0.75f;
	pos.y = -1 * SCREEN_HEIGHT / 2 + g_Score.size * 0.75f;
	tw = 1.f / 5.f;
	th = 1.f / 5.f;

	do
	{
		_life = life % 10;
		u = (float)(_life % 5) / 5.0f;
		v = (float)(_life / 5) / 5.0f;
		DrawSpriteQuad(g_score, pos.x, pos.y, g_Score.size, g_Score.size, 1.f, 1.f, 1.f, 1.f, u, v, tw, th);
		pos.x -= g_Score.size * 0.75f;
		life /= 10;
	} while (life > 0);
	u = 0.f;
	v = 2.f / 5.f;
	DrawSpriteQuad(g_score, pos.x, pos.y, g_Score.size, g_Score.size, 1.f, 0.5f, 0.5f, 1.f, u, v, tw, th);
}

static void Zanki(void)
{
	PLAYER *player = GetPlayer();
	int zanki = player->Zanki, _zanki, num = zanki, count = 0;

	if (num <= 0)
	{
		zanki = 0;
		count = 1;
	}

	while (num > 0)
	{
		num /= 10;
		count++;
	}
	Float2 pos;
	float tw, th, u, v;
	pos.x = -1 * SCREEN_WIDTH / 2 + g_Score.size * (1 + count) * 0.75f;
	pos.y = -1 * SCREEN_HEIGHT / 2 + 2 * g_Score.size * 0.85f;
	tw = 1.f / 5.f;
	th = 1.f / 5.f;

	do
	{
		_zanki = zanki % 10;
		u = (float)(_zanki % 5) / 5.0f;
		v = (float)(_zanki / 5) / 5.0f;
		DrawSpriteQuad(g_score, pos.x, pos.y, g_Score.size, g_Score.size, 1.f, 1.f, 1.f, 1.f, u, v, tw, th);
		pos.x -= g_Score.size * 0.75f;
		zanki /= 10;
	} while (zanki > 0);
	DrawSpriteQuad(g_player, pos.x, pos.y, g_Score.size * (43.75f / 71.f), g_Score.size, 1.f, 1.f, 1.f, 1.f, 36.5f / 663.f, 25.f / 410.f, 43.75f / 664.f, 70.f / 410.f);
}

void AddScore(int Count)
{
	g_Score.count += Count;
}

void AddPoint(int Count)
{
	g_Score.point += Count;
}

SCORE *getScore(void)
{
	return &g_Score;
}

void checkpointScore(void)
{
	g_Score.checkpoint = g_Score.point;
}

void respawnScore(void)
{
	g_Score.checkpoint = g_Score.checkpoint * 8 / 10;
	g_Score.point = g_Score.checkpoint;
}
