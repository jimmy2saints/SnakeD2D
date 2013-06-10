#include "GameState.h"



GameState::GameState(HWND windowHandle, ID2D1Factory* factory)
{
	direct2dFactory = factory;
	hWnd = windowHandle;
	renderTarget = nullptr;
}


GameState::~GameState(void)
{
	DiscardDeviceResources();
}

HRESULT GameState::Update()
{
	// TODO: Lock to execute OnUpdate() to specific time based update interval?
	OnUpdate();
	HRESULT hr = Render();
	return hr;
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
            D2D1::HwndRenderTargetProperties(hWnd, size),
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
