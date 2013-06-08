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

class Game
{
public:
	HRESULT Initialize();
	HRESULT Update();
	void OnResize(UINT width, UINT height);

	Game(HWND windowsHandle);
	~Game(void);

private:
	HWND hWnd;
	ID2D1Factory* direct2dFactory;
	GameState* state;
};

