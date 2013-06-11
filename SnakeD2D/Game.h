#pragma once

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <math.h>

#include "safe_release.h"
#include "debugging.h"
#include "GameState.h"
#include "StartMenuState.h"
#include "PlayingGameState.h"
#include "game_state.h"


class Game
{
public:
	HRESULT Initialize();
	HRESULT Update();
	void OnResize(UINT width, UINT height);
	void ChangeGameState(GAME_STATE new_state);

	Game(HWND windowsHandle);
	~Game(void);

private:
	HWND hWnd;
	ID2D1Factory* direct2dFactory;
	GameState* state;
	GAME_STATE gameState;
	GAME_STATE oldGameState;

	int fpsCounter;
	int ticksCounter;
	int ticksSinceLastFrame;
	int ticksLastFrame;
	int fps;

	void UpdateGameState();
	void UpdateFps();
};

