#pragma once
#ifndef _PAUSE_H_
#define _PAUSE_H_
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
void DrawTutorial(void);
bool GetPause(void);
void SetPause(void);

enum ENUM_TUTORIAL
{
	TUTORIAL_TYPE_1 = 0,
	TUTORIAL_TYPE_2 = 1,
	TUTORIAL_TYPE_3 = 2,
	TUTORIAL_TYPE_4 = 3,
	TUTORIAL_TYPE_NONE = 4
};
void CreateTutorial(ENUM_TUTORIAL en_tutorial, float pos_x, float pos_y, float SIZE);

struct PAUSE
{
	AssetNameView* Tex;
	Float2 Position;
	Float2 NextPosition;
	float fill;
};

struct TUTORIAL
{
	bool Enable;
	ENUM_TUTORIAL Tex;
	Float2 Position;
	float SIZE;
	COLOR color;
};
#endif
