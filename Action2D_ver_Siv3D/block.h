#pragma once
#ifndef _BLOCK_H_
#define _BLOCK_H_
enum BLOCK_TYPE
{
	BLOCK_TYPE_NORMAL,
	BLOCK_TYPE_SOIL,
	BLOCK_TYPE_ICE,
	BLOCK_TYPE_NONE
};

enum STAIRS_VEL
{
	STAIRS_UP,
	STAIRS_DOWN
};

struct BLOCK
{
	BLOCK_TYPE TYPE;
	bool Enable;
	Float2 Position;
	Float2 SIZE;
	BLOCK *Left;
	BLOCK *Right;
};

#define BLOCK_MAX 1024
#define RANDOM_STAGE_BLOCK (SCREEN_WIDTH / 2)
#define BLOCK_SIZE 100.0f

void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);

BLOCK* GetBlock(void);
int CreateBlock(BLOCK_TYPE TYPE, float pos_x, float pos_y, float SIZE_X, float SIZE_Y);
void CreateBlockRectangle(BLOCK_TYPE TYPE, float pos_x, float pos_y, float SIZE_X, float SIZE_Y, int w, int h);
void CreateBlockStairs(BLOCK_TYPE TYPE, float pos_x, float pos_y, float SIZE_X, float SIZE_Y, int Count, STAIRS_VEL vel);

void DeleteBlock(int index);
#endif
