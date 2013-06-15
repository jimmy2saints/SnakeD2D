#pragma once
#include "gamestate.h"
#include <string>
#include <fstream>
#include "game_defs.h"

class PlayingGameState :
	public GameState
{
public:
	PlayingGameState(HWND windowHandle, ID2D1Factory* factory, Game* gamePtr);
	~PlayingGameState(void);

private:
	int _currentLevel;
	bool _levelData[GAME_GRID_SIZE][GAME_GRID_SIZE];
	
	ID2D1SolidColorBrush* lightSlateGrayBrush;
	
	
	HRESULT OnRender(ID2D1HwndRenderTarget* target);
	void OnUpdate();
	void OnDiscardDeviceResources();
	HRESULT OnCreateDeviceResources(ID2D1HwndRenderTarget* renderTarget);
	void OnInput(int keysChanged, int keysDown, int keysUp);
	void ChangeLevel(int level);
	void LoadLevel(std::string filename);

};

