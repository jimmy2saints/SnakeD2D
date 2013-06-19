#include "GameState.h"
#include "Game.h"
#include "safe_release.h"

GameState::GameState(HWND windowHandle, ID2D1Factory* factory, Game* gamePtr)
{
	direct2dFactory = factory;
	hWnd = windowHandle;
	game = gamePtr;
	renderTarget = nullptr;
	previousKeyboardState = 0;
}


GameState::~GameState(void)
{
	SafeRelease(&renderTarget);
}

HRESULT GameState::Update()
{
	// TODO: Lock to execute OnUpdate() to specific time based update interval?
	HandleInput();
	OnUpdate();
	HRESULT hr = Render();
	return hr;
}

void GameState::HandleInput()
{
	int currentKeyboardState = 0;
	if(GetAsyncKeyState(VK_DOWN)) currentKeyboardState |= (int)KeyboardKeys::UP_ARROW;
	if(GetAsyncKeyState(VK_UP)) currentKeyboardState |= (int)KeyboardKeys::DOWN_ARROW;
	if(GetAsyncKeyState(VK_LEFT)) currentKeyboardState |= (int)KeyboardKeys::LEFT_ARROW;
	if(GetAsyncKeyState(VK_RIGHT)) currentKeyboardState |= (int)KeyboardKeys::RIGHT_ARROW;
	if(GetAsyncKeyState(VK_RETURN)) currentKeyboardState |= (int)KeyboardKeys::ENTER;
	if(GetAsyncKeyState(VK_SPACE)) currentKeyboardState |= (int)KeyboardKeys::SPACE;

	int keysChanged = currentKeyboardState ^ previousKeyboardState;
	int keysDown = currentKeyboardState & keysChanged;
	int keysUp = ~currentKeyboardState & keysChanged;

	OnInput(keysChanged, keysDown, keysUp);

	previousKeyboardState = currentKeyboardState;
}

void GameState::ChangeGameState(GAME_STATE new_state)
{
	game->ChangeGameState(new_state);
}

void GameState::OnResize(UINT width, UINT height)
{
    if (renderTarget)
		renderTarget->Resize(D2D1::SizeU(width, height));
}

HRESULT GameState::Render()
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();

	if(SUCCEEDED(hr))
	{
		renderTarget->BeginDraw();
		OnRender(renderTarget);
		hr = renderTarget->EndDraw();
	}

	if (hr == D2DERR_RECREATE_TARGET)
    {
        hr = S_OK;
        DiscardDeviceResources();
    }

	return hr;
}

HRESULT GameState::CreateDeviceResources()
{
    HRESULT hr = S_OK;

    if (!renderTarget)
    {
        RECT rc;
        GetClientRect(hWnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(
            rc.right - rc.left,
            rc.bottom - rc.top
            );

        hr = direct2dFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hWnd, size, D2D1_PRESENT_OPTIONS_IMMEDIATELY), // D2D1_PRESENT_OPTIONS_NONE
            &renderTarget
            );

		if(SUCCEEDED(hr))
		{
			hr = OnCreateDeviceResources(renderTarget);
		}
    }

    return hr;
}

void GameState::DiscardDeviceResources()
{
	SafeRelease(&renderTarget);
	OnDiscardDeviceResources();
}
