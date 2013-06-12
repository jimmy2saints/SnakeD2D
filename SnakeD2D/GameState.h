#pragma once
#include <Windows.h>
#include <d2d1.h>

class Game;
enum class GAME_STATE;

enum class KeyboardKeys : int 
{
	UP_ARROW =		1 << 0,
	DOWN_ARROW =	1 << 1,
	LEFT_ARROW =	1 << 2,
	RIGHT_ARROW =	1 << 3,
	ENTER =			1 << 4,
	SPACE =			1 << 5
};

class GameState
{
public:
	HRESULT Update();
	void OnResize(UINT width, UINT height);
	GameState(HWND windowHandle, ID2D1Factory* factory, Game* gamePtr);
	~GameState(void);

protected:
	void ChangeGameState(GAME_STATE new_state);



private:
	ID2D1Factory* direct2dFactory;
	ID2D1HwndRenderTarget* renderTarget;
	HWND hWnd;
	Game* game;

	int previousKeyboardState;

	virtual HRESULT OnRender(ID2D1HwndRenderTarget* target) = 0;
	virtual void OnUpdate() = 0;
	virtual void OnDiscardDeviceResources() = 0;
	virtual HRESULT OnCreateDeviceResources(ID2D1HwndRenderTarget* target) = 0;
	virtual void OnInput(int keysChanged, int keysDown, int keysUp) = 0;

	HRESULT Render();
	HRESULT CreateDeviceResources();
	void DiscardDeviceResources();
	void HandleInput();

};

