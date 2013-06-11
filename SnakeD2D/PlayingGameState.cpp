#include "PlayingGameState.h"


PlayingGameState::PlayingGameState(HWND windowHandle, ID2D1Factory* factory, Game* gamePtr) :
	GameState(windowHandle, factory, gamePtr)
{
}


PlayingGameState::~PlayingGameState(void)
{
	OnDiscardDeviceResources();
}

HRESULT PlayingGameState::OnRender(ID2D1HwndRenderTarget* renderTarget)
{
	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::AliceBlue));
	return S_OK;
}

void PlayingGameState::OnUpdate()
{
}

void PlayingGameState::OnDiscardDeviceResources()
{
}

HRESULT PlayingGameState::OnCreateDeviceResources(ID2D1HwndRenderTarget* target)
{
	return S_OK;
}
