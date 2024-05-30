# include <Siv3D.hpp>
#include "Main.h"
#include "Sprite.h"
#include "block.h"
#include "camera.h"
#include "player.h"
#include "action_enemy.h"
#include "lift.h"

#define BLOCK_TYPE_CNT BLOCK_TYPE_NONE
static AssetNameView block_normal = U"block_normal", block_soil = U"block_soil", block_ice = U"block_ice";
static AssetNameView* g_block[BLOCK_TYPE_CNT]=
{
	&block_normal,
	&block_soil,
	&block_ice
};
static BLOCK g_Block[BLOCK_MAX];

void InitBlock(void)
{
	TextureAsset::Register(*g_block[BLOCK_TYPE_NORMAL], U"Action/Texture/block.tga");
	TextureAsset::Register(*g_block[BLOCK_TYPE_SOIL], U"Action/Texture/soil_block.tga");
	TextureAsset::Register(*g_block[BLOCK_TYPE_ICE], U"Action/Texture/ice_block.tga");

	for (int i = 0; i < BLOCK_MAX; i++)
	{
		g_Block[i].TYPE = BLOCK_TYPE_NORMAL;
		g_Block[i].Enable = false;
		g_Block[i].Position.x = 0;
		g_Block[i].Position.y = 0;
		g_Block[i].SIZE.x = 0;
		g_Block[i].SIZE.y = 0;
		g_Block[i].Left = NULL;
		g_Block[i].Right = NULL;
	}
}

void UninitBlock(void)
{
	for (int i = 0; i < BLOCK_TYPE_CNT; i++)
	{
		TextureAsset::Release(*g_block[i]);
	}
}

void UpdateBlock(void)
{

}

void DrawBlock(void)
{
	CAMERA *camera = GetCamera();
	for (int i = 0; i < BLOCK_MAX; i++)
	{
		if (g_Block[i].Enable && (g_Block[i].TYPE == BLOCK_TYPE_NORMAL || g_Block[i].TYPE == BLOCK_TYPE_SOIL || g_Block[i].TYPE == BLOCK_TYPE_ICE))
		{
			DrawSpriteQuad(*g_block[g_Block[i].TYPE],
				g_Block[i].Position.x - camera->Position.x, g_Block[i].Position.y - camera->Position.y, g_Block[i].SIZE.x, g_Block[i].SIZE.y,
				1.f, 1.f, 1.f, 1.f,
				0.f, 0.f, 1.0f, 1.0f);
		}
	}
}

BLOCK* GetBlock(void)
{
	return g_Block;
}

int CreateBlock(BLOCK_TYPE TYPE, float pos_x, float pos_y, float SIZE_X, float SIZE_Y)
{
	for (int i = 0; i < BLOCK_MAX; i++)
	{
		if (g_Block[i].Enable == false)
		{
			g_Block[i].Enable = true;
			g_Block[i].Position.x = pos_x;
			g_Block[i].Position.y = pos_y;
			g_Block[i].SIZE.x = SIZE_X;
			g_Block[i].SIZE.y = SIZE_Y;
			g_Block[i].TYPE = TYPE;
			g_Block[i].Left = NULL;
			g_Block[i].Right = NULL;
			return i;
		}
	}
	return -1;
}

void CreateBlockRectangle(BLOCK_TYPE TYPE, float pos_x, float pos_y, float SIZE_X, float SIZE_Y, int w, int h)
{
	Float2 pos;
	pos.x = pos_x;
	pos.y = pos_y;
	int index, before_index;
	for (int i = 0; i < h; i++)
	{
		before_index = 0;
		index = 0;
		for (int j = 0; j < w; j++)
		{
			index = CreateBlock(TYPE, pos.x, pos.y, SIZE_X, SIZE_Y);
			pos.x += SIZE_X;
			if (j > 0 && j < w)
			{
				g_Block[index].Left = &g_Block[before_index];
				g_Block[before_index].Right = &g_Block[index];
			}
			before_index = index;
		}
		pos.x = pos_x;
		pos.y += SIZE_Y;
	}
}

void CreateBlockStairs(BLOCK_TYPE TYPE, float pos_x, float pos_y, float SIZE_X, float SIZE_Y, int Count, STAIRS_VEL vel)
{
	Float2 pos;
	switch (vel)
	{
	case STAIRS_UP:
		pos.x = pos_x;
		pos.y = pos_y;
		for (int i = 0; i < Count; i++)
		{
			CreateBlockRectangle(TYPE, pos.x, pos.y, SIZE_X, SIZE_Y, 1, i + 1);
			pos.x += SIZE_X;
			pos.y -= SIZE_Y;
		}
		break;
	case STAIRS_DOWN:
		pos.x = pos_x;
		pos.y = pos_y - SIZE_Y * (Count - 1);
		for (int i = 0; i < Count; i++)
		{
			CreateBlockRectangle(TYPE, pos.x, pos.y, SIZE_X, SIZE_Y, 1, Count - i);
			pos.x += SIZE_X;
			pos.y += SIZE_Y;
		}
		break;
	}
}

void DeleteBlock(int index)
{
	g_Block[index].Enable = false;
}
