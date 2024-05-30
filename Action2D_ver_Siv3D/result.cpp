# include <Siv3D.hpp>
#include "Main.h"
#include "time.h"
#include "Sprite.h"
#include "player.h"
#include "score.h"
#include "result.h"
#include "sound.h"
#include "gimmick.h"

static AssetNameView
g_Backg = U"result_backg",
g_Gameover = U"result_gameover",
g_Clear = U"result_clear",
g_score = U"result_score",
g_coin = U"result_coin",
g_player = U"result_player",
g_toTitle = U"result_totitle";
static float score_size = 100.f;
static RESULT g_result = RESULT_NONE;
static RESULTSCORE g_resultscore;
static void DrawClearScore(void);

static GIMMICK Coin;

static void ResultPoint(void);
static void Score(void);
static void Zanki(void);
static void Total(void);

void InitResult(void)
{
	TextureAsset::Register(g_Backg, U"Action/Texture/backg.tga");
	TextureAsset::Register(g_Gameover, U"Action/Texture/gameover.tga");
	TextureAsset::Register(g_Clear, U"Action/Texture/Clear.tga");
	TextureAsset::Register(g_score, U"Action/Texture/number.tga");
	TextureAsset::Register(g_coin, U"Action/Texture/action_coin.tga");
	TextureAsset::Register(g_player, U"Action/Texture/action_ninja.tga");
	TextureAsset::Register(g_toTitle, U"Action/Texture/toTitle.tga");

	SetVolumeBGM(0.1);
	SetVolumeSE(0.05);

	PlayBGM(STRM_BLOOD_BLAST);

	SCORE *score = getScore();
	PLAYER *player = GetPlayer();
	g_resultscore.coin = score->count;
	g_resultscore.point = score->point;
	g_resultscore.zanki = player->Zanki;
	g_resultscore.total_score = (g_resultscore.point + g_resultscore.coin * 2) * (g_resultscore.zanki + 1);
	g_resultscore.display_coin = 0;
	g_resultscore.display_point = 0;
	g_resultscore.display_zanki = 0;
	g_resultscore.display_total_score = 0;

	Coin.TYPE = GIMMICK_TYPE_COIN;
	Coin.SIZE = score_size;
	Coin.Enable = true;
	Coin.cnt_anim = 0;
}

void UninitResult(void)
{
	TextureAsset::Release(g_Backg);
	TextureAsset::Release(g_Gameover);
	TextureAsset::Release(g_Clear);
	TextureAsset::Release(g_score);
	TextureAsset::Release(g_coin);
	TextureAsset::Release(g_player);
	TextureAsset::Release(g_toTitle);
	StopBGM();

	UninitScore();
	UninitPlayer();
	UninitGimmick();
}

void UpdateResult(void)
{
	PLAYER *player = GetPlayer();
	//ENEMY *boss = GetBoss();
	/*if ((player->Life > 0) && (boss->Life <= 0))
	{
		g_result = RESULT_GAMECLEAR;
	}
	else
	{
		g_result = RESULT_GAMEOVER;
	}*/
	if (g_result == RESULT_GAMEOVER)
	{
		if (MouseL.down())
		{
			_SetScene(SCENE_GAME);
		}
	}
	if (MouseR.down())
	{
		if (g_result == RESULT_GAMECLEAR)
		{
			PlaySE(WSD_MAN_YEAH);
		}
		_SetScene(SCENE_TITLE);
	}
	if (KeyQ.pressed()) System::Exit();
	if (g_resultscore.display_point != g_resultscore.point)
	{
		int diff = g_resultscore.point - g_resultscore.display_point;
		if (diff > 10 || diff < -10)
		{
			g_resultscore.display_point += (int)(diff * 0.1f);
		}
		else
		{
			if (diff > 0)
			{
				g_resultscore.display_point++;
			}
			else
			{
				g_resultscore.display_point--;
			}
		}
	}
	else if (g_resultscore.display_coin != g_resultscore.coin)
	{
		int diff = g_resultscore.coin - g_resultscore.display_coin;
		if (diff > 10 || diff < -10)
		{
			g_resultscore.display_coin += (int)(diff * 0.1f);
		}
		else
		{
			if (diff > 0)
			{
				g_resultscore.display_coin++;
			}
			else
			{
				g_resultscore.display_coin--;
			}
		}
	}
	else if (g_resultscore.display_zanki != g_resultscore.zanki)
	{
		int diff = g_resultscore.zanki - g_resultscore.display_zanki;
		if (diff > 10 || diff < -10)
		{
			g_resultscore.display_zanki += (int)(diff * 0.1f);
		}
		else
		{
			if (diff > 0)
			{
				g_resultscore.display_zanki++;
			}
			else
			{
				g_resultscore.display_zanki--;
			}
		}
	}
	else if (g_resultscore.display_total_score != g_resultscore.total_score)
	{
		int diff = g_resultscore.total_score - g_resultscore.display_total_score;
		if (diff > 10 || diff < -10)
		{
			g_resultscore.display_total_score += (int)(diff * 0.1f);
		}
		else
		{
			if (diff > 0)
			{
				g_resultscore.display_total_score++;
			}
			else
			{
				g_resultscore.display_total_score--;
			}
		}
	}
}

void DrawResult(void)
{
	DrawSpriteQuad(g_Backg, 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.0f, 1.0f);
	switch (g_result)
	{
	case RESULT_GAMEOVER:
		DrawSpriteQuad(g_Gameover, 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.0f, 1.0f);
		break;
	case RESULT_GAMECLEAR:
		DrawClearScore();
		break;
	default:
		break;
	}
}

void SetResult(RESULT result)
{
	g_result = result;
}

static void DrawClearScore(void)
{
	DrawSpriteQuad(g_Clear, 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.0f, 1.0f);
	DrawSpriteQuad(g_toTitle, 520.f, 390.f, 600.f, 300.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.0f, 1.0f);
	ResultPoint();
	Score();
	Zanki();
	Total();
}

static void ResultPoint(void)
{
	int point = g_resultscore.display_point, _point;
	Float2 pos;
	float tw, th, u, v;
	pos.x = -500.f;
	pos.y = 0.f;
	tw = 1.f / 5.f;
	th = 1.f / 5.f;

	do
	{
		_point = point % 10;
		u = (float)(_point % 5) / 5.0f;
		v = (float)(_point / 5) / 5.0f;
		DrawSpriteQuad(g_score, pos.x, pos.y, score_size, score_size, 1.f, 1.f, 1.f, 1.f, u, v, tw, th);
		pos.x -= score_size * 0.75f;
		point /= 10;
	} while (point > 0);
}

static void Score(void)
{
	int count = g_resultscore.display_coin, _count;
	Float2 pos;
	float tw, th, u, v;
	pos.x = 0.f;
	pos.y = 0.f;
	tw = 1.f / 5.f;
	th = 1.f / 5.f;

	do
	{
		_count = count % 10;
		u = (float)(_count % 5) / 5.0f;
		v = (float)(_count / 5) / 5.0f;
		DrawSpriteQuad(g_score, pos.x, pos.y, score_size, score_size, 1.f, 1.f, 1.f, 1.f, u, v, tw, th);
		pos.x -= score_size * 0.75f;
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

static void Zanki(void)
{
	int zanki = g_resultscore.display_zanki, _zanki, num = zanki, count = 0;

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
	pos.x = 500.f;
	pos.y = 0.f;
	tw = 1.f / 5.f;
	th = 1.f / 5.f;

	do
	{
		_zanki = zanki % 10;
		u = (float)(_zanki % 5) / 5.0f;
		v = (float)(_zanki / 5) / 5.0f;
		DrawSpriteQuad(g_score, pos.x, pos.y, score_size, score_size, 1.f, 1.f, 1.f, 1.f, u, v, tw, th);
		pos.x -= score_size * 0.75f;
		zanki /= 10;
	} while (zanki > 0);
	DrawSpriteQuad(g_player, pos.x, pos.y, score_size * (43.75f / 71.f), score_size, 1.f, 1.f, 1.f, 1.f, 36.5f / 663.f, 25.f / 410.f, 43.75f / 664.f, 70.f / 410.f);
}

static void Total(void)
{
	{
		int total_score = g_resultscore.display_total_score, _total_score;
		Float2 pos;
		float tw, th, u, v;
		float size = score_size * 1.5f;
		pos.x = 300.f;
		pos.y = 270.f;
		tw = 1.f / 5.f;
		th = 1.f / 5.f;

		do
		{
			_total_score = total_score % 10;
			u = (float)(_total_score % 5) / 5.0f;
			v = (float)(_total_score / 5) / 5.0f;
			DrawSpriteQuad(g_score, pos.x, pos.y, size, size, 1.f, 1.f, 1.f, 1.f, u, v, tw, th);
			pos.x -= size * 0.75f;
			total_score /= 10;
		} while (total_score > 0);
	}
}
