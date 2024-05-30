#pragma once
#ifndef _FADE_H_
#define _FADE_H_
enum FADE
{
	FADE_NONE,
	FADE_IN,
	FADE_OUT
};

void InitFade(void);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);

void FadeIn(void);
void FadeOut(void);
bool isFadeCheck(void);
FADE GetFadeMode(void);
bool *GetFadeEnd(void);
#endif
