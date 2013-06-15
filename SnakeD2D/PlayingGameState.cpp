#include "PlayingGameState.h"
#include "game_defs.h"
#include "debugging.h"
#include "safe_release.h"
#include "utility.h"
#include <random>

PlayingGameState::PlayingGameState(HWND windowHandle, ID2D1Factory* factory, Game* gamePtr) :
	GameState(windowHandle, factory, gamePtr)
{
	_wallBrush = nullptr;
	_snakeBrush = nullptr;
	_itemBrush = nullptr;

	for( int x = 0; x < GAME_GRID_SIZE; x++)
		for(int y = 0; y < GAME_GRID_SIZE; y++ )
			_levelData[x][y] = 0;

	_currentLevel = 1;
	_snakeDirection = Direction::RIGHT;

	Position startSnake;
	startSnake.x = 2;
	startSnake.y = 2;

	_itemPosition.x = 5;
	_itemPosition.y = 5;

	_snake.push_back(startSnake);

	_ticksSinceLastUpdate = 0;
	_ticksSinceLastSnakeMove = 0;
	_ticksLastUpdate = 0;
	_gameSpeed = 200;
	_growNextMove = 0;

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

	
	D2D1_RECT_F item_rectangle = utility::CreateUnitRectangle(x_offset, y_offset, _itemPosition.x, _itemPosition.y);
	renderTarget->FillRectangle(&item_rectangle, _itemBrush);

	for( int x = 0; x < GAME_GRID_SIZE; x++)
	{
		for( int y = 0; y < GAME_GRID_SIZE; y++)
		{
			D2D1_RECT_F rectangle = utility::CreateUnitRectangle(x_offset, y_offset, x, y);

			if(_levelData[y][x]  & (int)TileTypes::WALL )
			{
				renderTarget->FillRectangle(&rectangle, _wallBrush);
			}
		}
	}

	for( auto snakePart : _snake)
	{
		D2D1_RECT_F rectangle = utility::CreateUnitRectangle(x_offset, y_offset, snakePart.x, snakePart.y);
		renderTarget->FillRectangle(&rectangle, _snakeBrush);
	}


	return S_OK;
}

void PlayingGameState::OnUpdate()
{

	int ticks = GetTickCount();
	_ticksSinceLastUpdate = ticks - _ticksLastUpdate;
	_ticksSinceLastSnakeMove += _ticksSinceLastUpdate;
	_ticksLastUpdate = ticks;

	if(_ticksSinceLastSnakeMove > _gameSpeed)
	{
		MoveSnake();
		_ticksSinceLastSnakeMove = 0;
	}

	CollisionType collision = CheckCollision();
	HandleCollision(collision);
}

CollisionType PlayingGameState::CheckCollision()
{
	if(_snake[0].x == _itemPosition.x && _snake[0].y == _itemPosition.y)
	{
		return CollisionType::ITEM;
	}

	if(_levelData[_snake[0].x][_snake[0].y] & static_cast<int>(TileTypes::WALL))
	{
		return CollisionType::WALL;
	}

	return CollisionType::NONE;
}

void PlayingGameState::HandleCollision(CollisionType collisionType)
{
	switch(collisionType)
	{
	case CollisionType::ITEM:
		_growNextMove++;
		PlaceNewItem();
		break;
	}
}

void PlayingGameState::PlaceNewItem()
{
	std::default_random_engine generator;
	generator.seed(GetTickCount());
	std::uniform_int_distribution<int> distribution(0, GAME_GRID_SIZE - 1);
	int x = distribution(generator);
	int y = distribution(generator);
	_itemPosition.x = x;
	_itemPosition.y = y;
}

void PlayingGameState::MoveSnake()
{
	int x = 0, y = 0;

	CalculateSnakeMovement(x, y);

	if(_growNextMove == 0 && _snake.size() == 1)
	{
		_snake[0].x += x;
		_snake[0].y += y;
	}
	else
	{
		Position new_snake_part;
		new_snake_part.x = _snake[0].x + x;
		new_snake_part.y = _snake[0].y + y;
		_snake.insert(_snake.begin(), new_snake_part);
		
		if(_growNextMove > 0)
			_growNextMove--;
		else
			_snake.pop_back();
	}
}

void PlayingGameState::CalculateSnakeMovement(int& x, int& y)
{
	switch(_snakeDirection)
	{
	case Direction::DOWN:
		y--;
		break;
	case Direction::LEFT:
		x--;
		break;
	case Direction::RIGHT:
		x++;
		break;
	case Direction::UP:
		y++;
		break;
	}
}


void PlayingGameState::OnDiscardDeviceResources()
{
	SafeRelease(&_snakeBrush);
	SafeRelease(&_wallBrush);
	SafeRelease(&_itemBrush);
}

HRESULT PlayingGameState::OnCreateDeviceResources(ID2D1HwndRenderTarget* target)
{
	HRESULT hr;
	hr = target->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::DarkGray),
		&_wallBrush);

	if(SUCCEEDED(hr))
	{
	hr = target->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::DarkSlateBlue),
		&_snakeBrush);
	}

	if(SUCCEEDED(hr))
	{
	hr = target->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Gold),
		&_itemBrush);
	}

	return hr;
}

void PlayingGameState::OnInput(int keysChanged, int keysDown, int keysUp)
{
	if(keysDown & static_cast<int>(KeyboardKeys::DOWN_ARROW))
		_snakeDirection = Direction::DOWN;
	if(keysDown & static_cast<int>(KeyboardKeys::UP_ARROW))
		_snakeDirection = Direction::UP;
	if(keysDown & static_cast<int>(KeyboardKeys::LEFT_ARROW))
		_snakeDirection = Direction::LEFT;
	if(keysDown & static_cast<int>(KeyboardKeys::RIGHT_ARROW))
		_snakeDirection = Direction::RIGHT;
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
			if(cell == '1')
				_levelData[x][y] |= (int)TileTypes::WALL;
			else if(cell == '0')
				_levelData[x][y] |= (int)TileTypes::NOTHING;
			y++;
		}
	}
	fin.close();
}
