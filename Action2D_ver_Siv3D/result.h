#pragma once
#ifndef _RESULT_H_
#define _RESULT_H_
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

enum RESULT
{
	RESULT_NONE,
	RESULT_GAMEOVER,
	RESULT_GAMECLEAR
};

struct RESULTSCORE
{
	int point;
	int display_point;
	int coin;
	int display_coin;
	int zanki;
	int display_zanki;
	int total_score;
	int display_total_score;
};

void SetResult(RESULT result);

#endif
