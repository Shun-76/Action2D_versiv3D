#pragma once
#ifndef _SCORE_H_
#define _SCORE_H_
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);

struct SCORE
{
	float size;
	int time;
	int count;
	int point;
	int display_point;
	int checkpoint;
	int distance;
};

void AddScore(int Count);
void AddPoint(int Count);
SCORE *getScore(void);
void checkpointScore(void);
void respawnScore(void);
#endif
