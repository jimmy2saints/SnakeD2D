#pragma once
#include "gamestate.h"
#include <dwrite.h>

#ifndef TEXT_SIZE
#define TEXT_SIZE 50.0f
#endif

class StartMenuState :
	public GameState
{
public:
	HRESULT OnRender(ID2D1HwndRenderTarget* renderTarget);
	void OnUpdate();
	void OnDiscardDeviceResources();
	HRESULT OnCreateDeviceResources(ID2D1HwndRenderTarget* renderTarget);
	StartMenuState(HWND windowHandle, ID2D1Factory* factory);
	~StartMenuState(void);

private:
	IDWriteFactory* writeFactory;
	IDWriteTextFormat* textFormat;
	ID2D1SolidColorBrush* idleTextBrush;
	ID2D1SolidColorBrush* activeTextBrush;

	int previousKeyboardState;

	enum START_MENU_OPTION
	{
		START = 1,
		QUIT
	};

	enum keyboardKeys : int 
	{
		UP_ARROW =		1 << 0,
		DOWN_ARROW =	1 << 1,
		ENTER =			1 << 2,
		SPACE =			1 << 3
	};

	START_MENU_OPTION currentMenuOption;

	void HandleInput();
};

