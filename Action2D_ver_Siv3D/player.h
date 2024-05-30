#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_
struct COLOR
{
	float r;
	float g;
	float b;
	float a;
};

struct PLAYER
{
	Float2 Position;
	Float2 Velocity;
	Float2 Scale;
	COLOR Color;
	int vector;
	int Life;
	Float2 size;
	Float2 CheckPoint;
	bool Jump;
	bool AirJump;
	bool Enable;
	bool Alive;
	bool Key;
	bool DoorOpen;
	int Zanki;
	int Count;
	int DamageCnt;
	int HitStopTimeCount;
	int DeathCount;
};

struct PLAYER_STATUS
{
	float Speed;
	float Jump;
	float Attack;
	float Weight;
};

void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER* GetPlayer(void);
int GetPlayerStageBlock(void);
void respawnPlayer(void);
void AddPlayerDamage(int damage);

void HitStopPlayer(int TimeCount);

#define VEL_RIGHT 1
#define VEL_LEFT -1
#define PLAYER_SPEED 13.0f
#define PLAYER_SIZE 100.0f
#define DEATH_CNT_MAX 70
#endif
