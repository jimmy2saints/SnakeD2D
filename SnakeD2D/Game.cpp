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

	SetWindowPos(hWnd, HWND_TOPMOST, CW_USEDEFAULT, CW_USEDEFAULT, width, height, WS_EX_TOPMOST | WS_POPUP );
	
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


        //if (SUCCEEDED(hr))
        //{
        //    // Create a gray brush.
        //    hr = renderTarget->CreateSolidColorBrush(
        //        D2D1::ColorF(D2D1::ColorF::LightSlateGray),
        //        &m_pLightSlateGrayBrush
        //        );
        //}
        //if (SUCCEEDED(hr))
        //{
            // Create a blue brush.
        //    hr = m_pRenderTarget->CreateSolidColorBrush(
        //        D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
        //        &m_pCornflowerBlueBrush
        //        );
        //}
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
	HRESULT hr = S_OK;
	return hr;
}

HRESULT Game::Render()
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();

	if(SUCCEEDED(hr))
	{
		renderTarget->BeginDraw();
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
