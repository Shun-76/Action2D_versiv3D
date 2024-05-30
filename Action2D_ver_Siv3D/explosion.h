#pragma once
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_
typedef struct EXPLOSION
{
	bool Enable;
	Float2 Position;
	float Size;
	int Count;
	int type;
}EXPLOSION;

void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void CreateExplosion(Float2 pos, float size, int type);

#define ENEMY_DAMAGED 0
#define PLAYER_DAMAGED 1
#endif
