#include "StartMenuState.h"


HRESULT StartMenuState::OnRender(ID2D1HwndRenderTarget* renderTarget)
{
	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
	return (HRESULT)S_OK;
}

void StartMenuState::OnUpdate()
{
}

HRESULT StartMenuState::OnCreateDeviceResources(ID2D1HwndRenderTarget* renderTarget)
{
	return (HRESULT)S_OK;
}

void StartMenuState::OnDiscardDeviceResources()
{
}


StartMenuState::StartMenuState(HWND windowHandle, ID2D1Factory* factory) :
	GameState(windowHandle, factory)
{
}


StartMenuState::~StartMenuState(void)
{
}
