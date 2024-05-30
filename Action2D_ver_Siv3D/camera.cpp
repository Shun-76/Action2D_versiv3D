# include <Siv3D.hpp>
#include "Main.h"
#include "Sprite.h"
#include "camera.h"
#include "player.h"

#define CAMERA_MAX_SPEED (2 * PLAYER_SPEED)
#define MAX_OFFSET 120.f
#define pi 3.141592f
static CAMERA g_Camera;
static float radv;

void InitCamera(void)
{
	g_Camera.Position.x = 0;
	g_Camera.Position.y = 0;
	g_Camera.Offset.x = 120.f;
	g_Camera.Offset.y = 0.f;
	g_Camera.ShakeOffset = 0.f;
	g_Camera.TimeCount = 0;
	radv = pi / 3;
}


void UpdateCamera(void)
{
	PLAYER *player = GetPlayer();

	g_Camera.Offset.x += (player->vector * MAX_OFFSET - g_Camera.Offset.x) * 0.05f;

	float x = player->Position.x + g_Camera.Offset.x - g_Camera.Position.x;
	//float y = -g_Camera.Position.y;
	float y = player->Position.y + g_Camera.Offset.y - g_Camera.Position.y;
	float abs_x = fabsf(x);
	
	g_Camera.Position.x += 0.2 * x;
	g_Camera.Position.y += 0.1 * y;

	g_Camera.Position.x += cosf(g_Camera.TimeCount * radv / 2) * g_Camera.ShakeOffset;
	g_Camera.Position.y += sinf(g_Camera.TimeCount * radv) * g_Camera.ShakeOffset;
	g_Camera.ShakeOffset -= g_Camera.ShakeOffset * 0.08f;
	g_Camera.TimeCount++;
}

CAMERA *GetCamera(void)
{
	return &g_Camera;
}
void ShakeCamera(float Offset)
{
	g_Camera.ShakeOffset = Offset;
}
