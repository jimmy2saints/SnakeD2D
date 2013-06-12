#pragma once
#include "GameState.h"
#include <dwrite.h>

#ifndef TEXT_SIZE
#define TEXT_SIZE 50.0f
#endif

class StartMenuState :
	public GameState
{
public:

	StartMenuState(HWND windowHandle, ID2D1Factory* factory, Game* gamePtr);
	~StartMenuState(void);

private:
	enum START_MENU_OPTION
	{
		START = 1,
		QUIT
	};
	IDWriteFactory* writeFactory;
	IDWriteTextFormat* textFormat;
	ID2D1SolidColorBrush* idleTextBrush;
	ID2D1SolidColorBrush* activeTextBrush;
	START_MENU_OPTION currentMenuOption;

	HRESULT OnRender(ID2D1HwndRenderTarget* renderTarget);
	void OnUpdate();
	void OnDiscardDeviceResources();
	HRESULT OnCreateDeviceResources(ID2D1HwndRenderTarget* renderTarget);
	void OnInput(int keysChanged, int keysDown, int keysUp);
};

