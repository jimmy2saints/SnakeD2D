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
	
	ChangeGameState(GAME_STATE::START_MENU);
	return hr; 
}

void Game::OnResize(UINT width, UINT height)
{
	state->OnResize(width, height);
}

HRESULT Game::Update()
{
	UpdateFps();

	HRESULT hr = S_OK;
	UpdateGameState();

	if(state != nullptr)
		hr = state->Update();
	
	return hr;
}

void Game::UpdateFps()
{
	fpsCounter++;
	int ticks = GetTickCount();
	ticksSinceLastFrame = ticks - ticksLastFrame;
	ticksCounter += ticksSinceLastFrame;
	ticksLastFrame = ticks;
	if(ticksCounter >= 1000)
	{
		fps = fpsCounter;
		fpsCounter = 0;
		ticksCounter = 0;
		DBOUT("Current FPS: " << fps << "\n");
	}
}

void Game::UpdateGameState()
{
	if( oldGameState == gameState)
		return;

	if(state != nullptr)
	{
		delete state;
		state = nullptr;
	}

	switch(gameState)
	{
	case GAME_STATE::START_MENU:
		state = new StartMenuState(hWnd, direct2dFactory, this);
		break;
	case GAME_STATE::PLAYING:
		state = new PlayingGameState(hWnd, direct2dFactory, this);
		break;
	}

	oldGameState = gameState;
}

void Game::ChangeGameState(GAME_STATE new_state)
{
	oldGameState = gameState;
	gameState = new_state;
}

Game::Game(HWND windowsHandle) : 
	hWnd(windowsHandle),
	state(nullptr),
	direct2dFactory(nullptr)
{
	oldGameState = GAME_STATE::BOOTING;
	gameState = GAME_STATE::BOOTING;
	fpsCounter = 0;
	ticksCounter = 0;
	fps = 0;
	ticksLastFrame = 0;
	ticksSinceLastFrame = 0;
}


Game::~Game(void)
{
	SafeRelease(&direct2dFactory);

	if(state)
		delete state;
}
