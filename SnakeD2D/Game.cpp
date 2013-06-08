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
	
	state = new StartMenuState(hWnd, direct2dFactory);
	return hr; 
}

void Game::OnResize(UINT width, UINT height)
{
	state->OnResize(width, height);
}

HRESULT Game::Update()
{
	HRESULT hr = S_OK;
	hr = state->Update();
	return hr;
}

Game::Game(HWND windowsHandle) : 
	hWnd(windowsHandle),
	state(nullptr),
	direct2dFactory(nullptr)
{
}


Game::~Game(void)
{
	SafeRelease(&direct2dFactory);

	if(state)
		delete state;
}
