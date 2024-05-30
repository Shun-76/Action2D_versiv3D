# include <Siv3D.hpp>
#include "Sound.h"

typedef struct
{
	bool BGM;
	double volume;
	AssetNameView *audio;
}Sound;
static Sound sound[WSD_MAX];

static AssetNameView
strm_blood_blast = U"strm_blood_blast",
strm_hidamari = U"strm_hidamari",
wsd_shoot = U"wsd_shoot",
wsd_pyoro1 = U"wsd_pyoro1",
wsd_hit_3 = U"wsd_hit_3",
wsd_exp = U"wsd_exp",
wsd_hit1 = U"wsd_hit1",
wsd_coin = U"wsd_coin",
wsd_jump01 = U"wsd_jump01",
wsd_jump02 = U"wsd_jump02",
wsd_select01 = U"wsd_select01",
wsd_boyon = U"wsd_boyon",
wsd_throw = U"wsd_throw",
wsd_man_yeah = U"wsd_man_yeah",
wsd_open = U"wsd_open",
wsd_close = U"wsd_close";

void InitSound(void)
{
	sound[STRM_BLOOD_BLAST].audio = &strm_blood_blast;
	sound[STRM_BLOOD_BLAST].BGM = true;
	AudioAsset::Register(*sound[STRM_BLOOD_BLAST].audio, Audio::Stream, U"Action/Sound/Blood_Blast.wav", Loop::Yes);

	sound[STRM_BO_TTO_HIDAMARI].audio = &strm_hidamari;
	sound[STRM_BO_TTO_HIDAMARI].BGM = true;
	AudioAsset::Register(*sound[STRM_BO_TTO_HIDAMARI].audio, Audio::Stream, U"Action/Sound/bo-tto hidamari.wav", Loop::Yes);

	sound[WSD_SHOOT].audio = &wsd_shoot;
	sound[WSD_SHOOT].BGM = false;
	AudioAsset::Register(*sound[WSD_SHOOT].audio, Audio::Stream, U"Action/Sound/shoot.wav");

	sound[WSD_PYORO1].audio = &wsd_pyoro1;
	sound[WSD_PYORO1].BGM = false;
	AudioAsset::Register(*sound[WSD_PYORO1].audio, Audio::Stream, U"Action/Sound/pyoto1.wav");

	sound[WSD_HIT3].audio = &wsd_hit_3;
	sound[WSD_HIT3].BGM = false;
	AudioAsset::Register(*sound[WSD_HIT3].audio, Audio::Stream, U"Action/Sound/hit3.wav");

	sound[WSD_EXP].audio = &wsd_exp;
	sound[WSD_EXP].BGM = false;
	AudioAsset::Register(*sound[WSD_EXP].audio, Audio::Stream, U"Action/Sound/exp.wav");

	sound[WSD_HIT1].audio = &wsd_hit1;
	sound[WSD_HIT1].BGM = false;
	AudioAsset::Register(*sound[WSD_HIT1].audio, Audio::Stream, U"Action/Sound/hit1.wav");

	sound[WSD_COIN].audio = &wsd_coin;
	sound[WSD_COIN].BGM = false;
	AudioAsset::Register(*sound[WSD_COIN].audio, Audio::Stream, U"Action/Sound/coin.wav");

	sound[WSD_JUMP01].audio = &wsd_jump01;
	sound[WSD_JUMP01].BGM = false;
	AudioAsset::Register(*sound[WSD_JUMP01].audio, Audio::Stream, U"Action/Sound/jump01.wav");

	sound[WSD_JUMP02].audio = &wsd_jump02;
	sound[WSD_JUMP02].BGM = false;
	AudioAsset::Register(*sound[WSD_JUMP02].audio, Audio::Stream, U"Action/Sound/jump02.wav");

	sound[WSD_SELECT01].audio = &wsd_select01;
	sound[WSD_SELECT01].BGM = false;
	AudioAsset::Register(*sound[WSD_SELECT01].audio, Audio::Stream, U"Action/Sound/select01.wav");

	sound[WSD_BOYON].audio = &wsd_boyon;
	sound[WSD_BOYON].BGM = false;
	AudioAsset::Register(*sound[WSD_BOYON].audio, Audio::Stream, U"Action/Sound/boyon.wav");

	sound[WSD_THROW].audio = &wsd_throw;
	sound[WSD_THROW].BGM = false;
	AudioAsset::Register(*sound[WSD_THROW].audio, Audio::Stream, U"Action/Sound/throw.wav");

	sound[WSD_MAN_YEAH].audio = &wsd_man_yeah;
	sound[WSD_MAN_YEAH].BGM = false;
	AudioAsset::Register(*sound[WSD_MAN_YEAH].audio, Audio::Stream, U"Action/Sound/man_yeah.wav");

	sound[WSD_OPEN].audio = &wsd_open;
	sound[WSD_OPEN].BGM = false;
	AudioAsset::Register(*sound[WSD_OPEN].audio, Audio::Stream, U"Action/Sound/Open.wav");

	sound[WSD_CLOSE].audio = &wsd_close;
	sound[WSD_CLOSE].BGM = false;
	AudioAsset::Register(*sound[WSD_CLOSE].audio, Audio::Stream, U"Action/Sound/Close.wav");
}

void PlayBGM(SoundID soundId)
{
	AudioAsset(*sound[soundId].audio).play();
}

void StopBGM()
{
	for (int i = 0; i < WSD_MAX; i++)
	{
		if (sound[i].BGM)
		{
			AudioAsset(*sound[i].audio).stop();
		}
	}
}

void SetVolumeBGM(double volume)
{
	for (int i = 0; i < WSD_MAX; i++)
	{
		if (sound[i].BGM)
		{
			AudioAsset(*sound[i].audio).setVolume(volume);
		}
	}
}

void SetVolumeSE(double volume)
{
	for (int i = 0; i < WSD_MAX; i++)
	{
		if (!sound[i].BGM)
		{
			sound[i].volume = volume;
		}
	}
}

void PlaySE(SoundID soundId)
{
	AudioAsset(*sound[soundId].audio).playOneShot(sound[soundId].volume);
}
