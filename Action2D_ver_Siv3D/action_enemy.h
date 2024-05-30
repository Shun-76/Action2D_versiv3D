#pragma once
#ifndef _ACTION_ENEMY_H_
#define _ACTION_ENEMY_H_
enum ACTION_ENEMY_TYPE
{
	ENEMY_TYPE_1,
	ENEMY_TYPE_2,
	ENEMY_TYPE_3,
	ENEMY_TYPE_MAX
};
 
struct ACTION_ENEMY
{
	ACTION_ENEMY_TYPE TYPE;
	bool Enable;
	bool Set;
	bool Move;
	bool Invincible;
	bool Alive;
	bool Boss;
	bool Landing;
	Float2 Position;
	Float2 SetPosition;
	Float2 Velocity;
	Float2 size;
	int Life;
	int Max_Life;
	int vector;
	int Count;
	int HitStopCount;
	float rad;
};

void InitActionEnemy(void);
void UninitActionEnemy(void);
void UpdateActionEnemy(void);
void DrawActionEnemy(void);
void CreateActionEnemy(ACTION_ENEMY_TYPE TYPE, float x, float y, int Life, int vec, bool boss);
void DeleteActionEnemy(int index);
ACTION_ENEMY* GetActionEnemy(void);
void AddActionEnemyDamage(int index, int damage);
void RespawnActionEnemy(void);

void HitStopActionEnemy(int index, int TimeCount);

#define ENEMY_MAX 50
#define ENEMY_TYPE_COUNT ENEMY_TYPE_MAX
#define ENEMY_SPEED 4.f

#endif
