#include "PlayingGameState.h"
#include "game_defs.h"

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
