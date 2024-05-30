#pragma once
#ifndef _MAIN_H_
#define _MAIN_H_

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

enum SCENE
{
	SCENE_NONE,
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_RESULT
};

void InitMain(void);
void UninitMain(void);
void UpdateMain(void);
void DrawMain(void);
void SetScene(SCENE Scene);
void _SetScene(SCENE Scene);
void SceneChange(void);

#endif
