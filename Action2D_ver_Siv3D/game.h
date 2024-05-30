#pragma once
#ifndef _GAME_H_
#define _GAME_H_
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

void InitStage(void);
void InitRandomStage(void);
void UpdateRandomStage(void);
void CreateRandomStageBlock(int point);
void CreateEnemyOnRandomPoint(int point);

enum STAGE_TYPE
{
	FIXED_STAGE,
	RANDOM_STAGE
};
#endif
