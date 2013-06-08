#pragma once
#include "gamestate.h"
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
};

