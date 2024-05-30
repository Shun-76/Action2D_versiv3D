# include <Siv3D.hpp>
#include "Main.h"
#include "Sprite.h"
#include "lift.h"
#include "camera.h"
#include "player.h"
#include "action_enemy.h"

#define LIFT_TYPE_CNT LIFT_TYPE_NONE
#define pi 3.141592f
#define LIFT_RAD_SPEED (pi / 120)
#define LIFT_SPEED 5.f

static AssetNameView lift_normal = U"lift_normal";
static AssetNameView* g_lift[LIFT_TYPE_CNT];
static LIFT g_Lift[LIFT_MAX];

static void DeleteLift(int index);

void InitLift(void)
{
	g_lift[LIFT_TYPE_NORMAL] = &lift_normal;
	TextureAsset::Register(*g_lift[LIFT_TYPE_NORMAL], U"Action/Texture/lift.tga");

	for (int i = 0; i < LIFT_MAX; i++)
	{
		g_Lift[i].Enable = false;
	}
}

void UninitLift(void)
{
	for (int i = 0; i < LIFT_TYPE_CNT; i++)
	{
		TextureAsset::Release(*g_lift[i]);
	}
}

void UpdateLift(void)
{
	CAMERA *camera = GetCamera();
	for (int i = 0; i < LIFT_MAX; i++)
	{
		if (g_Lift[i].Enable == true)
		{
			g_Lift[i].OldPosition.x = g_Lift[i].Position.x;
			g_Lift[i].OldPosition.y = g_Lift[i].Position.y;
			switch (g_Lift[i].Vel)
			{
			case LIFT_CYCLIC:
				g_Lift[i].Position.x = g_Lift[i].Center.x + cosf(g_Lift[i].Timecount * LIFT_RAD_SPEED) * g_Lift[i].Radius.x;
				g_Lift[i].Position.y = g_Lift[i].Center.y + sinf(g_Lift[i].Timecount * LIFT_RAD_SPEED) * g_Lift[i].Radius.y;
				break;
			case LIFT_ONE_WAY_UP:
				g_Lift[i].Position.y -= LIFT_SPEED;
				if ((g_Lift[i].Position.y < -(SCREEN_HEIGHT + LIFT_SIZE_Y) / 2) && g_Lift[i].Oneway_next != true)
				{
					CreateLiftOneWay(LIFT_TYPE_NORMAL, g_Lift[i].Position.x, (SCREEN_HEIGHT + LIFT_SIZE_Y) / 2, LIFT_SIZE_X, LIFT_SIZE_Y, LIFT_ONE_WAY_UP);
					g_Lift[i].Oneway_next = true;
				}
				break;
			case LIFT_ONE_WAY_DOWN:
				g_Lift[i].Position.y += LIFT_SPEED;
				if ((g_Lift[i].Position.y > (SCREEN_HEIGHT + LIFT_SIZE_Y) / 2) && g_Lift[i].Oneway_next != true)
				{
					CreateLiftOneWay(LIFT_TYPE_NORMAL, g_Lift[i].Position.x, -(SCREEN_HEIGHT + LIFT_SIZE_Y) / 2, LIFT_SIZE_X, LIFT_SIZE_Y, LIFT_ONE_WAY_DOWN);
					g_Lift[i].Oneway_next = true;
				}
				break;
			default:
				break;
			}
			g_Lift[i].Timecount++;
			if (((g_Lift[i].Position.x - camera->Position.x ) < - 5 * SCREEN_WIDTH) || ((g_Lift[i].Position.x - camera->Position.x) > 10 * SCREEN_WIDTH) || ((g_Lift[i].Position.y - camera->Position.y) < -SCREEN_WIDTH) || ((g_Lift[i].Position.y - camera->Position.y) > SCREEN_WIDTH))
			{
				DeleteLift(i);
			}
		}
	}
}

void DrawLift(void)
{
	CAMERA *camera = GetCamera();
	for (int i = 0; i < LIFT_MAX; i++)
	{
		if (g_Lift[i].Enable)
		{
			DrawSpriteQuad(*g_lift[g_Lift[i].TYPE],
				g_Lift[i].Position.x - camera->Position.x, g_Lift[i].Position.y - camera->Position.y, g_Lift[i].SIZE.x, g_Lift[i].SIZE.y,
				1.f, 1.f, 1.f, 1.f,
				0.f, 0.f, 1.0f, 1.0f);
		}
	}
}

LIFT* GetLift(void)
{
	return g_Lift;
}

int CreateLift(LIFT_TYPE TYPE, float pos_x, float pos_y, float SIZE_X, float SIZE_Y, float rad_x, float rad_y, int offset)
{
	for (int i = 0; i < LIFT_MAX; i++)
	{
		if (g_Lift[i].Enable == false)
		{
			g_Lift[i].Enable = true;
			g_Lift[i].TYPE = TYPE;
			g_Lift[i].Position.x = pos_x;
			g_Lift[i].Position.y = pos_y;
			g_Lift[i].OldPosition.x = pos_x;
			g_Lift[i].OldPosition.y = pos_y;
			g_Lift[i].Center.x = pos_x;
			g_Lift[i].Center.y = pos_y;
			g_Lift[i].SIZE.x = SIZE_X;
			g_Lift[i].SIZE.y = SIZE_Y;
			g_Lift[i].Radius.x = rad_x;
			g_Lift[i].Radius.y = rad_y;
			g_Lift[i].Vel = LIFT_CYCLIC;
			g_Lift[i].Timecount = offset;
			return i;
		}
	}
	return -1;
}

int CreateLiftOneWay(LIFT_TYPE TYPE, float pos_x, float pos_y, float SIZE_X, float SIZE_Y, LIFT_VEL vel)
{
	for (int i = 0; i < LIFT_MAX; i++)
	{
		if (g_Lift[i].Enable == false)
		{
			g_Lift[i].Enable = true;
			g_Lift[i].TYPE = TYPE;
			g_Lift[i].Position.x = pos_x;
			g_Lift[i].Position.y = pos_y;
			g_Lift[i].OldPosition.x = pos_x;
			g_Lift[i].OldPosition.y = pos_y;
			g_Lift[i].Center.x = pos_x;
			g_Lift[i].Center.y = pos_y;
			g_Lift[i].SIZE.x = SIZE_X;
			g_Lift[i].SIZE.y = SIZE_Y;
			g_Lift[i].Radius.x = 0.f;
			g_Lift[i].Radius.y = 0.f;
			g_Lift[i].Vel = vel;
			g_Lift[i].Timecount = 0;
			g_Lift[i].Oneway_next = false;
			return i;
		}
	}
	return -1;
}

static void DeleteLift(int index)
{
	g_Lift[index].Enable = false;
}
