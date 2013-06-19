#pragma once
#include "gamestate.h"
#include <string>
#include <fstream>
#include "game_defs.h"
#include <vector>
#include <dwrite.h>

enum class TileTypes : int 
{
	WALL =		1 << 0,
	NOTHING	=	1 << 1
};

enum class Direction : int
{
	RIGHT = 1,
	LEFT = 2,
	UP = 3,
	DOWN = 4
};

enum class CollisionType : int
{
	WALL = 1,
	SELF = 2,
	ITEM = 3,
	NONE = 4
};

struct Position
{
	int x;
	int y;
};

class PlayingGameState :
	public GameState
{
public:
	PlayingGameState(HWND windowHandle, ID2D1Factory* factory, Game* gamePtr);
	~PlayingGameState(void);

private:
	int _currentLevel;
	int _levelData[GAME_GRID_SIZE][GAME_GRID_SIZE];

	Direction _snakeDirection;

	std::vector<Position> _snake;
	Position _itemPosition;
	
	IDWriteFactory* _writeFactory;
	IDWriteTextFormat* _scoreFormat;
	IDWriteTextFormat* _textFormatLarge;
	IDWriteTextFormat* _textFormatMedium;

	ID2D1SolidColorBrush* _wallBrush;
	ID2D1SolidColorBrush* _snakeBrush;
	ID2D1SolidColorBrush* _itemBrush;
	ID2D1SolidColorBrush* _blackBrush;
	ID2D1SolidColorBrush* _whiteBrush;
	
	int _ticksSinceLastUpdate;
	int _ticksLastUpdate;
	int _ticksSinceLastSnakeMove;
	int _gameSpeed;
	int _growNextMove;

	int _totalScore;
	int _scoreThisLevel;
	int _lives;
	int _scoreToLevel;

	bool _dead;
	bool _nextLevelSplash;
	bool _won;
	

	HRESULT OnRender(ID2D1HwndRenderTarget* target);
	void OnUpdate();
	void OnDiscardDeviceResources();
	HRESULT OnCreateDeviceResources(ID2D1HwndRenderTarget* renderTarget);
	void OnInput(int keysChanged, int keysDown, int keysUp);
	void ChangeLevel(int level);
	void LoadLevel(std::string filename);
	void MoveSnake();
	void CalculateSnakeMovement(int& x, int& y);
	CollisionType CheckCollision();
	void HandleCollision(CollisionType collisionType);
	bool PlaceNewItem();
	bool IsValidItemPosition(int x, int y);
	void NextLevel();
	void RestartLevel();
	void StartLevel();
};

