#pragma once
#include <Windows.h>
#include <d2d1.h>

#include "safe_release.h"

class GameState
{
public:
	HRESULT Update();
	void OnResize(UINT width, UINT height);
	GameState(HWND windowHandle, ID2D1Factory* factory);
	~GameState(void);

private:
	ID2D1Factory* direct2dFactory;
	ID2D1HwndRenderTarget* renderTarget;
	HWND hWnd;

	virtual HRESULT OnRender(ID2D1HwndRenderTarget* target) = 0;
	virtual void OnUpdate() = 0;
	virtual void OnDiscardDeviceResources() = 0;
	virtual HRESULT OnCreateDeviceResources(ID2D1HwndRenderTarget* target) = 0;

	HRESULT Render();
	HRESULT CreateDeviceResources();
	void DiscardDeviceResources();

};

