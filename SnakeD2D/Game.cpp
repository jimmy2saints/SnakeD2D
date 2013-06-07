#include "Game.h"

HRESULT Game::Initialize()
{
	HRESULT hr = S_OK;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &direct2dFactory);

	float currentResolutionWidth = (float)GetSystemMetrics(SM_CXSCREEN);
	float currentResolutionHeight = (float)GetSystemMetrics(SM_CYSCREEN);

	FLOAT dpiX, dpiY;
	direct2dFactory->GetDesktopDpi(&dpiX, &dpiY);

	UINT width = static_cast<UINT>(ceil(currentResolutionWidth * dpiX / 96.f));
	UINT height = static_cast<UINT>(ceil(currentResolutionHeight * dpiY / 96.f));

	DBOUT("Current Width is " << currentResolutionWidth << ", Current height is " << currentResolutionHeight << "\n" );
	DBOUT("dpiX is " << dpiX << ", dpiY is " << dpiY << "\n" );
	DBOUT("width is " << width << ", height is " << height << "\n" );

	SetWindowPos(hWnd, HWND_TOP, CW_USEDEFAULT, CW_USEDEFAULT, width, height, SWP_NOMOVE );
	
	return hr; 
}

HRESULT Game::CreateDeviceResources()
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
    }

    return hr;
}

void Game::OnResize(UINT width, UINT height)
{
    if (renderTarget)
		renderTarget->Resize(D2D1::SizeU(width, height));
}

HRESULT Game::Update()
{
	DBOUT("Update occured");
	HRESULT hr = S_OK;
	hr = Render();
	return hr;
}

HRESULT Game::Render()
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();

	if(SUCCEEDED(hr))
	{
		renderTarget->BeginDraw();
		renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
		hr = renderTarget->EndDraw();
	}

	if (hr == D2DERR_RECREATE_TARGET)
    {
        hr = S_OK;
        DiscardDeviceResources();
    }

	return hr;
}


void Game::DiscardDeviceResources()
{
	SafeRelease(&direct2dFactory);
	SafeRelease(&renderTarget);
}

Game::Game(HWND windowsHandle) : 
	hWnd(windowsHandle),
	direct2dFactory(nullptr), 
	renderTarget(nullptr)
{
}


Game::~Game(void)
{
	SafeRelease(&direct2dFactory);
	SafeRelease(&renderTarget);
}
