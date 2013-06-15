#include "PlayingGameState.h"
#include "game_defs.h"
#include "debugging.h"
#include "safe_release.h"

PlayingGameState::PlayingGameState(HWND windowHandle, ID2D1Factory* factory, Game* gamePtr) :
	GameState(windowHandle, factory, gamePtr)
{
	for( int x = 0; x < GAME_GRID_SIZE; x++)
		for(int y = 0; y < GAME_GRID_SIZE; y++ )
			_levelData[x][y] = false;

	_currentLevel = 1;
	ChangeLevel(_currentLevel);
}


PlayingGameState::~PlayingGameState(void)
{
	OnDiscardDeviceResources();
}

HRESULT PlayingGameState::OnRender(ID2D1HwndRenderTarget* renderTarget)
{
	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::AliceBlue));
	D2D1_SIZE_F rtSize = renderTarget->GetSize();

	int x_offset = (static_cast<int>(rtSize.width) - GAME_AREA_SIZE) / 2;
	int y_offset = (static_cast<int>(rtSize.height) - GAME_AREA_SIZE) / 2;

	for( int x = 0; x < GAME_GRID_SIZE; x++)
	{
		for( int y = 0; y < GAME_GRID_SIZE; y++)
		{
			if(_levelData[y][x] == true )
			{
				D2D1_RECT_F rectangle = D2D1::RectF(
					x_offset + (GAME_UNIT_SIZE * x),
					y_offset + (GAME_UNIT_SIZE * y),
					x_offset + (GAME_UNIT_SIZE * x) + GAME_UNIT_SIZE,
					y_offset + (GAME_UNIT_SIZE * y) + GAME_UNIT_SIZE
				);
				renderTarget->FillRectangle(&rectangle, lightSlateGrayBrush);
			}
		}
	}

	return S_OK;
}

void PlayingGameState::OnUpdate()
{
}

void PlayingGameState::OnDiscardDeviceResources()
{
	SafeRelease(&lightSlateGrayBrush);
}

HRESULT PlayingGameState::OnCreateDeviceResources(ID2D1HwndRenderTarget* target)
{
	HRESULT hr;
	hr = target->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::DarkGray),
		&lightSlateGrayBrush);

	return hr;
}

void PlayingGameState::OnInput(int keysChanged, int keysDown, int keysUp)
{
}

void PlayingGameState::ChangeLevel(int level)
{
	std::string lvl = "level" + std::to_string(level);
	std::string path = "levels/" + lvl;

	LoadLevel(path);

	// Do all kindsa funky stuff
}

void PlayingGameState::LoadLevel(std::string filename)
{
	int x = 0, y = 0;
	char cell;

	std::fstream fin(filename, std::fstream::in);
	while(!fin.eof())
	{
		fin.get(cell);
		if(cell == '\n')
		{
			x++;
			y = 0;
		}
		else
		{
			if(cell == '0')
				_levelData[x][y] = false;
			else if(cell == '1')
				_levelData[x][y] = true;
			y++;
		}
	}
	fin.close();
}
