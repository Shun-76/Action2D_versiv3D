#pragma once
#ifndef _ENEMY_BULLET_H_
#define _ENEMY_BULLET_H_
struct ENEMY_BULLET
{
	ACTION_ENEMY_TYPE TYPE;
	bool enable;
	Float2 pos;
	Float2 velocity;
	float speed;
	float size;
	int cnt;
	float rad;
	int vector;
};

void InitEnemyBullet(void);
void UninitEnemyBullet(void);
void UpdateEnemyBullet(void);
void DrawEnemyBullet(void);
void CreateEnemyBullet(ACTION_ENEMY_TYPE TYPE, Float2 chara_pos, float chara_width, int vel, bool boss);
void enemy_bullet_shot(ACTION_ENEMY_TYPE TYPE, Float2 chara_pos, Float2 bullet_vel, float chara_width, int vel, bool boss);
void BreakEnemyBullet(int index);

ENEMY_BULLET *GetEnemyBullet(void);
#define ENEMY_BULLET_RECAST 90
#endif
