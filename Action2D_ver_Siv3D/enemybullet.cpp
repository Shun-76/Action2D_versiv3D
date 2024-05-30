# include <Siv3D.hpp>
#include "Main.h"
#include "Sprite.h"
#include "bullet.h"
#include "player.h"
#include "action_enemy.h"
#include "enemybullet.h"
#include "sound.h"
#include "explosion.h"
#include "camera.h"

#define ENEMY_BULLET_SPEED 25.0f
#define BULLET_CNT_MAX 80
#define ENEMY_BULLET_SIZE 50.0f

static AssetNameView enemy_bullet_type_1 = U"\enemy_bullet_type_1", enemy_bullet_type_2 = U"\enemy_bullet_type_2", enemy_bullet_type_3 = U"\enemy_bullet_type_3";
static AssetNameView* g_bullet_enemy[ENEMY_TYPE_COUNT] =
{
	&enemy_bullet_type_1,
	&enemy_bullet_type_2,
	&enemy_bullet_type_3
};

static ENEMY_BULLET g_Enemy_bullet[BULLET_CNT_MAX];

void InitEnemyBullet(void)
{
	TextureAsset::Register(*g_bullet_enemy[ENEMY_TYPE_1], U"Action/Texture/tekinotama.tga");
	TextureAsset::Register(*g_bullet_enemy[ENEMY_TYPE_2], U"Action/Texture/bosunotama.tga");
	TextureAsset::Register(*g_bullet_enemy[ENEMY_TYPE_3], U"Action/Texture/axe.tga");

	for (int i = 0; i < BULLET_CNT_MAX; i++)
	{
		g_Enemy_bullet[i].pos.x = 0.0f;
		g_Enemy_bullet[i].pos.y = 0.0f;
		g_Enemy_bullet[i].enable = false;
		g_Enemy_bullet[i].cnt = 0;
		g_Enemy_bullet[i].rad = 0.f;
	}
}

void UninitEnemyBullet(void)
{
	for (int i = 0; i < ENEMY_TYPE_COUNT; i++)
	{
		TextureAsset::Release(*g_bullet_enemy[ENEMY_TYPE_3]);
	}
}

void UpdateEnemyBullet(void)
{
	for (int i = 0; i < BULLET_CNT_MAX; i++)
	{
		if (g_Enemy_bullet[i].enable)
		{
			if (g_Enemy_bullet[i].pos.y > SCREEN_HEIGHT / 2)
			{
				g_Enemy_bullet[i].enable = false;
			}
			g_Enemy_bullet[i].cnt++;
			g_Enemy_bullet[i].rad += 0.09f;
			g_Enemy_bullet[i].velocity.y += 1.0f;
			
			g_Enemy_bullet[i].pos.x += g_Enemy_bullet[i].velocity.x;
			g_Enemy_bullet[i].pos.y += g_Enemy_bullet[i].velocity.y;
		}
	}
}

void DrawEnemyBullet(void)
{
	CAMERA *camera = GetCamera();
	for (int i = 0; i < BULLET_CNT_MAX; i++)
	{
		if (g_Enemy_bullet[i].enable)
		{
			if (g_Enemy_bullet[i].vector == VEL_RIGHT)
			{
				DrawSpriteQuad(
					*g_bullet_enemy[g_Enemy_bullet[i].TYPE],
					g_Enemy_bullet[i].pos.x - camera->Position.x, g_Enemy_bullet[i].pos.y - camera->Position.y,
					g_Enemy_bullet[i].size, g_Enemy_bullet[i].size,
					1.f, 1.f, 1.f, 1.f,
					0.f, 0.f, 1.0f, 1.0f,
					false, g_Enemy_bullet[i].rad * g_Enemy_bullet[i].vector);
			}
			else
			{
				DrawSpriteQuad(
					*g_bullet_enemy[g_Enemy_bullet[i].TYPE],
					g_Enemy_bullet[i].pos.x - camera->Position.x, g_Enemy_bullet[i].pos.y - camera->Position.y,
					g_Enemy_bullet[i].size, g_Enemy_bullet[i].size,
					1.f, 1.f, 1.f, 1.f,
					0.f, 0.f, 1.0f, 1.0f,
					true, g_Enemy_bullet[i].rad * g_Enemy_bullet[i].vector);
			}
		}
	}
}

void CreateEnemyBullet(ACTION_ENEMY_TYPE TYPE, Float2 chara_pos, float chara_width, int vel, bool boss)
{
	Float2 bullet_vel;
	int angle = rand() % 4;
	float pi = 3.141592;
	float sh = pi * (6 + angle) / 20;
	bullet_vel.x = vel * cosf(sh) * ENEMY_BULLET_SPEED;
	bullet_vel.y = -1 * sinf(sh) * ENEMY_BULLET_SPEED;
	enemy_bullet_shot(TYPE, chara_pos, bullet_vel, chara_width, vel, boss);
}

void enemy_bullet_shot(ACTION_ENEMY_TYPE TYPE, Float2 chara_pos, Float2 bullet_vel, float chara_width, int vel, bool boss)
{
	for (int i = 0; i < BULLET_CNT_MAX; i++)
	{
		if (g_Enemy_bullet[i].enable == false)
		{
			g_Enemy_bullet[i].TYPE = TYPE;
			g_Enemy_bullet[i].pos.x = chara_pos.x + vel * chara_width / 2;
			g_Enemy_bullet[i].pos.y = chara_pos.y;
			g_Enemy_bullet[i].velocity.x = bullet_vel.x;
			g_Enemy_bullet[i].velocity.y = bullet_vel.y;
			g_Enemy_bullet[i].size = ENEMY_BULLET_SIZE;
			g_Enemy_bullet[i].enable = true;
			g_Enemy_bullet[i].cnt = 0;
			g_Enemy_bullet[i].vector = vel;

			if (boss)
			{
				g_Enemy_bullet[i].size *= 2;
			}
			break;
		}
	}
}

ENEMY_BULLET *GetEnemyBullet(void)
{
	return g_Enemy_bullet;
}

void BreakEnemyBullet(int index)
{
	g_Enemy_bullet[index].enable = false;
	CreateExplosion(g_Enemy_bullet[index].pos, g_Enemy_bullet[index].size, ENEMY_DAMAGED);
}
