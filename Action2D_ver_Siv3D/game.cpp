# include <Siv3D.hpp>
#include "Main.h"
#include "sound.h"
#include "game.h"
#include "Sprite.h"
#include "player.h"
#include "action_enemy.h"
#include "enemybullet.h"
#include "background.h"
#include "block.h"
#include "gimmick.h"
#include "score.h"
#include "explosion.h"
#include "camera.h"
#include "lift.h"
#include "bullet.h"
#include "efect.h"
#include "pause.h"

static int old_player_point;
static STAGE_TYPE g_stage;

#define LEVEL_BLOCK (BLOCK_SIZE / 2)

static void InitFixedStage(void);

void InitGame(void)
{
	InitBG();
	InitPlayer();
	InitBullet();
	InitActionEnemy();
	InitEnemyBullet();
	InitGimmick();
	InitLift();
	InitBlock();
	InitEfect();
	InitExplosion();
	InitScore();
	InitPause();
	InitCamera();

	SetVolumeBGM(0.1);
	SetVolumeSE(0.05);

	//g_stage = RANDOM_STAGE;
	g_stage = FIXED_STAGE;
	if (g_stage == FIXED_STAGE)
	{
		//InitStage();
		InitFixedStage();
	}
	else if (g_stage == RANDOM_STAGE)
	{
		srand((unsigned int)time(NULL));
		InitRandomStage();
		old_player_point = 0;
	}

	PlayBGM(STRM_BO_TTO_HIDAMARI);
}

void UninitGame(void)
{
	UninitPause();
	UninitExplosion();
	UninitEfect();
	UninitLift();
	UninitBlock();
	UninitEnemyBullet();
	UninitActionEnemy();
	UninitBullet();
	UninitBG();

	StopBGM();
}

void UpdateGame(void)
{
	/*UpdatePlayer();*/
	if (!GetPause())
	{
		UpdateActionEnemy();
		UpdateEnemyBullet();
		UpdateGimmick();
		UpdateBlock();
		UpdateLift();
		UpdatePlayer();
		UpdateBullet();
		UpdateEfect();
		UpdateExplosion();
		UpdateScore();
		UpdateBG();
		UpdateCamera();
	}
	UpdatePause();

	if (g_stage == RANDOM_STAGE)
	{
		UpdateRandomStage();
	}
}

void DrawGame(void)
{
	DrawBG();
	DrawTutorial();
	DrawBullet();
	DrawActionEnemy();
	DrawEnemyBullet();
	DrawGimmick();
	DrawPlayer();
	DrawBlock();
	DrawLift();
	DrawEfect();
	DrawExplosion();
	DrawScore();
	DrawPause();
}


void InitStage(void)
{
	Float2 pos, pos_coin;
	int point = 0;
	pos_coin.x = 0.f;
	pos_coin.y = 0.f;

	CreateBlockRectangle(BLOCK_TYPE_SOIL, -SCREEN_WIDTH, SCREEN_HEIGHT / 2 - 2 * BLOCK_SIZE, LEVEL_BLOCK, LEVEL_BLOCK, 2 * SCREEN_WIDTH / BLOCK_SIZE - 3, 3);

	pos.x = (float)(point * RANDOM_STAGE_BLOCK);
	pos.y = SCREEN_HEIGHT / 2 - 2 * BLOCK_SIZE;
	//台 + 高さ2の足場
	pos.x += BLOCK_SIZE;
	CreateBlockRectangle(BLOCK_TYPE_SOIL, pos.x, pos.y, BLOCK_SIZE, BLOCK_SIZE, RANDOM_STAGE_BLOCK / BLOCK_SIZE - 3, 3);
	pos.x += 2 * BLOCK_SIZE;
	pos.y -= 3 * BLOCK_SIZE;
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 5, 2);

	point++;
	pos.x = (float)(point * RANDOM_STAGE_BLOCK);
	pos.y = SCREEN_HEIGHT / 2 - 2 * BLOCK_SIZE;
	//氷ブロック 上り下り階段付きの台
	CreateBlockStairs(BLOCK_TYPE_ICE, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 2, STAIRS_UP);
	pos.x += BLOCK_SIZE;
	pos.y -= BLOCK_SIZE;
	CreateBlockRectangle(BLOCK_TYPE_ICE, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 9, 3);
	pos_coin.x = pos.x + LEVEL_BLOCK;
	pos_coin.y = pos.y - 3 * LEVEL_BLOCK;
	CreateGimmickRectangle(GIMMICK_TYPE_COIN, pos_coin.x, pos_coin.y, GIMMICK_SIZE, 7, 1);
	pos.x += 9 * LEVEL_BLOCK;
	pos.y += BLOCK_SIZE;
	CreateBlockStairs(BLOCK_TYPE_ICE, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 2, STAIRS_DOWN);

	point++;
	pos.x = (float)(point * RANDOM_STAGE_BLOCK);
	pos.y = SCREEN_HEIGHT / 2 - 2 * BLOCK_SIZE;
	//氷ブロック 上側左に上り階段付きの台　下側右に下り階段付きの台
	pos.y -= 3 * LEVEL_BLOCK;
	CreateBlockRectangle(BLOCK_TYPE_ICE, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 10, 3);
	pos.x += BLOCK_SIZE * 5;
	pos.y += BLOCK_SIZE;
	CreateBlockStairs(BLOCK_TYPE_ICE, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 2, STAIRS_DOWN);
	pos.x -= BLOCK_SIZE * 5;
	pos.y -= 7 * LEVEL_BLOCK;
	CreateBlockStairs(BLOCK_TYPE_ICE, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 2, STAIRS_UP);
	pos.x += BLOCK_SIZE;
	pos.y -= BLOCK_SIZE;
	CreateBlockRectangle(BLOCK_TYPE_ICE, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 10, 3);

	point++;
	pos.x = (float)(point * RANDOM_STAGE_BLOCK);
	pos.y = SCREEN_HEIGHT / 2 - 2 * BLOCK_SIZE;
	//ブロックの柱と真ん中にリフト
	pos.y -= BLOCK_SIZE * 2;
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, BLOCK_SIZE, BLOCK_SIZE, 1, 3);
	pos_coin.x = pos.x + 2.25 * BLOCK_SIZE;
	pos_coin.y = pos.y - 1.5 * BLOCK_SIZE;
	CreateGimmick(GIMMICK_TYPE_COIN, pos_coin.x, pos_coin.y, GIMMICK_SIZE, false);
	CreateLift(LIFT_TYPE_NORMAL, pos_coin.x, 0.f, LIFT_SIZE_X, LIFT_SIZE_Y, 0.f, SCREEN_HEIGHT / 4, 0);
	pos.x += 4.5 * BLOCK_SIZE;
	pos.y -= BLOCK_SIZE * 2;
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, BLOCK_SIZE, BLOCK_SIZE, 1, 5);

	point++;
	pos.x = (float)(point * RANDOM_STAGE_BLOCK);
	pos.y = SCREEN_HEIGHT / 2 - 2 * BLOCK_SIZE;
	//台 + 高さ1の足場2個
	CreateBlockRectangle(BLOCK_TYPE_SOIL, pos.x, pos.y, BLOCK_SIZE, BLOCK_SIZE, RANDOM_STAGE_BLOCK / BLOCK_SIZE - 3, 3);
	pos.x += 2 * BLOCK_SIZE;
	pos.y -= 2.5 * BLOCK_SIZE;
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 5, 1);
	pos.y -= 2.5 * BLOCK_SIZE;
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 5, 1);

	point++;
	pos.x = (float)(point * RANDOM_STAGE_BLOCK);
	pos.y = SCREEN_HEIGHT / 2 - 2 * BLOCK_SIZE;
	//台 + ブロックが左右に1個ずつ
	pos.x += BLOCK_SIZE;
	pos.y -= 3 * BLOCK_SIZE;
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, BLOCK_SIZE, LEVEL_BLOCK, 6, 2);
	pos.x -= BLOCK_SIZE;
	pos.y += 2 * BLOCK_SIZE;
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 1, 1);
	pos_coin.x = pos.x;
	pos_coin.y = pos.y - BLOCK_SIZE;
	CreateGimmick(GIMMICK_TYPE_COIN, pos_coin.x, pos_coin.y, GIMMICK_SIZE, false);
	pos.x += BLOCK_SIZE * 7;
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 1, 1);
	pos_coin.x = pos.x;
	pos_coin.y = pos.y - BLOCK_SIZE;
	CreateGimmick(GIMMICK_TYPE_COIN, pos_coin.x, pos_coin.y, GIMMICK_SIZE, false);

	point++;
	pos.x = (float)(point * RANDOM_STAGE_BLOCK);
	pos.y = SCREEN_HEIGHT / 2 - 2 * BLOCK_SIZE;
	//かなり高さの違う2つの台
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, (RANDOM_STAGE_BLOCK / BLOCK_SIZE - 2) * 2, 2);
	pos.y -= 5 * BLOCK_SIZE;
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, (RANDOM_STAGE_BLOCK / BLOCK_SIZE - 2) * 2, 1);
	pos_coin.x = pos.x;
	pos_coin.y = pos.y - 2 * BLOCK_SIZE;
	CreateGimmickRectangle(GIMMICK_TYPE_COIN, pos_coin.x, pos_coin.y, GIMMICK_SIZE, (RANDOM_STAGE_BLOCK / BLOCK_SIZE - 2) * 2, 1);

	point++;
	pos.x = (float)(point * RANDOM_STAGE_BLOCK);
	pos.y = SCREEN_HEIGHT / 2 - 2 * BLOCK_SIZE;
	//壁で囲んだ島
	CreateBlockRectangle(BLOCK_TYPE_SOIL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, (RANDOM_STAGE_BLOCK / BLOCK_SIZE - 4) * 2, 3);
	pos.y -= 5 * LEVEL_BLOCK;
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 1, 5);
	pos.x += 5 * BLOCK_SIZE;
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 1, 5);

	point++;
	pos.x = (float)(point * RANDOM_STAGE_BLOCK);
	pos.y = SCREEN_HEIGHT / 2 - 2 * BLOCK_SIZE;
	//幅1ブロックの柱が4本
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 1, 1);
	pos.x += 2 * BLOCK_SIZE;
	pos.y -= BLOCK_SIZE;
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 1, 3);
	pos.x += 2 * BLOCK_SIZE;
	pos.y -= BLOCK_SIZE;
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 1, 5);
	pos.x += 2 * BLOCK_SIZE;
	pos.y -= BLOCK_SIZE;
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 1, 7);

	point++;
	pos.x = (float)(point * RANDOM_STAGE_BLOCK);
	pos.y = SCREEN_HEIGHT / 2 - 2 * BLOCK_SIZE;
	//入れ違いまくるリフト
	CreateLift(LIFT_TYPE_NORMAL, pos.x, 0.f, LIFT_SIZE_X, LIFT_SIZE_Y, 0.f, SCREEN_HEIGHT / 4, 0);
	pos.x += 3 * BLOCK_SIZE;
	pos.y -= 3 * BLOCK_SIZE;
	CreateLift(LIFT_TYPE_NORMAL, pos.x, pos.y, LIFT_SIZE_X, LIFT_SIZE_Y, BLOCK_SIZE, -100.f, 0);
	pos.x += 3 * BLOCK_SIZE;
	pos.y += 2 * BLOCK_SIZE;
	CreateLift(LIFT_TYPE_NORMAL, pos.x, pos.y, LIFT_SIZE_X, LIFT_SIZE_Y, BLOCK_SIZE, 2 * BLOCK_SIZE, 0);
	pos.x -= 4 * BLOCK_SIZE;
	pos.y += LEVEL_BLOCK;
	CreateLift(LIFT_TYPE_NORMAL, pos.x, pos.y, LIFT_SIZE_X, LIFT_SIZE_Y, 4 * BLOCK_SIZE, 0.f, 0);

	point++;
	pos.x = (float)(point * RANDOM_STAGE_BLOCK);
	pos.y = SCREEN_HEIGHT / 2 - 2 * BLOCK_SIZE;
	//足場と巨大階段
	CreateBlockRectangle(BLOCK_TYPE_SOIL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 2 * RANDOM_STAGE_BLOCK / BLOCK_SIZE - 3, 3);
	pos.y -= LEVEL_BLOCK;
	CreateBlockStairs(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 9, STAIRS_UP);
	pos.x += BLOCK_SIZE * 5;
	CreateBlockStairs(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 6, STAIRS_DOWN);

	point++;
	pos.x = (float)(point * RANDOM_STAGE_BLOCK);
	pos.y = SCREEN_HEIGHT / 2 - 2 * BLOCK_SIZE;
	//足場と上に上るための足場
	CreateBlockRectangle(BLOCK_TYPE_SOIL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 2 * RANDOM_STAGE_BLOCK / BLOCK_SIZE - 3, 3);
	pos.x += BLOCK_SIZE;
	pos.y -= 8 * BLOCK_SIZE;
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 1, 11);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x + BLOCK_SIZE * 6, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 1, 11);
	pos.x += LEVEL_BLOCK;
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 2, 1);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y + BLOCK_SIZE * 3, LEVEL_BLOCK, LEVEL_BLOCK, 2, 1);
	pos.x += BLOCK_SIZE * 4.5;
	pos.y += BLOCK_SIZE * 2;
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 2, 1);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y + BLOCK_SIZE * 3, LEVEL_BLOCK, LEVEL_BLOCK, 2, 1);

	point++;
	pos.x = (float)(point * RANDOM_STAGE_BLOCK);
	pos.y = SCREEN_HEIGHT / 2 - 2 * BLOCK_SIZE;
	//段々な足場とそれぞれに敵
	CreateBlockRectangle(BLOCK_TYPE_SOIL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 2 * RANDOM_STAGE_BLOCK / BLOCK_SIZE - 3, 3);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y - BLOCK_SIZE * 1.5, LEVEL_BLOCK, LEVEL_BLOCK, 1, 1);
	pos.x += BLOCK_SIZE;
	pos.y -= BLOCK_SIZE * 1.5;
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 3, 1);
	CreateActionEnemy(ENEMY_TYPE_2, pos.x, pos.y - BLOCK_SIZE, 20, VEL_LEFT, false);
	pos.x += BLOCK_SIZE * 2;
	pos.y -= BLOCK_SIZE * 1.5;
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 3, 1);
	CreateActionEnemy(ENEMY_TYPE_2, pos.x, pos.y - BLOCK_SIZE, 20, VEL_LEFT, false);
	pos.x += BLOCK_SIZE * 2;
	pos.y -= BLOCK_SIZE * 1.5;
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 3, 1);
	CreateActionEnemy(ENEMY_TYPE_2, pos.x, pos.y - BLOCK_SIZE, 20, VEL_LEFT, false);
	pos.x += BLOCK_SIZE * 2;
	pos.y -= BLOCK_SIZE * 1.5;
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 3, 1);
	CreateActionEnemy(ENEMY_TYPE_2, pos.x, pos.y - BLOCK_SIZE, 20, VEL_LEFT, false);
}

void InitRandomStage(void)
{
	CreateBlockRectangle(BLOCK_TYPE_SOIL, -SCREEN_WIDTH, SCREEN_HEIGHT / 2 - 2 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, SCREEN_WIDTH / BLOCK_SIZE - 3, 3);
	//CreateActionEnemy(ENEMY_TYPE_1, -2 * BLOCK_SIZE, -BLOCK_SIZE);
	//画面4個分までステージ生成
	for (int i = 0; i < 8; i++)
	{
		CreateRandomStageBlock(i);
		CreateEnemyOnRandomPoint(i);
	}
}

void UpdateRandomStage(void)
{
	BLOCK *g_Block = GetBlock();
	int player_point = GetPlayerStageBlock();

	for (int i = 0; i < BLOCK_MAX; i++)
	{
		if (g_Block[i].Enable == true)
		{
			//画面10個分離れたら消去するコード
			if (((player_point - g_Block[i].Position.x / RANDOM_STAGE_BLOCK) > 20) || ((player_point - g_Block[i].Position.x / RANDOM_STAGE_BLOCK) < -10))
			{
				DeleteBlock(i);
			}
		}
	}
	//画面4個分先のステージをランダムに生成するコード
	if (old_player_point < player_point)
	{
		CreateRandomStageBlock(player_point + 7);
		CreateEnemyOnRandomPoint(player_point + 7);
		old_player_point = player_point;
	}
}

void CreateRandomStageBlock(int point)
{
	//1ブロック分生成
	Float2 pos, pos_coin, pos_enemy;
	pos.x = (float)(point * RANDOM_STAGE_BLOCK);
	pos.y = SCREEN_HEIGHT / 2 - 2 * BLOCK_SIZE;
	pos_coin.x = 0.f;
	pos_coin.y = 0.f;
	pos_enemy.x = 0.f;
	pos_enemy.y = 0.f;
	int check = rand() % 10;
	switch (check)
	{
	case 0:
		pos.x += BLOCK_SIZE;
		CreateBlockRectangle(BLOCK_TYPE_SOIL, pos.x, pos.y, BLOCK_SIZE, BLOCK_SIZE, RANDOM_STAGE_BLOCK / BLOCK_SIZE - 3, 3);
		pos.x += 2 * BLOCK_SIZE;
		pos.y -= 3 * BLOCK_SIZE;
		CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 5, 2);
		break;
	case 1:
		CreateBlockStairs(BLOCK_TYPE_ICE, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 2, STAIRS_UP);
		pos.x += BLOCK_SIZE;
		pos.y -= BLOCK_SIZE;
		CreateBlockRectangle(BLOCK_TYPE_ICE, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 9, 3);
		pos_coin.x = pos.x + LEVEL_BLOCK;
		pos_coin.y = pos.y - BLOCK_SIZE * 3 / 2;
		CreateGimmickRectangle(GIMMICK_TYPE_COIN, pos_coin.x, pos_coin.y, GIMMICK_SIZE, 7, 1);
		pos.x += BLOCK_SIZE * 9 / 2;
		pos.y += BLOCK_SIZE;
		CreateBlockStairs(BLOCK_TYPE_ICE, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 2, STAIRS_DOWN);
		break;
	case 2:
		pos.y -= BLOCK_SIZE * 3 / 2;
		CreateBlockRectangle(BLOCK_TYPE_ICE, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 10, 3);
		pos.x += BLOCK_SIZE * 5;
		pos.y += BLOCK_SIZE;
		CreateBlockStairs(BLOCK_TYPE_ICE, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 2, STAIRS_DOWN);
		pos_enemy.x = pos.x - BLOCK_SIZE;
		pos_enemy.y = pos.y - BLOCK_SIZE;
		CreateActionEnemy(ENEMY_TYPE_1, pos_enemy.x, pos_enemy.y, 10, VEL_LEFT, false);
		pos.x -= BLOCK_SIZE * 5;
		pos.y -= BLOCK_SIZE * 7 / 2;
		CreateBlockStairs(BLOCK_TYPE_ICE, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 2, STAIRS_UP);
		pos.x += BLOCK_SIZE;
		pos.y -= BLOCK_SIZE;
		CreateBlockRectangle(BLOCK_TYPE_ICE, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 10, 3);
		break;
	case 3:
		pos.y -= BLOCK_SIZE * 2;
		CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, BLOCK_SIZE, BLOCK_SIZE, 1, 3);
		pos_coin.x = pos.x + 2.25 * BLOCK_SIZE;
		pos_coin.y = pos.y - 1.5 * BLOCK_SIZE;
		CreateGimmick(GIMMICK_TYPE_COIN, pos_coin.x, pos_coin.y, GIMMICK_SIZE, false);
		CreateLift(LIFT_TYPE_NORMAL, pos_coin.x, 0.f, LIFT_SIZE_X, LIFT_SIZE_Y, 0.f, SCREEN_HEIGHT / 4, 0);
		pos.x += 4.5 * BLOCK_SIZE;
		pos.y -= BLOCK_SIZE * 2;
		CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, BLOCK_SIZE, BLOCK_SIZE, 1, 5);
		break;
	case 4:
		CreateBlockRectangle(BLOCK_TYPE_SOIL, pos.x, pos.y, BLOCK_SIZE, BLOCK_SIZE, RANDOM_STAGE_BLOCK / BLOCK_SIZE - 3, 3);
		pos.x += 2 * BLOCK_SIZE;
		pos.y -= 2.5 * BLOCK_SIZE;
		CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 5, 1);
		pos.y -= 2.5 * BLOCK_SIZE;
		CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 5, 1);
		pos_enemy.x = pos.x + 2 * BLOCK_SIZE;
		pos_enemy.x = pos.y - BLOCK_SIZE;
		CreateActionEnemy(ENEMY_TYPE_1, pos_enemy.x, pos_enemy.x, 10, VEL_LEFT, false);
		break;
	case 5:
		pos.x += BLOCK_SIZE;
		pos.y -= 3 * BLOCK_SIZE;
		CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, BLOCK_SIZE, LEVEL_BLOCK, 6, 2);
		pos.x -= BLOCK_SIZE;
		pos.y += 2 * BLOCK_SIZE;
		CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 1, 1);
		pos_coin.x = pos.x;
		pos_coin.y = pos.y - BLOCK_SIZE;
		CreateGimmick(GIMMICK_TYPE_COIN, pos_coin.x, pos_coin.y, GIMMICK_SIZE, false);
		pos.x += BLOCK_SIZE * 7;
		CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 1, 1);
		pos_coin.x = pos.x;
		pos_coin.y = pos.y - BLOCK_SIZE;
		CreateGimmick(GIMMICK_TYPE_COIN, pos_coin.x, pos_coin.y, GIMMICK_SIZE, false);
		break;
	case 6:
		CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, (RANDOM_STAGE_BLOCK / BLOCK_SIZE - 2) * 2, 2);
		pos.y -= 5 * BLOCK_SIZE;
		CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, (RANDOM_STAGE_BLOCK / BLOCK_SIZE - 2) * 2, 1);
		pos_coin.x = pos.x;
		pos_coin.y = pos.y - 2 * BLOCK_SIZE;
		CreateGimmickRectangle(GIMMICK_TYPE_COIN, pos_coin.x, pos_coin.y, GIMMICK_SIZE, (RANDOM_STAGE_BLOCK / BLOCK_SIZE - 2) * 2, 1);
		break;
	case 7:
		CreateBlockRectangle(BLOCK_TYPE_SOIL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, (RANDOM_STAGE_BLOCK / BLOCK_SIZE - 4) * 2, 3);
		pos.y -= BLOCK_SIZE * 5 / 2;
		CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 1, 5);
		pos.x += 5 * BLOCK_SIZE;
		CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 1, 5);
		pos_enemy.x = pos.x - BLOCK_SIZE;
		pos_enemy.y = pos.y - BLOCK_SIZE;
		CreateActionEnemy(ENEMY_TYPE_1, pos_enemy.x, pos_enemy.y, 10, VEL_LEFT, false);
		pos_enemy.x = pos_enemy.x - 2 * BLOCK_SIZE;
		CreateActionEnemy(ENEMY_TYPE_1, pos_enemy.x, pos_enemy.y, 10, VEL_LEFT, false);
		break;
	case 8:
		CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 1, 1);
		pos.x += 2 * BLOCK_SIZE;
		pos.y -= BLOCK_SIZE;
		CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 1, 3);
		pos.x += 2 * BLOCK_SIZE;
		pos.y -= BLOCK_SIZE;
		CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 1, 5);
		pos.x += 2 * BLOCK_SIZE;
		pos.y -= BLOCK_SIZE;
		CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 1, 7);
		break;
	case 9:
		CreateLift(LIFT_TYPE_NORMAL, pos.x, 0.f, LIFT_SIZE_X, LIFT_SIZE_Y, 0.f, SCREEN_HEIGHT / 4, 0);
		pos.x += 3 * BLOCK_SIZE;
		pos.y -= 3 * BLOCK_SIZE;
		CreateLift(LIFT_TYPE_NORMAL, pos.x, pos.y, LIFT_SIZE_X, LIFT_SIZE_Y, BLOCK_SIZE, -100.f, 0);
		pos.x += 3 * BLOCK_SIZE;
		pos.y += 2 * BLOCK_SIZE;
		CreateLift(LIFT_TYPE_NORMAL, pos.x, pos.y, LIFT_SIZE_X, LIFT_SIZE_Y, BLOCK_SIZE, 2 * BLOCK_SIZE, 0);
		pos.x -= 4 * BLOCK_SIZE;
		pos.y += LEVEL_BLOCK;
		CreateLift(LIFT_TYPE_NORMAL, pos.x, pos.y, LIFT_SIZE_X, LIFT_SIZE_Y, 4 * BLOCK_SIZE, 0.f, 0);
		break;
	}
}

void CreateEnemyOnRandomPoint(int point)
{
	int i = rand() % 3;
	if (i == 0)
	{
		Float2 pos_enemy;
		pos_enemy.x = (float)(point * RANDOM_STAGE_BLOCK) + 4 * BLOCK_SIZE;
		pos_enemy.y = -0.5 * SCREEN_HEIGHT;
		CreateActionEnemy(ENEMY_TYPE_2, pos_enemy.x, pos_enemy.y, 20, VEL_LEFT, false);
	}
}

/*
soil
0	0	69*2
71	0	15*2
89	0	64*2
155	0	45*2
*/
/*
normal
17	5	1
21	5	5*1
23	10	1
77	5	3*1
80	10	8*1
91	10	4*1
94	5	1
100	5	2*1
106	5	1
108	5	1
108	10	1
110	5	1
116	5	1
119	10	3*1
126	10	4*1
127	5	2*1
163	2	2*2
168	5	4*1
179 2	2*2
181	9	stairs ip
190	9	1*9
*/
/*
ice
29	2	2*3
39	4	2*5
47	6	2*8
58	7	2*8
132	4	stairs up
138	4	stairs down
148 5	stairs up
155 4	stairs down
*/

static void InitFixedStage(void)
{
	Float2 pos;
	pos.x = -SCREEN_WIDTH / 4;
	pos.y = SCREEN_HEIGHT / 2 - 2 * BLOCK_SIZE;
	CreateBlockRectangle(BLOCK_TYPE_SOIL, -SCREEN_WIDTH, SCREEN_HEIGHT / 2 - 2 * BLOCK_SIZE, LEVEL_BLOCK, LEVEL_BLOCK, (SCREEN_WIDTH * 3 / 4) / (LEVEL_BLOCK) + 1, 2);

	/*
	soil
	0	0	69*2
	71	0	15*2
	89	0	64*2
	155	0	45*2
	*/
	CreateBlockRectangle(BLOCK_TYPE_SOIL, pos.x, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 68, 2);
	CreateBlockRectangle(BLOCK_TYPE_SOIL, pos.x + 94 * LEVEL_BLOCK, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 59, 2);
	CreateBlockRectangle(BLOCK_TYPE_SOIL, pos.x + 155 * LEVEL_BLOCK, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 15, 2);
	CreateBlockRectangle(BLOCK_TYPE_SOIL, pos.x + 224 * LEVEL_BLOCK, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 23, 2);
	CreateBlockRectangle(BLOCK_TYPE_SOIL, pos.x + 192 * LEVEL_BLOCK, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 5, 2);
	
	/*
	normal
	17	5	1
	21	5	5*1
	23	10	1
	77	5	3*1
	80	10	8*1
	91	10	4*1
	94	5	1
	100	5	2*1
	106	5	1
	108	5	1
	108	10	1
	110	5	1
	116	5	1
	119	10	3*1
	126	10	4*1
	127	5	2*1
	163	2	2*2
	168	5	4*1
	181	9	stairs ip
	190	9	1*9
	*/
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x + 17 * LEVEL_BLOCK, pos.y - 5 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 1, 1);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x + 21 * LEVEL_BLOCK, pos.y - 5 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 5, 1);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x + 23 * LEVEL_BLOCK, pos.y - 10 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 1, 1);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x + 29 * LEVEL_BLOCK, pos.y - 2 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 2, 2);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x + 39 * LEVEL_BLOCK, pos.y - 4 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 2, 4);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x + 47 * LEVEL_BLOCK, pos.y - 6 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 2, 6);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x + 58 * LEVEL_BLOCK, pos.y - 8 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 2, 8);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x + 80 * LEVEL_BLOCK, pos.y - 10 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 8, 1);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x + 91 * LEVEL_BLOCK, pos.y - 10 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 4, 1);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x + 94 * LEVEL_BLOCK, pos.y - 5 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 1, 1);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x + 100 * LEVEL_BLOCK, pos.y - 5 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 2, 1);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x + 106 * LEVEL_BLOCK, pos.y - 5 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 1, 1);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x + 108 * LEVEL_BLOCK, pos.y - 5 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 1, 1);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x + 108 * LEVEL_BLOCK, pos.y - 10 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 1, 1);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x + 110 * LEVEL_BLOCK, pos.y - 5 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 1, 1);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x + 116 * LEVEL_BLOCK, pos.y - 5 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 1, 1);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x + 119 * LEVEL_BLOCK, pos.y - 10 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 3, 1);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x + 126 * LEVEL_BLOCK, pos.y - 10 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 4, 1);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x + 134 * LEVEL_BLOCK, pos.y - 15 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 2, 1);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x + 127 * LEVEL_BLOCK, pos.y - 5 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 2, 1);
	CreateBlockStairs(BLOCK_TYPE_NORMAL, pos.x + 132 * LEVEL_BLOCK, pos.y - 1 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 4, STAIRS_UP);
	CreateBlockStairs(BLOCK_TYPE_NORMAL, pos.x + 138 * LEVEL_BLOCK, pos.y - 1 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 4, STAIRS_DOWN);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x + 163 * LEVEL_BLOCK, pos.y - 2 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 2, 2);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x + 168 * LEVEL_BLOCK, pos.y - 5 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 4, 1);
	CreateBlockStairs(BLOCK_TYPE_NORMAL, pos.x + 224 * LEVEL_BLOCK, pos.y - 1 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 9, STAIRS_UP);
	CreateBlockRectangle(BLOCK_TYPE_NORMAL, pos.x + 233 * LEVEL_BLOCK, pos.y - 9 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 1, 9);

	/*
	ice
	29	2	2*2
	39	4	2*4
	47	6	2*6
	58	7	2*8
	132	4	stairs up
	138	4	stairs down
	148 5	stairs up
	155 4	stairs down
	*/

	CreateBlockRectangle(BLOCK_TYPE_ICE, pos.x + 71 * LEVEL_BLOCK, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 15, 2);
	CreateBlockRectangle(BLOCK_TYPE_ICE, pos.x + 77 * LEVEL_BLOCK, pos.y - 5 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 3, 1);
	CreateBlockStairs(BLOCK_TYPE_ICE, pos.x + 148 * LEVEL_BLOCK, pos.y - 1 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 4, STAIRS_UP);
	CreateBlockRectangle(BLOCK_TYPE_ICE, pos.x + 152 * LEVEL_BLOCK, pos.y - 4 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 1, 4);
	CreateBlockStairs(BLOCK_TYPE_ICE, pos.x + 155 * LEVEL_BLOCK, pos.y - 1 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 4, STAIRS_DOWN);
	CreateBlockRectangle(BLOCK_TYPE_ICE, pos.x + 222 * LEVEL_BLOCK, pos.y - 2 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 2, 2);
	CreateBlockRectangle(BLOCK_TYPE_ICE, pos.x + 200 * LEVEL_BLOCK, pos.y, LEVEL_BLOCK, LEVEL_BLOCK, 24, 2);
	CreateBlockRectangle(BLOCK_TYPE_ICE, pos.x + 200 * LEVEL_BLOCK, pos.y - 5 * LEVEL_BLOCK, LEVEL_BLOCK, LEVEL_BLOCK, 1, 5);

	/*
	lift
	
	*/
	CreateLift(LIFT_TYPE_NORMAL, pos.x + 174 * LEVEL_BLOCK, pos.y - 4 * LEVEL_BLOCK, LIFT_SIZE_X, LIFT_SIZE_Y, 0.f, BLOCK_SIZE * 3, 0);
	CreateLift(LIFT_TYPE_NORMAL, pos.x + 178 * LEVEL_BLOCK, pos.y - 4 * LEVEL_BLOCK, LIFT_SIZE_X, LIFT_SIZE_Y, 0.f, BLOCK_SIZE * 3, 90);
	CreateLift(LIFT_TYPE_NORMAL, pos.x + 183 * LEVEL_BLOCK, pos.y - 4 * LEVEL_BLOCK, LIFT_SIZE_X, LIFT_SIZE_Y, 0.f, BLOCK_SIZE * 3, 180);
	CreateLift(LIFT_TYPE_NORMAL, pos.x + 188 * LEVEL_BLOCK, pos.y - 4 * LEVEL_BLOCK, LIFT_SIZE_X, LIFT_SIZE_Y, 0.f, BLOCK_SIZE * 3, 270);
	CreateLift(LIFT_TYPE_NORMAL, pos.x + 238.5 * LEVEL_BLOCK, pos.y - 3 * LEVEL_BLOCK, LIFT_SIZE_X, LIFT_SIZE_Y, LEVEL_BLOCK * 3.5, 0.f, 270);
	
	/*
	Enemy
	*/
	CreateActionEnemy(ENEMY_TYPE_1, pos.x + 23 * LEVEL_BLOCK, pos.y - 2 * LEVEL_BLOCK, 10, VEL_LEFT, false);
	CreateActionEnemy(ENEMY_TYPE_1, pos.x + 108 * LEVEL_BLOCK, pos.y - 2 * LEVEL_BLOCK, 10, VEL_LEFT, false);
	CreateActionEnemy(ENEMY_TYPE_1, pos.x + 110 * LEVEL_BLOCK, pos.y - 2 * LEVEL_BLOCK, 10, VEL_LEFT, false);
	CreateActionEnemy(ENEMY_TYPE_3, pos.x + 120 * LEVEL_BLOCK, pos.y - 12 * LEVEL_BLOCK, 20, VEL_LEFT, false);
	CreateActionEnemy(ENEMY_TYPE_2, pos.x + 124 * LEVEL_BLOCK, pos.y - 2 * LEVEL_BLOCK, 20, VEL_LEFT, false);
	CreateActionEnemy(ENEMY_TYPE_2, pos.x + 174 * LEVEL_BLOCK, pos.y - 11 * LEVEL_BLOCK, 20, VEL_LEFT, false);
	CreateActionEnemy(ENEMY_TYPE_2, pos.x + 178 * LEVEL_BLOCK, pos.y - 11 * LEVEL_BLOCK, 20, VEL_LEFT, false);
	CreateActionEnemy(ENEMY_TYPE_2, pos.x + 183 * LEVEL_BLOCK, pos.y - 11 * LEVEL_BLOCK, 20, VEL_LEFT, false);
	CreateActionEnemy(ENEMY_TYPE_2, pos.x + 188 * LEVEL_BLOCK, pos.y - 11 * LEVEL_BLOCK, 20, VEL_LEFT, false);
	CreateActionEnemy(ENEMY_TYPE_3, pos.x + 218 * LEVEL_BLOCK, pos.y - 3 * LEVEL_BLOCK, 80, VEL_LEFT, true);
	/*
	gimmick
	23	10	1
	80	10	8*1
	91	10	4*1
	108	10	1
	119	10	3*1
	126	10	4*1
	*/
	CreateGimmickRectangle(GIMMICK_TYPE_COIN, pos.x + 23 * LEVEL_BLOCK, pos.y - 12 * LEVEL_BLOCK, GIMMICK_SIZE, 1, 1);
	CreateGimmickRectangle(GIMMICK_TYPE_COIN, pos.x + 84 * LEVEL_BLOCK, pos.y - 5 * LEVEL_BLOCK, GIMMICK_SIZE, 1, 5);
	CreateGimmickRectangle(GIMMICK_TYPE_COIN, pos.x + 91 * LEVEL_BLOCK, pos.y - 12 * LEVEL_BLOCK, GIMMICK_SIZE, 4, 1);
	CreateGimmickRectangle(GIMMICK_TYPE_COIN, pos.x + 108 * LEVEL_BLOCK, pos.y - 12 * LEVEL_BLOCK, GIMMICK_SIZE, 1, 1);
	CreateGimmickRectangle(GIMMICK_TYPE_COIN, pos.x + 126 * LEVEL_BLOCK, pos.y - 12 * LEVEL_BLOCK, GIMMICK_SIZE, 4, 1);
	CreateGimmickRectangle(GIMMICK_TYPE_COIN, pos.x + 176 * LEVEL_BLOCK, pos.y - 6 * LEVEL_BLOCK, GIMMICK_SIZE, 1, 1);
	CreateGimmickRectangle(GIMMICK_TYPE_COIN, pos.x + 180 * LEVEL_BLOCK, pos.y - 6 * LEVEL_BLOCK, GIMMICK_SIZE, 1, 1);
	CreateGimmickRectangle(GIMMICK_TYPE_COIN, pos.x + 185 * LEVEL_BLOCK, pos.y - 6 * LEVEL_BLOCK, GIMMICK_SIZE, 1, 1);
	CreateGimmickRectangle(GIMMICK_TYPE_COIN, pos.x + 245 * LEVEL_BLOCK, pos.y - 9 * LEVEL_BLOCK, GIMMICK_SIZE, 2, 9);
	/*
	spike
	*/
	CreateGimmickRectangle(GIMMICK_TYPE_SPIKE, pos.x + 86 * LEVEL_BLOCK, pos.y - 5 * LEVEL_BLOCK, TRAP_SIZE, 1, 5);
	CreateGimmickRectangle(GIMMICK_TYPE_SPIKE, pos.x + 221 * LEVEL_BLOCK, pos.y - 2 * LEVEL_BLOCK, TRAP_SIZE, 1, 2);
	CreateGimmickRectangle(GIMMICK_TYPE_SPIKE, pos.x + 234 * LEVEL_BLOCK, pos.y - 1 * LEVEL_BLOCK, TRAP_SIZE, 11, 1);
	CreateGimmickRectangle(GIMMICK_TYPE_SPIKE, pos.x + 247 * LEVEL_BLOCK, pos.y - 1 * LEVEL_BLOCK, TRAP_SIZE, 1, 1);
	/*
	check point
	*/
	CreateGimmickRectangle(GIMMICK_TYPE_CHECK_POINT, pos.x + 84 * LEVEL_BLOCK, pos.y - 11 * LEVEL_BLOCK, BLOCK_SIZE, 1, 1);
	CreateGimmickRectangle(GIMMICK_TYPE_CHECK_POINT, pos.x + 194 * LEVEL_BLOCK, pos.y - 1 * LEVEL_BLOCK, BLOCK_SIZE, 1, 1);
	/*
	bounce ball
	*/
	CreateGimmickRectangle(GIMMICK_TYPE_BOUNCE, pos.x + 74 * LEVEL_BLOCK, pos.y - 5 * LEVEL_BLOCK, BLOCK_SIZE, 1, 1);
	CreateGimmickRectangle(GIMMICK_TYPE_BOUNCE, pos.x + 145 * LEVEL_BLOCK, pos.y - 7 * LEVEL_BLOCK, BLOCK_SIZE, 1, 1);
	CreateGimmickRectangle(GIMMICK_TYPE_BOUNCE, pos.x + 160 * LEVEL_BLOCK, pos.y - 7 * LEVEL_BLOCK, BLOCK_SIZE, 1, 1);
	/*
	key
	*/
	//CreateGimmickRectangle(GIMMICK_TYPE_KEY, pos.x + 127.5 * LEVEL_BLOCK, pos.y - 7 * LEVEL_BLOCK, 2 * GIMMICK_SIZE, 1, 1);
	/*
	door
	*/
	//CreateGimmickRectangle(GIMMICK_TYPE_DOOR, pos.x + 241 * LEVEL_BLOCK, pos.y - 1.2 * LEVEL_BLOCK, BLOCK_SIZE, 1, 1);
	CreateGimmickRectangle(GIMMICK_TYPE_DOOR, pos.x + 134.5 * LEVEL_BLOCK, pos.y - 16.2 * LEVEL_BLOCK, BLOCK_SIZE, 1, 1);
	/*
	tutorial
	*/
	CreateTutorial(TUTORIAL_TYPE_1, pos.x - 15 * LEVEL_BLOCK, pos.y - 4 * LEVEL_BLOCK, 4 * BLOCK_SIZE);
	CreateTutorial(TUTORIAL_TYPE_2, pos.x + 11 * LEVEL_BLOCK, pos.y - 4 * LEVEL_BLOCK, 4 * BLOCK_SIZE);
	CreateTutorial(TUTORIAL_TYPE_3, pos.x + 100.5 * LEVEL_BLOCK, pos.y - 9 * LEVEL_BLOCK, 4 * BLOCK_SIZE);
	//CreateTutorial(TUTORIAL_TYPE_4, pos.x + 245 * LEVEL_BLOCK, pos.y - 4 * LEVEL_BLOCK, 4 * BLOCK_SIZE);
	CreateTutorial(TUTORIAL_TYPE_4, pos.x + 127.5 * LEVEL_BLOCK, pos.y - 14 * LEVEL_BLOCK, 4 * BLOCK_SIZE);
}
