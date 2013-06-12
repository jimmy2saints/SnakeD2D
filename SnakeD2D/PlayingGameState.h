#pragma once
#include "gamestate.h"
#include <string>
#include <fstream>

class PlayingGameState :
	public GameState
{
public:
	PlayingGameState(HWND windowHandle, ID2D1Factory* factory, Game* gamePtr);
	~PlayingGameState(void);

private:
	int _currentLevel;
	bool _levelData[50][50];
	HRESULT OnRender(ID2D1HwndRenderTarget* target);
	void OnUpdate();
	void OnDiscardDeviceResources();
	HRESULT OnCreateDeviceResources(ID2D1HwndRenderTarget* renderTarget);
	void OnInput(int keysChanged, int keysDown, int keysUp);
	void ChangeLevel(int level);
	void LoadLevel(std::string filename);
};

