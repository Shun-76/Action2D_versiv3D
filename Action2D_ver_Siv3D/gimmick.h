#pragma once
#ifndef _GIMMICK_H_
#define _GIMMICK_H_
enum GIMMICK_TYPE
{
	GIMMICK_TYPE_COIN,
	GIMMICK_TYPE_BOUNCE,
	GIMMICK_TYPE_KEY,
	GIMMICK_TYPE_DOOR,
	GIMMICK_TYPE_SPIKE,
	GIMMICK_TYPE_CHECK_POINT,
	GIMMICK_TYPE_NONE
};

struct GIMMICK
{
	GIMMICK_TYPE TYPE;
	bool Enable;
	bool Get;
	int Shop;
	Float2 Position;
	Float2 Velocity;
	Float2 Scale;
	float Scale_Offset;
	float SIZE;
	int cnt_anim;
	int cnt_get;
	COLOR color;
};

#define GIMMICK_MAX 1024
#define GIMMICK_SIZE 35.0f
#define TRAP_SIZE 50.f

void InitGimmick(void);
void UninitGimmick(void);
void UpdateGimmick(void);
void DrawGimmick(void);

GIMMICK* GetGimmick(void);
void CreateGimmick(GIMMICK_TYPE TYPE, float pos_x, float pos_y, float SIZE, int Shop);
void CreateGimmickRectangle(GIMMICK_TYPE TYPE, float pos_x, float pos_y, float SIZE, int w, int h);
void DeleteGimmick(int index);
void GimmickGet(int index);
void failedDoorOpen(int index);
#endif
