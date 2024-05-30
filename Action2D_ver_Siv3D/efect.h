#pragma once
#ifndef _EFECT_H_
#define _EFECT_H_
enum EFECT_TYPE
{
	EFECT_TYPE_SMOKE,
	EFECT_TYPE_DEATH,
	EFECT_TYPE_KIRAKIRA,
	EFECT_TYPE_JUMP,
	EFECT_TYPE_COUNT
};
struct EFECT
{
	EFECT_TYPE TYPE;
	Float2 Position;
	Float2 Velocity;
	int Count;
	Float2 size;
	bool Enable;
	int vel;
	COLOR Color;
};

void InitEfect(void);
void UninitEfect(void);
void UpdateEfect(void);
void DrawEfect(void);

void CreateSmoke(float Position_x, float Position_y, int vel, float size);
void CreateDeathEfect(float Position_x, float Position_y, bool fall);
void CreateKirakira(float Position_x, float Position_y);
void CreateJump(float Position_x, float Position_y);

#define EFECT_MAX 40
#define EFECT_CNT_MAX 15
#define EFECT_SIZE 50

#endif
