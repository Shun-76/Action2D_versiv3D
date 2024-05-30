# include <Siv3D.hpp>
#include "Main.h"
#include "Sprite.h"
#include "player.h"
#include "action_enemy.h"
#include "enemybullet.h"
#include "sound.h"
#include "explosion.h"
#include "camera.h"
#include "block.h"
#include "gimmick.h"
#include "score.h"
#include "lift.h"
#include "bullet.h"
#include "efect.h"
#include "result.h"

#define DEFAULT_GRAVITY_SCALE 2.0f
#define pi 3.141592f

#define EDGE_CHARA_WIDTH ((SCREEN_WIDTH - PLAYER_SIZE) / 2)
#define EDGE_CHARA_HEIGHT ((SCREEN_HEIGHT - PLAYER_SIZE) / 2)

#define CHARA_DEFAULT_LIFE 3

static Float2 g_Gravity_Player;
static int gravity_vec;

static AssetNameView g_player = U"player_character";
static PLAYER g_Player;
static float MAX_PLAYER_SPEED;

static void DrawWalking(float scale);
static void DrawIdle(float scale);
static void DrawJumping(float scale);
static bool PlayerIdle;
static bool oldIdle;
static int runningCnt;
static void DrawOntheWall(float scale);
static bool PlayerOntheWall;
static bool fall;
static bool isDash;

PLAYER_STATUS g_Status;

static BLOCK_TYPE BlockHitCheck(void);
static GIMMICK_TYPE GimmickHitCheck(int *index);
static void EnemyHitCheck(void);
static void EnemyBulletHitCheck(void);
static BLOCK_TYPE LiftHitCheck(void);
static float CalcResistance(BLOCK_TYPE block_type);

void InitPlayer(void)
{
	TextureAsset::Register(g_player, U"Action/Texture/action_ninja.tga");

	g_Player.Enable = true;
	g_Player.Position.x = -750.f - SCREEN_WIDTH / 4;
	g_Player.Position.y = 0.f;
	g_Player.CheckPoint.x = g_Player.Position.x;
	g_Player.CheckPoint.y = g_Player.Position.y;
	g_Player.Velocity.x = 0;
	g_Player.Velocity.y = 0;
	g_Player.Scale.x = 1.f;
	g_Player.Scale.y = 1.f;
	g_Player.Life = CHARA_DEFAULT_LIFE;
	g_Player.Alive = true;
	g_Player.Key = false;
	g_Player.vector = VEL_RIGHT;
	g_Player.size.x = PLAYER_SIZE;
	g_Player.size.y = PLAYER_SIZE;
	g_Player.Zanki = 3;
	g_Player.Count = 0;
	g_Player.Jump = false;
	g_Player.AirJump = false;
	g_Player.DoorOpen = false;
	g_Player.DamageCnt = -1;
	g_Player.HitStopTimeCount = 0;

	PlayerIdle = false;
	PlayerOntheWall = false;
	oldIdle = PlayerIdle;
	runningCnt = 0;

	g_Player.DeathCount = 0;
	MAX_PLAYER_SPEED = PLAYER_SPEED;

	g_Player.Color.r = 1.f;
	g_Player.Color.g = 1.f;
	g_Player.Color.b = 1.f;
	g_Player.Color.a = 1.f;


	gravity_vec = 0;
	g_Gravity_Player.x = sinf(pi / 2 * gravity_vec) * DEFAULT_GRAVITY_SCALE;
	g_Gravity_Player.y = cosf(pi / 2 * gravity_vec) * DEFAULT_GRAVITY_SCALE;

	fall = false;
	isDash = false;

	g_Status.Speed = 1.0f;
	g_Status.Jump = 1.0f;
	g_Status.Attack = 1.0f;
	g_Status.Weight = 1.0f;
}

void UninitPlayer(void)
{
	TextureAsset::Release(g_player);
}

void UpdatePlayer(void)
{
	if (!g_Player.Enable)
	{
		if (MouseL.down())
		{
			respawnPlayer();
		}
		if (MouseR.down())
		{
			SetResult(RESULT_GAMEOVER);
			_SetScene(SCENE_RESULT);
		}
	}
	else if(g_Player.DoorOpen)
	{
	}
	else if (g_Player.Alive == true)
	{
		g_Player.Scale.x += (1.f - g_Player.Scale.x) * 0.1f;
		g_Player.Scale.y += (1.0f - g_Player.Scale.y) * 0.1f;
		if (g_Player.HitStopTimeCount > 0)
		{
			g_Player.HitStopTimeCount--;
			return;
		}
		oldIdle = PlayerIdle;
		PlayerIdle = true;
		fall = false;
		isDash = false;
		if (KeyLShift.pressed())
		{
			isDash = true;
			MAX_PLAYER_SPEED = 1.5f * PLAYER_SPEED * g_Status.Speed;
		}
		else
		{
			if (MAX_PLAYER_SPEED > PLAYER_SPEED * g_Status.Speed)
			{
				MAX_PLAYER_SPEED -= 0.1f * MAX_PLAYER_SPEED;
			}
			else
			{
				MAX_PLAYER_SPEED = PLAYER_SPEED * g_Status.Speed;
			}
		}
		if (KeyA.pressed())
		{
			//g_Player.Position.x -= PLAYER_SPEED;
			PlayerIdle = false;
			g_Player.Velocity.x -= 3.f;
			g_Player.vector = VEL_LEFT;
			if (g_Player.Velocity.x < -1 * MAX_PLAYER_SPEED)
			{
				g_Player.Velocity.x = -1 * MAX_PLAYER_SPEED;
			}
		}
		if (KeyD.pressed())
		{
			//g_Player.Position.x += PLAYER_SPEED;
			PlayerIdle = false;
			g_Player.Velocity.x += 3.f;
			g_Player.vector = VEL_RIGHT;
			if (g_Player.Velocity.x > MAX_PLAYER_SPEED)
			{
				g_Player.Velocity.x = MAX_PLAYER_SPEED;
			}
		}
		if (MouseL.down())
		{
			if (!g_Player.Jump)
			{
				g_Player.Velocity.y = -30.0f * g_Status.Jump;
				g_Player.Scale.x = 0.8f;
				g_Player.Scale.y = 1.3f;
				PlaySE(WSD_JUMP01);
				CreateJump(g_Player.Position.x, g_Player.Position.y + (g_Player.size.y * 0.5f));
			}
			else
			{
				if (!g_Player.AirJump)
				{
					g_Player.Velocity.y = -25.0f * g_Status.Jump;
					g_Player.AirJump = true;

					g_Player.Scale.x = 0.8f;
					g_Player.Scale.y = 1.3f;
					PlaySE(WSD_JUMP02);

					CreateJump(g_Player.Position.x, g_Player.Position.y + (g_Player.size.y * 0.5f));
				}
			}
		}
		bool oldJump = g_Player.Jump;
		g_Player.Jump = true;
		if (KeyP.down())
		{
			gravity_vec++;
			float gravity_scale;
			if (gravity_vec % 4 == 0)
			{
				gravity_scale = DEFAULT_GRAVITY_SCALE;
			}
			else
			{
				gravity_scale = DEFAULT_GRAVITY_SCALE / 3;
			}
			g_Gravity_Player.x = sinf(pi / 2 * gravity_vec) * gravity_scale * g_Status.Weight;
			g_Gravity_Player.y = cosf(pi / 2 * gravity_vec) * gravity_scale * g_Status.Weight;
		}

		if (MouseR.down())
		{
			CreateBullet(g_Player.Position, (float)g_Player.vector, g_Player.size.x * g_Player.Scale.x);
			PlaySE(WSD_THROW);
		}

		g_Player.Velocity.x += g_Gravity_Player.x;
		g_Player.Velocity.y += g_Gravity_Player.y;

		if (g_Player.Velocity.y > 30.f)
		{
			g_Player.Velocity.y = 30.f;
		}
		g_Player.Position.x += g_Player.Velocity.x;
		g_Player.Position.y += g_Player.Velocity.y;

		EnemyHitCheck();
		EnemyBulletHitCheck();
		BLOCK_TYPE touch_block = BlockHitCheck();
		BLOCK_TYPE touch_lift = LiftHitCheck();
		int gimmick_index;
		GIMMICK_TYPE gimmick = GimmickHitCheck(&gimmick_index);

		if ((gimmick == GIMMICK_TYPE_DOOR) && (KeySpace.down() || KeyW.down()))
		{
			if (g_Player.Key)
			{
				g_Player.DoorOpen = true;
				PlaySE(WSD_OPEN);
			}
			else
			{
				failedDoorOpen(gimmick_index);
				PlaySE(WSD_CLOSE);
			}
		}

		if (oldJump && !g_Player.Jump)
		{
			PlaySE(WSD_HIT1);
			g_Player.Scale.x = 1.3f;
			g_Player.Scale.y = 0.7f;
			CreateSmoke(g_Player.Position.x, g_Player.Position.y, g_Player.vector, EFECT_SIZE);
			CreateSmoke(g_Player.Position.x, g_Player.Position.y, (-1 * g_Player.vector), EFECT_SIZE);
		}
		if (!g_Player.Jump)
		{
			if (oldIdle && !PlayerIdle)
			{
				CreateSmoke(g_Player.Position.x, g_Player.Position.y, (-1 * g_Player.vector), 1.5f * EFECT_SIZE);
			}
			else if (!oldIdle && PlayerIdle)
			{
				CreateSmoke(g_Player.Position.x, g_Player.Position.y, g_Player.vector, EFECT_SIZE);
			}
			if (!PlayerIdle)
			{
				runningCnt++;
				int SmokeCnt;
				if (isDash)
				{
					SmokeCnt = 20;
				}
				else
				{
					SmokeCnt = 30;
				}
				if ((runningCnt != 0) && (runningCnt % SmokeCnt == 0))
				{
					CreateSmoke(g_Player.Position.x, g_Player.Position.y, (-1 * g_Player.vector), 0.8f * EFECT_SIZE);
				}
			}
		}
		else
		{
			runningCnt = 0;
		}

		float resistance;
		if (touch_lift == BLOCK_TYPE_NONE)
		{
			resistance = CalcResistance(touch_block);
		}
		else
		{
			resistance = CalcResistance(touch_lift);
		}
		g_Player.Velocity.x -= g_Player.Velocity.x * resistance;
		/*g_Player.Velocity.x -= g_Player.Velocity.x * 0.1f;*/
		if ((g_Player.Velocity.x < 0.1f) && (g_Player.Velocity.x > -0.1f))
		{
			g_Player.Velocity.x = 0.0f;
		}

		if (g_Player.Position.y > PLAYER_SIZE + (SCREEN_HEIGHT / 2))
		{
			g_Player.Life = 0;
			fall = true;
		}

		g_Player.Count++;

		if (g_Player.DamageCnt >= 0)
		{
			g_Player.DamageCnt++;
			if (g_Player.DamageCnt > 80)
			{
				g_Player.DamageCnt = -1;
			}
		}

		if (g_Player.Life <= 0)
		{
			g_Player.Alive = false;
			g_Player.DeathCount = 0;
			CreateDeathEfect(g_Player.Position.x, g_Player.Position.y, fall);
			PlaySE(WSD_EXP);
		}
	}
	else if (g_Player.Alive == false)
	{
		g_Player.DeathCount++;
		if (g_Player.DeathCount > DEATH_CNT_MAX)
		{
			g_Player.Enable = false;
			if (g_Player.Zanki < 1)
			{
				SetResult(RESULT_GAMEOVER);
				_SetScene(SCENE_RESULT);
			}
		}
	}
}

void DrawPlayer(void)
{
	//CAMERA *camera = GetCamera();
	//DrawSpriteQuad(g_Player.Position.x - camera->Position.x, g_Player.Position.y - camera->Position.y, g_Player.size, g_Player.size, 1.f, 1.f, 1.f, 1.f, g_player, 0.f, 0.f, g_Player.vel, 1.0f);
	if (g_Player.Enable)
	{
		bool Enable = true;
		int Cnt = g_Player.DamageCnt;
		if (Cnt >= 0)
		{
			if ((Cnt / 5) % 2 == 1)
			{
				Enable = false;
			}
		}
		if (Enable)
		{
			float scale = g_Player.size.y * (1.0f - g_Player.Scale.y) * 0.5f;
			if (g_Player.Jump)
			{
				DrawJumping(scale);
			}
			else if (PlayerIdle)
			{
				DrawIdle(scale);
			}
			else if (PlayerOntheWall)
			{
				DrawOntheWall(scale);
			}
			else
			{
				DrawWalking(scale);
			}
		}
	}
}

PLAYER* GetPlayer(void)
{
	return &g_Player;
}


int GetPlayerStageBlock(void)
{
	return (int)g_Player.Position.x / RANDOM_STAGE_BLOCK;
}

void respawnPlayer(void)
{
	g_Player.Enable = true;
	g_Player.Position.x = g_Player.CheckPoint.x;
	g_Player.Position.y = g_Player.CheckPoint.y - 80.f;
	g_Player.Velocity.x = 0;
	g_Player.Velocity.y = 0;
	g_Player.Scale.x = 1.f;
	g_Player.Scale.y = 1.f;
	g_Player.Life = CHARA_DEFAULT_LIFE;
	g_Player.Alive = true;
	g_Player.Jump = false;
	g_Player.AirJump = false;
	g_Player.Zanki--;
	g_Player.vector = VEL_RIGHT;
	g_Player.Count = 0;
	g_Player.DamageCnt = -1;
	g_Player.HitStopTimeCount = 0;
	g_Player.DeathCount = 0;

	RespawnActionEnemy();

	respawnScore();
}

void AddPlayerDamage(int damage)
{
	g_Player.Life -= damage;
	g_Player.DamageCnt = 0;
	CreateExplosion(g_Player.Position, (g_Player.size.y / 2), PLAYER_DAMAGED);
	PlaySE(WSD_HIT1);
}

static void DrawWalking(float scale)
{
	int count = g_Player.Count / 5;
	float tw, th;
	tw = 71.f / 663.f;
	th = 70.f / 410.f;

	float u = (float)((count % 6) * 71 + 30) / 663.f;
	float v = 110.f / 410.f;
	CAMERA *camera = GetCamera();

	g_Player.size.x = PLAYER_SIZE;
	if (g_Player.vector == VEL_RIGHT)
	{
		DrawSpriteQuad(g_player,
			g_Player.Position.x - camera->Position.x, g_Player.Position.y - camera->Position.y + scale,
			g_Player.size.x * g_Player.Scale.x, g_Player.size.y * g_Player.Scale.y,
			g_Player.Color.r, g_Player.Color.g, g_Player.Color.b, g_Player.Color.a,
			u, v, tw, th);
	}
	else
	{
		DrawSpriteQuad(g_player,
			g_Player.Position.x - camera->Position.x, g_Player.Position.y - camera->Position.y + scale,
			g_Player.size.x * g_Player.Scale.x, g_Player.size.y * g_Player.Scale.y,
			g_Player.Color.r, g_Player.Color.g, g_Player.Color.b, g_Player.Color.a,
			u, v, tw, th, true);
	}
}

static void DrawIdle(float scale)
{
	int count = g_Player.Count / 10;
	float tw, th;
	tw = 43.75f / 664.f;
	th = 70.f / 410.f;

	float u = 0.f;
	switch (count % 4)
	{
	case 0:
		u = 36.5f / 663.f;
		break;
	case 1:
		u = 82.f / 663.f;
		break;
	case 2:
		u = 125.f / 663.f;
		break;
	case 3:
		u = 169.f / 663.f;
		break;
	}
	float v = 25.f / 410.f;
	CAMERA *camera = GetCamera();

	g_Player.size.x = PLAYER_SIZE * (43.75f / 71.f);
	if (g_Player.vector == VEL_RIGHT)
	{
		DrawSpriteQuad(g_player,
			g_Player.Position.x - camera->Position.x, g_Player.Position.y - camera->Position.y + scale,
			g_Player.size.x * g_Player.Scale.x, g_Player.size.y * g_Player.Scale.y,
			g_Player.Color.r, g_Player.Color.g, g_Player.Color.b, g_Player.Color.a,
			u, v, tw, th);
	}
	else
	{
		DrawSpriteQuad(g_player,
			g_Player.Position.x - camera->Position.x, g_Player.Position.y - camera->Position.y + scale,
			g_Player.size.x * g_Player.Scale.x, g_Player.size.y * g_Player.Scale.y,
			g_Player.Color.r, g_Player.Color.g, g_Player.Color.b, g_Player.Color.a,
			u, v, tw, th, true);
	}
}

static void DrawJumping(float scale)
{
	float tw = 1.f, th = 1.f;

	float u = 0.f, v = 0.f;
	CAMERA *camera = GetCamera();

	if (g_Player.Velocity.y < 0)
	{
		tw = 52.f / 664.f;
		th = 70.f / 410.f;
		u = 84.f / 663.f;
		v = 200.f / 410.f;
		g_Player.size.x = PLAYER_SIZE * (52.f / 71.f);
		g_Player.size.y = PLAYER_SIZE;
	}
	else
	{
		tw = 58.f / 664.f;
		th = 60.f / 410.f;
		u = 463.f / 663.f;
		v = 200.f / 410.f;
		g_Player.size.x = PLAYER_SIZE * (58.f / 71.f);
		g_Player.size.y = PLAYER_SIZE * (60.f / 70.f);
	}
	if (g_Player.vector == VEL_RIGHT)
	{
		DrawSpriteQuad(g_player,
			g_Player.Position.x - camera->Position.x, g_Player.Position.y - camera->Position.y + scale,
			g_Player.size.x * g_Player.Scale.x, g_Player.size.y * g_Player.Scale.y,
			g_Player.Color.r, g_Player.Color.g, g_Player.Color.b, g_Player.Color.a,
			u, v, tw, th);
	}
	else
	{
		DrawSpriteQuad(g_player,
			g_Player.Position.x - camera->Position.x, g_Player.Position.y - camera->Position.y + scale,
			g_Player.size.x * g_Player.Scale.x, g_Player.size.y * g_Player.Scale.y,
			g_Player.Color.r, g_Player.Color.g, g_Player.Color.b, g_Player.Color.a,
			u, v, tw, th, true);
	}
}

static void DrawOntheWall(float scale)
{
	float tw, th;
	tw = 39.0f / 663.f;
	th = 70.f / 410.f;

	float u = 587.f / 663.f;
	float v = 216.f / 410.f;
	CAMERA *camera = GetCamera();
	g_Player.size.x = PLAYER_SIZE * (39.0f / 71.f);
	if (g_Player.vector == VEL_RIGHT)
	{
		DrawSpriteQuad(g_player,
			g_Player.Position.x - camera->Position.x, g_Player.Position.y - camera->Position.y + scale,
			g_Player.size.x * g_Player.Scale.x, g_Player.size.y * g_Player.Scale.y,
			g_Player.Color.r, g_Player.Color.g, g_Player.Color.b, g_Player.Color.a,
			u, v, tw, th);
	}
	else
	{
		DrawSpriteQuad(g_player,
			g_Player.Position.x - camera->Position.x, g_Player.Position.y - camera->Position.y + scale,
			g_Player.size.x * g_Player.Scale.x, g_Player.size.y * g_Player.Scale.y,
			g_Player.Color.r, g_Player.Color.g, g_Player.Color.b, g_Player.Color.a,
			u, v, tw, th, true);
	}
}

static BLOCK_TYPE BlockHitCheck(void)
{
	/*float pl = g_Player.Position.x - g_Player.size.x / 2;
	float pr = g_Player.Position.x + g_Player.size.x / 2;*/
	float pl = g_Player.Position.x - PLAYER_SIZE * (43.75f / 71.f) / 2;
	float pr = g_Player.Position.x + PLAYER_SIZE * (43.75f / 71.f) / 2;
	float pt = g_Player.Position.y - g_Player.size.y / 2;
	float pb = g_Player.Position.y + g_Player.size.y / 2;
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
					if (dy * g_Player.Velocity.y < 0.0f)
					{
						g_Player.Position.y += dy;
						g_Player.Velocity.y = 0.0f;
						
						if (dy < 0)
						{
							g_Player.Jump = false;

							g_Player.AirJump = false;
						}
						ret = block[i].TYPE;
					}
				}
				else
				{
					if (dx * g_Player.Velocity.x < 0.0f)
					{
						g_Player.Position.x += dx;
						g_Player.Velocity.x = 0.0f;
					}
				}
			}
		}
	}
	return ret;
}

static GIMMICK_TYPE GimmickHitCheck(int *index)
{
	/*float pl = g_Player.Position.x - g_Player.size.x / 2;
	float pr = g_Player.Position.x + g_Player.size.x / 2;*/
	float pl = g_Player.Position.x - PLAYER_SIZE * (43.75f / 71.f) / 2;
	float pr = g_Player.Position.x + PLAYER_SIZE * (43.75f / 71.f) / 2;
	float pt = g_Player.Position.y - g_Player.size.y / 2;
	float pb = g_Player.Position.y + g_Player.size.y / 2;
	GIMMICK *gimmick = GetGimmick();
	GIMMICK_TYPE ret = GIMMICK_TYPE_NONE;

	for (int i = 0; i < GIMMICK_MAX; i++)
	{
		if (gimmick[i].Enable && !gimmick[i].Get)
		{
			float bl = gimmick[i].Position.x - gimmick[i].SIZE / 2;
			float br = gimmick[i].Position.x + gimmick[i].SIZE / 2;
			float bt = gimmick[i].Position.y - gimmick[i].SIZE / 2;
			float bb = gimmick[i].Position.y + gimmick[i].SIZE / 2;

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

				if ((dy * g_Player.Velocity.y <= 0.0f) || (dx * g_Player.Velocity.x < 0.0f))
				{
					GimmickGet(i);
					ret = gimmick[i].TYPE;
					*index = i;
				}
			}
		}
	}
	return ret;
}

static void EnemyHitCheck(void)
{
	bool stamp = false;
	
	float pl = g_Player.Position.x - PLAYER_SIZE * (43.75f / 71.f) / 2;
	float pr = g_Player.Position.x + PLAYER_SIZE * (43.75f / 71.f) / 2;
	float pt = g_Player.Position.y - g_Player.size.y / 2;
	float pb = g_Player.Position.y + g_Player.size.y / 2;
	ACTION_ENEMY *enemy = GetActionEnemy();

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (enemy[i].Alive)
		{
			float bl = enemy[i].Position.x - enemy[i].size.x / 2;
			float br = enemy[i].Position.x + enemy[i].size.x / 2;
			float bt = enemy[i].Position.y - enemy[i].size.y / 2;
			float bb = enemy[i].Position.y + enemy[i].size.y / 2;

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
					if (dy * g_Player.Velocity.y < 0.0f)
					{
						g_Player.Position.y += dy;
						if (stamp)
						{
							g_Player.Velocity.y = -20.f / g_Status.Weight;
							AddActionEnemyDamage(i, (int)(10 * g_Status.Weight));
							HitStopPlayer(5);
							HitStopActionEnemy(i, 5);
							ShakeCamera(15.f);
						}
						else
						{
							if (g_Player.DamageCnt < 0)
							{
								AddPlayerDamage(1);
							}
						}
					}
				}
				else
				{
					if (dx * g_Player.Velocity.x <= 0.0f)
					{
						g_Player.Position.x += 30 * dx / fabsf(dx);
						g_Player.Velocity.x = 0.0f;

						if (g_Player.DamageCnt < 0)
						{
							AddPlayerDamage(1);
						}
					}
				}
			}
		}
	}
}

static void EnemyBulletHitCheck(void)
{
	float pl = g_Player.Position.x - PLAYER_SIZE * (43.75f / 71.f) / 2;
	float pr = g_Player.Position.x + PLAYER_SIZE * (43.75f / 71.f) / 2;
	float pt = g_Player.Position.y - g_Player.size.y / 2;
	float pb = g_Player.Position.y + g_Player.size.y / 2;
	ENEMY_BULLET *enemy_bullt = GetEnemyBullet();

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (enemy_bullt[i].enable)
		{
			float bl = enemy_bullt[i].pos.x - enemy_bullt[i].size / 2;
			float br = enemy_bullt[i].pos.x + enemy_bullt[i].size / 2;
			float bt = enemy_bullt[i].pos.y - enemy_bullt[i].size / 2;
			float bb = enemy_bullt[i].pos.y + enemy_bullt[i].size / 2;

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
					if (dy * g_Player.Velocity.y < 0.0f)
					{
						if (g_Player.DamageCnt < 0)
						{
							AddPlayerDamage(1);
							BreakEnemyBullet(i);
						}
					}
				}
				else
				{
					if (dx * g_Player.Velocity.x <= 0.0f)
					{
						if (g_Player.DamageCnt < 0)
						{
							AddPlayerDamage(1);
							BreakEnemyBullet(i);
						}
					}
				}
			}
		}
	}
}

static BLOCK_TYPE LiftHitCheck(void)
{
	bool stamp = false;
	/*float pl = g_Player.Position.x - g_Player.size.x / 2;
	float pr = g_Player.Position.x + g_Player.size.x / 2;*/
	float pl = g_Player.Position.x - PLAYER_SIZE * (43.75f / 71.f) / 2;
	float pr = g_Player.Position.x + PLAYER_SIZE * (43.75f / 71.f) / 2;
	float pt = g_Player.Position.y - g_Player.size.y / 2;
	float pb = g_Player.Position.y + g_Player.size.y / 2;
	LIFT *lift = GetLift();
	BLOCK_TYPE ret = BLOCK_TYPE_NONE;

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
					if (dy * g_Player.Velocity.y < 0.0f)
					{
						if (stamp)
						{
							ret = BLOCK_TYPE_NORMAL;
							g_Player.Position.y += dy;
							g_Player.Velocity.y = 0.f;
							/*g_Player.Position.x += lift[i].Velocity.x;
							g_Player.Position.y += lift[i].Velocity.y;*/
							g_Player.Position.x += lift[i].Position.x - lift[i].OldPosition.x;
							g_Player.Position.y += lift[i].Position.y - lift[i].OldPosition.y;

							if (dy < 0)
							{
								g_Player.Jump = false;

								g_Player.AirJump = false;
							}
						}
					}
				}
				else
				{
					if (dx * g_Player.Velocity.x < 0.0f)
					{
						g_Player.Position.x += dx;
						g_Player.Velocity.x = 0.0f;
					}
				}
			}
		}
	}
	return ret;
}

static float CalcResistance(BLOCK_TYPE block_type)
{
	float resistance = 0.05f;
	switch (block_type)
	{
	case BLOCK_TYPE_NORMAL:
		resistance += 0.15f;
		break;
	case BLOCK_TYPE_SOIL:
		resistance += 0.18f;
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

void HitStopPlayer(int TimeCount)
{
	g_Player.HitStopTimeCount = TimeCount;
}
