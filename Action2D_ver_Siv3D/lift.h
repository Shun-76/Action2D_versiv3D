#pragma once
#ifndef _LIFT_H_
#define _LIFT_H_
enum LIFT_TYPE
{
	LIFT_TYPE_NORMAL,
	LIFT_TYPE_NONE
};

enum LIFT_VEL
{
	LIFT_ONE_WAY_UP,
	LIFT_ONE_WAY_DOWN,
	LIFT_CYCLIC
};

struct LIFT
{
	LIFT_TYPE TYPE;
	LIFT_VEL Vel;
	bool Enable;
	Float2 Position;
	Float2 OldPosition;
	Float2 SIZE;
	Float2 Center;
	Float2 Radius;
	int Timecount;
	bool Oneway_next;
};

#define LIFT_MAX 100
#define LIFT_SIZE_X 120.0f
#define LIFT_SIZE_Y 30.0f

void InitLift(void);
void UninitLift(void);
void UpdateLift(void);
void DrawLift(void);

LIFT* GetLift(void);
int CreateLift(LIFT_TYPE TYPE, float pos_x, float pos_y, float SIZE_X, float SIZE_Y, float rad_x, float rad_y, int offset);
int CreateLiftOneWay(LIFT_TYPE TYPE, float pos_x, float pos_y, float SIZE_X, float SIZE_Y, LIFT_VEL vel);
#endif
