# include <Siv3D.hpp>
#include "Main.h"
#include "Sprite.h"
#include "action_enemy.h"
#include "enemybullet.h"
#include "player.h"
#include "Sound.h"
#include "explosion.h"
#include "camera.h"
#include "block.h"
#include "score.h"
#include "lift.h"
#include "gimmick.h"
#include "efect.h"

#define CHARA_SIZE 80.0f
#define pi 3.141592f

#define EDGE_CHARA_WIDTH ((SCREEN_WIDTH - CHARA_SIZE) / 2)
#define EDGE_CHARA_HEIGHT ((SCREEN_HEIGHT - CHARA_SIZE) / 2)

#define ENEMY_SHAKE_OFFSET 10.f

static AssetNameView enemy_type_1 = U"action_enemy_type_1", enemy_type_2 = U"action_enemy_type_2", enemy_type_3 = U"action_enemy_type_3";
static AssetNameView* g_action_enemy[ENEMY_TYPE_MAX] = 
{
	&enemy_type_1,
	&enemy_type_2,
	&enemy_type_3
};
static ACTION_ENEMY g_ActionEnemy[ENEMY_MAX];

static void DrawWalking(int index);
static void DrawIdle(int index);
static void DrawDeath(int index);

static BLOCK_TYPE BlockHitCheck(int index);
static void AnotherEnemyHitCheck(int index);
static void LiftHitCheck(int index);
static void UpdateEnemyType3(int index);
static float CalcResistance(int index, BLOCK_TYPE block_type);
static void EnemyDeath(int index);
static void RespawnOutScreen(int index, int vec);

void InitActionEnemy(void)
{
	TextureAsset::Register(*g_action_enemy[ENEMY_TYPE_1], U"Action/Texture/enemy_2.tga");
	TextureAsset::Register(*g_action_enemy[ENEMY_TYPE_2], U"Action/Texture/enemy_3.tga");
	TextureAsset::Register(*g_action_enemy[ENEMY_TYPE_3], U"Action/Texture/enemy.tga");

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		g_ActionEnemy[i].Enable = false;
		g_ActionEnemy[i].Alive = false;
		g_ActionEnemy[i].Set = false;
		g_ActionEnemy[i].Position.x = 0.f;
		g_ActionEnemy[i].Position.y = SCREEN_HEIGHT;
		g_ActionEnemy[i].Velocity.x = 0;
		g_ActionEnemy[i].Velocity.y = 0;

		g_ActionEnemy[i].size.x = CHARA_SIZE;
		g_ActionEnemy[i].size.y = CHARA_SIZE;
		g_ActionEnemy[i].vector = VEL_RIGHT;
		g_ActionEnemy[i].rad = 0.f;
	}
}

void UninitActionEnemy(void)
{
	for (int i = 0; i < ENEMY_TYPE_MAX; i++)
	{
		TextureAsset::Release(*g_action_enemy[i]);
	}
}

void UpdateActionEnemy(void)
{
	PLAYER *player = GetPlayer();
	CAMERA *camera = GetCamera();
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_ActionEnemy[i].Enable)
		{
			if (g_ActionEnemy[i].Alive == true)
			{
				if (g_ActionEnemy[i].HitStopCount > 0)
				{
					g_ActionEnemy[i].HitStopCount--;
					continue;
				}
				//移動
				if ((g_ActionEnemy[i].Landing) && (g_ActionEnemy[i].Position.x < (camera->Position.x + CHARA_SIZE + SCREEN_WIDTH / 2)) && (g_ActionEnemy[i].Position.x > (camera->Position.x - CHARA_SIZE - SCREEN_WIDTH / 2)))
				{
					g_ActionEnemy[i].Move = true;
					g_ActionEnemy[i].Invincible = false;
				}
				else
				{
					g_ActionEnemy[i].Invincible = true;
				}
				if (g_ActionEnemy[i].Move)
				{
					if (g_ActionEnemy[i].TYPE == ENEMY_TYPE_3)
					{
						UpdateEnemyType3(i);
					}
					else
					{
						int vector = g_ActionEnemy[i].vector;

						if ((g_ActionEnemy[i].Velocity.x < ENEMY_SPEED) && (g_ActionEnemy[i].Velocity.x > -ENEMY_SPEED))
						{
							g_ActionEnemy[i].Velocity.x += vector * 0.8f;
						}
						else
						{
							g_ActionEnemy[i].Velocity.x = vector * ENEMY_SPEED;
						}
					}
				}

				g_ActionEnemy[i].Velocity.y += 2.0f;


				g_ActionEnemy[i].Position.x += g_ActionEnemy[i].Velocity.x;
				g_ActionEnemy[i].Position.y += g_ActionEnemy[i].Velocity.y;

				BLOCK_TYPE touch_block = BlockHitCheck(i);
				LiftHitCheck(i);
				AnotherEnemyHitCheck(i);

				float resistance = CalcResistance(i, touch_block);
				g_ActionEnemy[i].Velocity.x -= g_ActionEnemy[i].Velocity.x * resistance;
				if ((g_ActionEnemy[i].Velocity.x < 0.1f) && (g_ActionEnemy[i].Velocity.x > -0.1f))
				{
					g_ActionEnemy[i].Velocity.x = 0.0f;
				}

				g_ActionEnemy[i].Count++;

				if (g_ActionEnemy[i].Life <= 0)
				{
					g_ActionEnemy[i].Alive = false;
					if (g_ActionEnemy[i].Position.x < player->Position.x)
					{
						g_ActionEnemy[i].Velocity.x = -20.f;
					}
					else
					{
						g_ActionEnemy[i].Velocity.x = 20.f;
					}
					g_ActionEnemy[i].Velocity.y = -15.f;
					g_ActionEnemy[i].Count = 0;
					PlaySE(WSD_HIT1);
					AddPoint(g_ActionEnemy[i].Max_Life);
				}

				if (g_ActionEnemy[i].Position.y > SCREEN_HEIGHT)
				{
					DeleteActionEnemy(i);
				}
			}
			else if (g_ActionEnemy[i].Alive == false)
			{
				EnemyDeath(i);
			}
		}
		/*else if (g_ActionEnemy[i].Set)
		{
			if (g_ActionEnemy[i].SetPosition.x > (camera->Position.x + 0.75 * SCREEN_WIDTH))
			{
				RespawnOutScreen(i, VEL_LEFT);
			}
			else if (g_ActionEnemy[i].SetPosition.x < (camera->Position.x - 0.75 * SCREEN_WIDTH))
			{
				RespawnOutScreen(i, VEL_RIGHT);
			}
		}*/
	}
}

void DrawActionEnemy(void)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_ActionEnemy[i].Enable)
		{
			if (g_ActionEnemy[i].Alive == true)
			{
				if (g_ActionEnemy[i].Velocity.x == 0)
				{
					DrawIdle(i);
				}
				else
				{
					DrawWalking(i);
				}
			}
			else
			{
				DrawDeath(i);
			}
		}
	}
}

ACTION_ENEMY* GetActionEnemy(void)
{
	return g_ActionEnemy;
}

void CreateActionEnemy(ACTION_ENEMY_TYPE TYPE, float x, float y, int Life, int vec, bool boss)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_ActionEnemy[i].Set == false && g_ActionEnemy[i].Enable == false)
		{
			g_ActionEnemy[i].TYPE = TYPE;
			g_ActionEnemy[i].Enable = true;
			g_ActionEnemy[i].Move = false;
			g_ActionEnemy[i].Position.x = x;
			g_ActionEnemy[i].Position.y = y;
			g_ActionEnemy[i].SetPosition.x = g_ActionEnemy[i].Position.x;
			g_ActionEnemy[i].SetPosition.y = g_ActionEnemy[i].Position.y;
			g_ActionEnemy[i].Velocity.x = 0;
			g_ActionEnemy[i].Velocity.y = 0;
			g_ActionEnemy[i].size.x = CHARA_SIZE;
			g_ActionEnemy[i].size.y = CHARA_SIZE;
			g_ActionEnemy[i].Life = Life;
			g_ActionEnemy[i].Max_Life = Life;
			g_ActionEnemy[i].Alive = true;
			g_ActionEnemy[i].vector = vec;
			g_ActionEnemy[i].Count = 0;
			g_ActionEnemy[i].rad = 0;
			g_ActionEnemy[i].HitStopCount = 0;
			g_ActionEnemy[i].Boss = boss;
			g_ActionEnemy[i].Set = true;
			g_ActionEnemy[i].Landing = false;

			if (boss)
			{
				g_ActionEnemy[i].size.x *= 2;
				g_ActionEnemy[i].size.y *= 2;
			}
			break;
		}
	}
}

void DeleteActionEnemy(int index)
{
	g_ActionEnemy[index].Enable = false;
	g_ActionEnemy[index].Alive = false;
}

void AddActionEnemyDamage(int index, int damage)
{
	if (g_ActionEnemy[index].Invincible) return;
	g_ActionEnemy[index].Life -= damage;
	CreateExplosion(g_ActionEnemy[index].Position, (g_ActionEnemy[index].size.y / 2), ENEMY_DAMAGED);
}

static void DrawWalking(int index)
{
	CAMERA *camera = GetCamera();

	DrawSpriteQuad(*g_action_enemy[g_ActionEnemy[index].TYPE],
		g_ActionEnemy[index].Position.x - camera->Position.x, g_ActionEnemy[index].Position.y - camera->Position.y, g_ActionEnemy[index].size.x, g_ActionEnemy[index].size.y,
		1.f, 1.f, 1.f, 1.f,
		0.f, 0.f, 1.0, 1.0,
		(g_ActionEnemy[index].vector == VEL_RIGHT ? true : false) , g_ActionEnemy[index].rad);
}

static void DrawIdle(int index)
{
	CAMERA *camera = GetCamera();

	DrawSpriteQuad(*g_action_enemy[g_ActionEnemy[index].TYPE],
		g_ActionEnemy[index].Position.x - camera->Position.x, g_ActionEnemy[index].Position.y - camera->Position.y, g_ActionEnemy[index].size.x, g_ActionEnemy[index].size.y,
		1.f, 1.f, 1.f, 1.f,
		0.f, 0.f, 1.0, 1.0,
		(g_ActionEnemy[index].vector == VEL_RIGHT ? true : false), g_ActionEnemy[index].rad);
}

static void DrawDeath(int index)
{
	CAMERA *camera = GetCamera();
	int Cnt = g_ActionEnemy[index].Count;
	int redTiming = (Cnt / 5) % 2;
	if (redTiming == 0)
	{
		DrawSpriteQuad(*g_action_enemy[g_ActionEnemy[index].TYPE],
		g_ActionEnemy[index].Position.x - camera->Position.x, g_ActionEnemy[index].Position.y - camera->Position.y, g_ActionEnemy[index].size.x, g_ActionEnemy[index].size.y,
		1.f, 1.f, 1.f, 1.f,
		0.f, 0.f, 1.0, 1.0,
		(g_ActionEnemy[index].vector == VEL_RIGHT ? true : false), g_ActionEnemy[index].rad);
	}
	else
	{
		DrawSpriteQuad(*g_action_enemy[g_ActionEnemy[index].TYPE],
		g_ActionEnemy[index].Position.x - camera->Position.x, g_ActionEnemy[index].Position.y - camera->Position.y, g_ActionEnemy[index].size.x, g_ActionEnemy[index].size.y,
		1.f, 0.f, 0.f, 1.f,
		0.f, 0.f, 1.0, 1.0,
		(g_ActionEnemy[index].vector == VEL_RIGHT ? true : false), g_ActionEnemy[index].rad);
	}
}

static BLOCK_TYPE BlockHitCheck(int index)
{
	float pl = g_ActionEnemy[index].Position.x - g_ActionEnemy[index].size.x / 2;
	float pr = g_ActionEnemy[index].Position.x + g_ActionEnemy[index].size.x / 2;
	float pt = g_ActionEnemy[index].Position.y - g_ActionEnemy[index].size.y / 2;
	float pb = g_ActionEnemy[index].Position.y + g_ActionEnemy[index].size.y / 2;
	BLOCK *block = GetBlock();
	BLOCK_TYPE ret = BLOCK_TYPE_NONE;

	for (int i = 0; i < BLOCK_MAX; i++)
	{
		if (block[i].Enable)
		{
			float bl = block[i].Position.x - block[i].SIZE.x / 2;
			float br = block[i].Position.x + block[i].SIZE.x / 2;
			float bt = block[i].Position.y - block[i].SIZE.y / 2;
			float bb = block[i].Position.y + block[i].SIZE.y / 2;

			if ((bl <= pr) //left
				&& (br >= pl) //right
				&& (bt <= pb) //top
				&& (bb >= pt)) //botom
			{
				float dl = bl - pr;
				float dr = br - pl;
				float dt = bt - pb;
				float db = bb - pt;
				float dx, dy;

				if (-dl < dr)
				{
					dx = dl;
				}
				else
				{
					dx = dr;
				}

				if (-dt < db)
				{
					dy = dt;
				}
				else
				{
					dy = db;
				}

				if ((abs(dx) > abs(dy)) || (abs(dy) < 8.0f))
				{
					if (dy * g_ActionEnemy[index].Velocity.y < 0.0f)
					{
						g_ActionEnemy[index].Position.y += dy;
						g_ActionEnemy[index].Velocity.y = 0.0f;
						g_ActionEnemy[index].Landing = true;

						if (g_ActionEnemy[index].TYPE == ENEMY_TYPE_2)
						{
							if (block[i].Left == NULL && g_ActionEnemy[index].Position.x < bl + 10.f)
							{
								g_ActionEnemy[index].vector = VEL_RIGHT;
							}
							else if (block[i].Right == NULL && g_ActionEnemy[index].Position.x > br - 10.f)
							{
								g_ActionEnemy[index].vector = VEL_LEFT;
							}
						}
						ret = block[i].TYPE;
					}
				}
				else
				{
					if (dx * g_ActionEnemy[index].Velocity.x < 0.0f)
					{
						g_ActionEnemy[index].Position.x += dx;
						g_ActionEnemy[index].vector *= -1;
						break;
					}
				}
			}
		}
	}
	return ret;
}

static void AnotherEnemyHitCheck(int index)
{
	float pl = g_ActionEnemy[index].Position.x - g_ActionEnemy[index].size.x / 2;
	float pr = g_ActionEnemy[index].Position.x + g_ActionEnemy[index].size.x / 2;
	float pt = g_ActionEnemy[index].Position.y - g_ActionEnemy[index].size.y / 2;
	float pb = g_ActionEnemy[index].Position.y + g_ActionEnemy[index].size.y / 2;
	BLOCK *block = GetBlock();

	for (int i = index + 1; i < BLOCK_MAX; i++)
	{
		if (g_ActionEnemy[i].Enable && g_ActionEnemy[i].Alive)
		{
			float bl = g_ActionEnemy[i].Position.x - g_ActionEnemy[i].size.x / 2;
			float br = g_ActionEnemy[i].Position.x + g_ActionEnemy[i].size.x / 2;
			float bt = g_ActionEnemy[i].Position.y - g_ActionEnemy[i].size.y / 2;
			float bb = g_ActionEnemy[i].Position.y + g_ActionEnemy[i].size.y / 2;

			if ((bl <= pr) //left
				&& (br >= pl) //right
				&& (bt <= pb) //top
				&& (bb >= pt)) //botom
			{
				float dl = bl - pr;
				float dr = br - pl;
				float dt = bt - pb;
				float db = bb - pt;
				float dx, dy;

				if (-dl < dr)
				{
					dx = dl;
				}
				else
				{
					dx = dr;
				}

				if (-dt < db)
				{
					dy = dt;
				}
				else
				{
					dy = db;
				}

				if (dx * g_ActionEnemy[index].Velocity.x < 0.0f)
				{
					g_ActionEnemy[index].Position.x += dx;
					g_ActionEnemy[index].vector *= -1;

					g_ActionEnemy[i].Position.x -= dx;
					g_ActionEnemy[i].vector *= -1;
					break;
				}
			}
		}
	}
}

static void EnemyDeath(int index)
{
	g_ActionEnemy[index].Velocity.x -= 0.15 * g_ActionEnemy[index].Velocity.x;
	g_ActionEnemy[index].Velocity.y -= 0.15 * g_ActionEnemy[index].Velocity.y;
	g_ActionEnemy[index].Position.x += g_ActionEnemy[index].Velocity.x;
	g_ActionEnemy[index].Position.y += g_ActionEnemy[index].Velocity.y;
	g_ActionEnemy[index].rad += -0.08f * g_ActionEnemy[index].vector;
	g_ActionEnemy[index].Count++;
	if (g_ActionEnemy[index].Count > 25)
	{
		DeleteActionEnemy(index);
		CreateExplosion(g_ActionEnemy[index].Position, g_ActionEnemy[index].size.y, ENEMY_DAMAGED);
		PlaySE(WSD_PYORO1);
		ShakeCamera(30.f);
		if (g_ActionEnemy[index].Boss)
		{
			CreateGimmickRectangle(GIMMICK_TYPE_KEY, g_ActionEnemy[index].Position.x, g_ActionEnemy[index].Position.y, 2 * GIMMICK_SIZE, 1, 1);
			CreateKirakira(g_ActionEnemy[index].Position.x, g_ActionEnemy[index].Position.y);
			PlaySE(WSD_MAN_YEAH);
			RespawnActionEnemy();
		}
	}
}

static void LiftHitCheck(int index)
{
	bool stamp = false;
	float pl = g_ActionEnemy[index].Position.x - g_ActionEnemy[index].size.x / 2;
	float pr = g_ActionEnemy[index].Position.x + g_ActionEnemy[index].size.x / 2;
	float pt = g_ActionEnemy[index].Position.y - g_ActionEnemy[index].size.y / 2;
	float pb = g_ActionEnemy[index].Position.y + g_ActionEnemy[index].size.y / 2;
	LIFT *lift = GetLift();

	for (int i = 0; i < LIFT_MAX; i++)
	{
		if (lift[i].Enable)
		{
			float bl = lift[i].Position.x - lift[i].SIZE.x / 2;
			float br = lift[i].Position.x + lift[i].SIZE.x / 2;
			float bt = lift[i].Position.y - lift[i].SIZE.y / 2;
			float bb = lift[i].Position.y + lift[i].SIZE.y / 2;

			if ((bl <= pr) //left
				&& (br >= pl) //right
				&& (bt <= pb) //top
				&& (bb >= pt)) //botom
			{
				float dl = bl - pr;
				float dr = br - pl;
				float dt = bt - pb;
				float db = bb - pt;
				float dx, dy;

				if (-dl < dr)
				{
					dx = dl;
				}
				else
				{
					dx = dr;
				}

				if (-dt < db)
				{
					dy = dt;
					stamp = true;
				}
				else
				{
					dy = db;
				}

				if ((abs(dx) > abs(dy)) || (abs(dy) < 8.0f))
				{
					if (dy * g_ActionEnemy[index].Velocity.y < 0.0f)
					{
						g_ActionEnemy[index].Position.y += dy;
						g_ActionEnemy[index].Velocity.y = 0.f;
						/*g_ActionEnemy[index].Position.x += lift[i].Velocity.x;
						g_ActionEnemy[index].Position.y += lift[i].Velocity.y;*/
						g_ActionEnemy[index].Position.x += lift[i].Position.x - lift[i].OldPosition.x;
						g_ActionEnemy[index].Position.y += lift[i].Position.y - lift[i].OldPosition.y;
						g_ActionEnemy[index].Landing = true;

						if (g_ActionEnemy[index].TYPE == ENEMY_TYPE_2)
						{
							if (g_ActionEnemy[index].Position.x < bl + 10.f)
							{
								g_ActionEnemy[index].vector = VEL_RIGHT;
							}
							else if (g_ActionEnemy[index].Position.x > br - 10.f)
							{
								g_ActionEnemy[index].vector = VEL_LEFT;
							}
						}
					}
				}
				else
				{
					if (dx * g_ActionEnemy[index].Velocity.x < 0.0f)
					{
						g_ActionEnemy[index].Position.x += dx;
						g_ActionEnemy[index].Velocity.x = 0.0f;
					}
				}
			}
		}
	}
}

static float CalcResistance(int index, BLOCK_TYPE block_type)
{
	float resistance = 0.05f;
	switch (block_type)
	{
	case BLOCK_TYPE_NORMAL:
		resistance += 0.1f;
		break;
	case BLOCK_TYPE_SOIL:
		resistance += 0.15f;
		break;
	case BLOCK_TYPE_ICE:
		resistance -= 0.04f;
		break;
	case BLOCK_TYPE_NONE:
		resistance += 0.05f;
		break;
	}
	return resistance;
}

static void UpdateEnemyType3(int index)
{
	if ((g_ActionEnemy[index].TYPE != ENEMY_TYPE_3) || (!g_ActionEnemy[index].Move)) return;
	g_ActionEnemy[index].Velocity.x = 0;
	PLAYER *player = GetPlayer();
	if (player->Position.x < g_ActionEnemy[index].Position.x)
	{
		g_ActionEnemy[index].vector = VEL_LEFT;
	}
	else
	{
		g_ActionEnemy[index].vector = VEL_RIGHT;
	}

	int turn;
	if (g_ActionEnemy[index].Boss)
	{
		turn = 30;
	}
	else
	{
		turn = 60;
	}
	if (g_ActionEnemy[index].Count % turn == 0)
	{
		int enemy_jump = rand() % 4;
		if (enemy_jump == 0)
		{
			g_ActionEnemy[index].Velocity.y = -30.f;
		}
		int enemy_shot = rand() % 2;
		if (enemy_shot == 0)
		{
			CreateEnemyBullet(g_ActionEnemy[index].TYPE, g_ActionEnemy[index].Position, g_ActionEnemy[index].size.x, g_ActionEnemy[index].vector, g_ActionEnemy[index].Boss);
		}
	}
}

void HitStopActionEnemy(int index, int TimeCount)
{
	g_ActionEnemy[index].HitStopCount = TimeCount;
}

static void RespawnOutScreen(int index, int vec)
{
	if (g_ActionEnemy[index].Set == false) return;
	if (g_ActionEnemy[index].Boss == true) return;
	g_ActionEnemy[index].Enable = true;
	g_ActionEnemy[index].Move = false;
	g_ActionEnemy[index].Position.x = g_ActionEnemy[index].SetPosition.x;
	g_ActionEnemy[index].Position.y = g_ActionEnemy[index].SetPosition.y;
	g_ActionEnemy[index].Velocity.x = 0;
	g_ActionEnemy[index].Velocity.y = 0;
	g_ActionEnemy[index].Life = g_ActionEnemy[index].Max_Life;
	g_ActionEnemy[index].Alive = true;
	g_ActionEnemy[index].vector = vec;
	g_ActionEnemy[index].Count = 0;
	g_ActionEnemy[index].rad = 0;
	g_ActionEnemy[index].HitStopCount = 0;
	g_ActionEnemy[index].Landing = false;
}

void RespawnActionEnemy(void)
{
	PLAYER *player = GetPlayer();
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_ActionEnemy[i].SetPosition.x < player->CheckPoint.x)
		{
			RespawnOutScreen(i, VEL_RIGHT);
		}
		else
		{
			RespawnOutScreen(i, VEL_LEFT);
		}
	}
}
