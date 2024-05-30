#pragma once
#ifndef _SOUND_H_
#define _SOUND_H_

enum SoundID
{
	STRM_BLOOD_BLAST = 0,
	STRM_BO_TTO_HIDAMARI = 1,
	WSD_SHOOT = 2,
	WSD_PYORO1 = 3,
	WSD_HIT3 = 4,
	WSD_EXP = 5,
	WSD_HIT1 = 6,
	WSD_COIN = 7,
	WSD_JUMP01 = 8,
	WSD_JUMP02 = 9,
	WSD_SELECT01 = 10,
	WSD_BOYON = 11,
	WSD_THROW = 12,
	WSD_MAN_YEAH = 13,
	WSD_OPEN = 14,
	WSD_CLOSE = 15,
	WSD_MAX = 16
};

void InitSound(void);

void PlayBGM(SoundID soundId);
void StopBGM();
void SetVolumeBGM(double volume);
void SetVolumeSE(double volume);

void PlaySE(SoundID soundId);

#endif
