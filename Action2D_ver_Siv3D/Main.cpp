# include <Siv3D.hpp> // Siv3D v0.6.14
#include "Main.h"
#include "Sound.h"
#include "game.h"
#include "title.h"
#include "result.h"
#include "fade.h"

#define pi 3.141592
#define FPS 60

static SCENE g_Scene, new_Scene;


void Main()
{
	System::SetTerminationTriggers(UserAction::CloseButtonClicked);
	Scene::SetResizeMode(ResizeMode::Virtual);
	Scene::Resize(SCREEN_WIDTH, SCREEN_HEIGHT);
	Scene::SetResizeMode(ResizeMode::Keep);

	Window::Resize(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	Window::SetTitle(U"Action_2D");
	//Window::SetFullscreen(true);

	// 背景の色を設定する | Set the background color
	Scene::SetBackground(ColorF{ 0.2, 0.2, 0.2 });


	InitMain();

	Stopwatch sw;
	sw.start();
	while (System::Update())
	{
		UpdateMain();
		DrawMain();

		if (KeyRShift.pressed()) break;
		while (sw.msF() < (1000.0 / FPS));
		sw.restart();
	}
	UninitMain();
}

//関数名:Init()
//機能:初期化処理
void InitMain(void)
{
	//音声初期化
	InitSound();

	InitFade();

	g_Scene = SCENE_NONE;
	new_Scene = SCENE_NONE;
	SetScene(SCENE_TITLE);
	//SetScene(SCENE_GAME);
}

//関数名:Uninit()
//機能:終了処理
void UninitMain(void)
{
	UninitFade();
	UninitGame();

	UninitTitle();

	System::Exit();
}

//関数名:Update()
//機能:更新処理
void UpdateMain(void)
{
	bool* isFadeEnd = GetFadeEnd();
	if (*isFadeEnd)
	{
		_SetScene(new_Scene);
	}

	switch (g_Scene)
	{
	case SCENE_TITLE:
		UpdateTitle();
		break;
	case SCENE_GAME:
		UpdateGame();
		break;
	case SCENE_RESULT:
		UpdateResult();
		break;
	default:
		break;
	}

	UpdateFade();
}

//関数名:Draw()
//機能:描画処理
void DrawMain(void)
{
	Scene::SetBackground(ColorF{ 1.0, 1.0, 1.0 });

	switch (g_Scene)
	{
	case SCENE_TITLE:
		DrawTitle();
		break;
	case SCENE_GAME:
		DrawGame();
		break;
	case SCENE_RESULT:
		DrawResult();
		break;
	default:
		break;
	}

	DrawFade();
}

void _SetScene(SCENE Scene)
{
	new_Scene = Scene;
	bool isFade = isFadeCheck();
	if (!isFade)
	{
		FadeIn();
	}
}

void SetScene(SCENE Scene)
{
	switch (g_Scene)
	{
	case SCENE_TITLE:
		UninitTitle();
		break;
	case SCENE_GAME:
		UninitGame();
		break;
	case SCENE_RESULT:
		UninitResult();
		break;
	default:
		break;
	}

	g_Scene = Scene;

	switch (g_Scene)
	{
	case SCENE_TITLE:
		InitTitle();
		break;
	case SCENE_GAME:
		InitGame();
		break;
	case SCENE_RESULT:
		InitResult();
		break;
	default:
		break;
	}
	bool isFade = isFadeCheck();
	if (!isFade)
	{
		FadeOut();
	}
}

void SceneChange(void)
{
	SetScene(new_Scene);
}
