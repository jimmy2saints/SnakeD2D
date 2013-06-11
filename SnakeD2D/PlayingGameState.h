#pragma once
#include "gamestate.h"
class PlayingGameState :
	public GameState
{
public:
	PlayingGameState(HWND windowHandle, ID2D1Factory* factory, Game* gamePtr);
	~PlayingGameState(void);

private:
	HRESULT OnRender(ID2D1HwndRenderTarget* target);
	void OnUpdate();
	void OnDiscardDeviceResources();
	HRESULT OnCreateDeviceResources(ID2D1HwndRenderTarget* renderTarget);
};

