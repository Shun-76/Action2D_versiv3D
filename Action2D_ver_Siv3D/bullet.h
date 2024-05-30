#pragma once
#ifndef _BULLET_H_
#define _BULLET_H_
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void CreateBullet(Float2 chara_pos, float vel, float chara_width);
void bullet_shot(Float2 chara_pos, float vel, Float2 bullet_vel, float chara_width, float speed, float size);
bool bullet_hit_check(Float2 chara_pos, float chara_size);

struct BULLET
{
	Float2 pos;
	float vel;
	Float2 bullet_vel;
	float speed;
	float size;
	bool enable;
	int cnt;
	int life;
};

#define BULLET_MAX 100
#define BULLET_RECAST 10

#endif
