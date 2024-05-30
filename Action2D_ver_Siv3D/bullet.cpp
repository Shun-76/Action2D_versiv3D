# include <Siv3D.hpp>
#include "Main.h"
#include "Sprite.h"
#include "player.h"
#include "action_enemy.h"
#include "bullet.h"
#include "sound.h"
#include "explosion.h"
#include "camera.h"
#include "block.h"

#define BULLET_SPEED 50.0f
#define BULLET_CNT_MAX 40
#define BULLET_SIZE 60.0f
#define pi 3.141592f

static AssetNameView g_bullet = U"bullet_shuriken";
static float rad_v, rad;

static BULLET g_Player_bullet[BULLET_MAX];

static BLOCK_TYPE BlockHitCheck(int index);

void InitBullet(void)
{
	TextureAsset::Register(g_bullet, U"Action/Texture/shuriken.tga");
	rad = 0.f;
	rad_v = 0.05f;

	for (int i = 0; i < BULLET_MAX; i++)
	{
		g_Player_bullet[i].pos.x = 0.0f;
		g_Player_bullet[i].pos.y = 0.0f;
		g_Player_bullet[i].enable = false;
		g_Player_bullet[i].size = BULLET_SIZE;
		g_Player_bullet[i].cnt = 0;
	}
}

void UninitBullet(void)
{
	TextureAsset::Release(g_bullet);
}

void UpdateBullet(void)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Player_bullet[i].enable)
		{
			if (g_Player_bullet[i].vel > 0)
			{
				g_Player_bullet[i].pos.x += g_Player_bullet[i].bullet_vel.x * g_Player_bullet[i].speed;
			}
			else
			{
				g_Player_bullet[i].pos.x -= g_Player_bullet[i].bullet_vel.x * g_Player_bullet[i].speed;
			}
			g_Player_bullet[i].pos.y += g_Player_bullet[i].bullet_vel.y * g_Player_bullet[i].speed;
			g_Player_bullet[i].cnt++;

			if (g_Player_bullet[i].cnt > BULLET_CNT_MAX)
			{
				g_Player_bullet[i].enable = false;
				g_Player_bullet[i].cnt = 0;
			}
			ACTION_ENEMY* enemy = GetActionEnemy();
			for (int j = 0; j < ENEMY_MAX; j++)
			{
				if (enemy[j].Alive)
				{
					float dx = enemy[j].Position.x - g_Player_bullet[i].pos.x;
					float dy = enemy[j].Position.y - g_Player_bullet[i].pos.y;

					if (dx * dx + dy * dy < (enemy[j].size.x * enemy[j].size.y / 4))
					{
						AddActionEnemyDamage(j, 10);
						g_Player_bullet[i].life--;
						//CreateExplosion(g_Player_bullet[i].pos, g_Player_bullet[i].size, ENEMY_DAMAGED);
						if (enemy[j].Life > 0) PlaySE(WSD_HIT1);
					}
				}
			}
			if (g_Player_bullet[i].life <= 0)g_Player_bullet[i].enable = false;
		}
		BlockHitCheck(i);
	}
	rad += rad_v;
}

void DrawBullet(void)
{
	CAMERA *camera = GetCamera();
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Player_bullet[i].enable)
		{
			DrawSpriteQuad(g_bullet,
				g_Player_bullet[i].pos.x - camera->Position.x, g_Player_bullet[i].pos.y - camera->Position.y, g_Player_bullet[i].size, g_Player_bullet[i].size,
				1.f, 1.f, 1.f, 1.f,
				0.f, 0.f, 1.0f, 1.0f);
		}
	}
}

void CreateBullet(Float2 chara_pos, float vel, float chara_width)
{
	Float2 bullet_vel;

	bullet_vel.x = 1.f;
	bullet_vel.y = 0.f;

	bullet_shot(chara_pos, vel, bullet_vel, chara_width, BULLET_SPEED, BULLET_SIZE);
}

void bullet_shot(Float2 chara_pos, float vel, Float2 bullet_vel, float chara_width, float speed, float size)
{
	for (int num = 0; num < BULLET_MAX; num++)
	{
		if (g_Player_bullet[num].enable == false)
		{
			g_Player_bullet[num].vel = vel;
			if (vel > 0)
			{
				g_Player_bullet[num].pos.x = chara_pos.x + chara_width * bullet_vel.x / 2;
			}
			else if (vel < 0)
			{
				g_Player_bullet[num].pos.x = chara_pos.x - chara_width / 2;
			}
			g_Player_bullet[num].bullet_vel.x = bullet_vel.x;
			g_Player_bullet[num].bullet_vel.y = bullet_vel.y;
			g_Player_bullet[num].speed = speed;
			g_Player_bullet[num].pos.y = chara_pos.y + chara_width * bullet_vel.y / 2;
			g_Player_bullet[num].enable = true;
			g_Player_bullet[num].life = 1;
			g_Player_bullet[num].size = size;
			g_Player_bullet[num].cnt = 0;
			break;
		}
	}
}

bool bullet_hit_check(Float2 chara_pos, float chara_size)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Player_bullet[i].enable)
		{
			if ((g_Player_bullet[i].pos.x > (chara_pos.x - (chara_size + g_Player_bullet[i].size) / 2))
				&& (g_Player_bullet[i].pos.x < (chara_pos.x + (chara_size + g_Player_bullet[i].size) / 2))
				&& (g_Player_bullet[i].pos.y > (chara_pos.y - (chara_size + g_Player_bullet[i].size) / 2))
				&& (g_Player_bullet[i].pos.y < (chara_pos.y + (chara_size + g_Player_bullet[i].size) / 2)))
			{
				g_Player_bullet[i].life--;
				return true;
			}
		}
	}
	return false;
}

static BLOCK_TYPE BlockHitCheck(int index)
{
	float pl = g_Player_bullet[index].pos.x - g_Player_bullet[index].size / 2;
	float pr = g_Player_bullet[index].pos.x + g_Player_bullet[index].size / 2;
	float pt = g_Player_bullet[index].pos.y - g_Player_bullet[index].size / 2;
	float pb = g_Player_bullet[index].pos.y + g_Player_bullet[index].size / 2;
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
					if (dy * g_Player_bullet[index].bullet_vel.y < 0.0f)
					{
						g_Player_bullet[index].pos.y += dy;
						g_Player_bullet[index].bullet_vel.y = 0.0f;
						
						
						ret = block[i].TYPE;
					}
				}
				else
				{
					if (dx * g_Player_bullet[index].bullet_vel.x * g_Player_bullet[index].vel < 0.0f)
					{
						g_Player_bullet[index].pos.x += dx;
						g_Player_bullet[index].bullet_vel.x = 0.0f;
					}
				}
			}
		}
	}
	return ret;
}
