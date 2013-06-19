#include "PlayingGameState.h"
#include "game_defs.h"
#include "debugging.h"
#include "safe_release.h"
#include "utility.h"
#include <random>
#include <stdio.h>
#include <wchar.h>
#include "game_state.h"

PlayingGameState::PlayingGameState(HWND windowHandle, ID2D1Factory* factory, Game* gamePtr) :
	GameState(windowHandle, factory, gamePtr)
{
	_wallBrush = nullptr;
	_snakeBrush = nullptr;
	_itemBrush = nullptr;
	_writeFactory = nullptr;
	_scoreFormat = nullptr;
	_textFormatLarge = nullptr;
	_textFormatMedium = nullptr;
	_blackBrush = nullptr;
	_whiteBrush = nullptr;

	for( int x = 0; x < GAME_GRID_SIZE; x++)
		for(int y = 0; y < GAME_GRID_SIZE; y++ )
			_levelData[x][y] = 0;

	_currentLevel = 0;
	_gameSpeed = 200;
	
	_totalScore = 0;
	_lives = 0;
	_scoreToLevel = 0;

	_won = false;

	_dead = false;
	_nextLevelSplash = false;

	NextLevel();
	StartLevel();

	HRESULT hr;
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,__uuidof(_writeFactory), reinterpret_cast<IUnknown **>(&_writeFactory));
	
	if(SUCCEEDED(hr))
	{
		hr = _writeFactory->CreateTextFormat(L"Impact", nullptr, DWRITE_FONT_WEIGHT_NORMAL, 
			DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 16.0f, L"", &_scoreFormat);
	}
	if(SUCCEEDED(hr))
	{
		_scoreFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		_scoreFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	}
	if(SUCCEEDED(hr))
	{
		hr = _writeFactory->CreateTextFormat(L"Impact", nullptr, DWRITE_FONT_WEIGHT_NORMAL, 
			DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 60.0f, L"", &_textFormatLarge);
	}
	if(SUCCEEDED(hr))
	{
		_textFormatLarge->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		_textFormatLarge->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}
	if(SUCCEEDED(hr))
	{
		hr = _writeFactory->CreateTextFormat(L"Impact", nullptr, DWRITE_FONT_WEIGHT_NORMAL, 
			DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 25.0f, L"", &_textFormatMedium);
	}
	if(SUCCEEDED(hr))
	{
		_textFormatMedium->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		_textFormatMedium->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	}

}


PlayingGameState::~PlayingGameState(void)
{
	SafeRelease(&_writeFactory);
	SafeRelease(&_scoreFormat);
	SafeRelease(&_textFormatLarge);
	SafeRelease(&_textFormatMedium);
	OnDiscardDeviceResources();
}

HRESULT PlayingGameState::OnRender(ID2D1HwndRenderTarget* renderTarget)
{
	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	D2D1_SIZE_F rtSize = renderTarget->GetSize();

	int x_offset = (static_cast<int>(rtSize.width) - GAME_AREA_SIZE) / 2;
	int y_offset = (static_cast<int>(rtSize.height) - GAME_AREA_SIZE) / 2;

	if(_won)
	{
		wchar_t wcharWon[100];
		swprintf(wcharWon, 100, L"You beat me...");
		renderTarget->DrawTextA(wcharWon, wcslen(wcharWon), _textFormatLarge,
			D2D1::RectF(x_offset, y_offset, x_offset + GAME_AREA_SIZE, y_offset + GAME_AREA_SIZE),
			_snakeBrush);

		wchar_t subheading[100];
		swprintf(subheading, 100, L"I didnt make more levels :(\n<space/enter> for main menu");
		renderTarget->DrawTextA(subheading, wcslen(subheading), _textFormatMedium,
			D2D1::RectF(x_offset, y_offset + (GAME_AREA_SIZE/1.5f), x_offset + GAME_AREA_SIZE, y_offset + GAME_AREA_SIZE),
			_snakeBrush);
		return S_OK;
	}

	if(_nextLevelSplash)
	{
		wchar_t wcharNextLevel[100];
		swprintf(wcharNextLevel, 100, L"Winz!");
		renderTarget->DrawTextA(wcharNextLevel, wcslen(wcharNextLevel), _textFormatLarge,
			D2D1::RectF(x_offset, y_offset, x_offset + GAME_AREA_SIZE, y_offset + GAME_AREA_SIZE),
			_snakeBrush);

		wchar_t subheading[100];
		swprintf(subheading, 100, L"<space/enter> for next level");
		renderTarget->DrawTextA(subheading, wcslen(subheading), _textFormatMedium,
			D2D1::RectF(x_offset, y_offset + (GAME_AREA_SIZE/1.5f), x_offset + GAME_AREA_SIZE, y_offset + GAME_AREA_SIZE),
			_snakeBrush);
		return S_OK;
	}

	for( int x = 0; x < GAME_GRID_SIZE; x++)
	{
		for( int y = 0; y < GAME_GRID_SIZE; y++)
		{
			D2D1_RECT_F rectangle = utility::CreateUnitRectangle(x_offset, y_offset, x, y);

			if(_levelData[x][y]  & (int)TileTypes::WALL )
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

	D2D1_RECT_F item_rectangle = utility::CreateUnitRectangle(x_offset, y_offset, _itemPosition.x, _itemPosition.y);
	renderTarget->FillRectangle(&item_rectangle, _itemBrush);

	wchar_t wcharScore[100], wcharLives[100], wcharScoreThisLevel[100],wcharCurrentLevel[100];
	swprintf(wcharScore, 100, L"Total Score: %d", _totalScore);
	swprintf(wcharScoreThisLevel, 100, L"Score this Level: %d / %d", _scoreThisLevel, _scoreToLevel);
	swprintf(wcharLives, 100, L"Lives: %d", _lives);
	swprintf(wcharCurrentLevel, 100, L"Level %d", _currentLevel);

	renderTarget->DrawTextA(wcharScore, wcslen(wcharScore), _scoreFormat,
		D2D1::RectF(x_offset, y_offset - 60, x_offset + 250, y_offset - 40), _snakeBrush);
	renderTarget->DrawTextA(wcharScoreThisLevel, wcslen(wcharScoreThisLevel), _scoreFormat,
		D2D1::RectF(x_offset, y_offset - 40, x_offset + 250, y_offset - 20), _snakeBrush);
	renderTarget->DrawTextA(wcharLives, wcslen(wcharLives), _scoreFormat,
		D2D1::RectF(x_offset, y_offset - 20, x_offset + 250, y_offset), _snakeBrush);
	renderTarget->DrawTextA(wcharCurrentLevel, wcslen(wcharCurrentLevel), _scoreFormat,
		D2D1::RectF(x_offset + 400, y_offset - 20, x_offset + 500, y_offset), _snakeBrush);
	
	if(_dead)
	{
		D2D1_RECT_F outline = D2D1::RectF(x_offset + 30, y_offset + 30, x_offset + GAME_AREA_SIZE - 30, y_offset + GAME_AREA_SIZE - 30);
		renderTarget->FillRectangle(&outline, _blackBrush);
		renderTarget->DrawRectangle(&outline, _whiteBrush);

		wchar_t wcharDead[100];
		swprintf(wcharDead, 100, L"Fail :(");
		renderTarget->DrawTextA(wcharDead, wcslen(wcharDead), _textFormatLarge,
			D2D1::RectF(x_offset, y_offset, x_offset + GAME_AREA_SIZE, y_offset + GAME_AREA_SIZE),
			_snakeBrush);

		wchar_t subheading[100];
		if(_lives < 1)
			swprintf(subheading, 100, L"Game Over!\n<space> for main menu.");
		else
			swprintf(subheading, 100, L"<space/enter> to try again!");

		renderTarget->DrawTextA(subheading, wcslen(subheading), _textFormatMedium,
			D2D1::RectF(x_offset, y_offset + (GAME_AREA_SIZE/1.5f), x_offset + GAME_AREA_SIZE, y_offset + GAME_AREA_SIZE),
			_snakeBrush);
	}

	return S_OK;
}

void PlayingGameState::OnUpdate()
{
	if(_dead)
		return;

	if(_nextLevelSplash)
		return;

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

	if(_scoreThisLevel >= _scoreToLevel)
		_nextLevelSplash = true;

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

	for( int i = 1; i < _snake.size(); i++)
	{
		if(_snake[i].x == _snake[0].x && _snake[i].y == _snake[0].y)
			return CollisionType::SELF;
	}

	return CollisionType::NONE;
}

void PlayingGameState::HandleCollision(CollisionType collisionType)
{
	bool placed_item = false;

	switch(collisionType)
	{
	case CollisionType::ITEM:
		_growNextMove++;
		_totalScore += 10;
		_scoreThisLevel += 10;
		while(!placed_item)
			placed_item = PlaceNewItem();
		break;
	case CollisionType::WALL:
		_dead = true;
		_lives--;
		break;
	case CollisionType::SELF:
		_dead = true;
		_lives--;
		break;
	}
}

bool PlayingGameState::PlaceNewItem()
{
	std::default_random_engine generator;
	generator.seed(GetTickCount());
	std::uniform_int_distribution<int> distribution(0, GAME_GRID_SIZE - 1);
	int x = distribution(generator);
	int y = distribution(generator);
	_itemPosition.x = x;
	_itemPosition.y = y;
	return IsValidItemPosition(x,y);
}

bool PlayingGameState::IsValidItemPosition(int x, int y)
{
	bool valid = true;

	if(_levelData[x][y] & static_cast<int>(TileTypes::WALL))
		valid = false;

	if(x == 0 || x == GAME_GRID_SIZE -1)
		valid = false;

	if(y == 0 || y == GAME_GRID_SIZE - 1)
		valid = false;

	for(auto part : _snake)
		if(part.x == x && part.y == y)
			valid = false;

	return valid;
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
	SafeRelease(&_blackBrush);
	SafeRelease(&_whiteBrush);
}

HRESULT PlayingGameState::OnCreateDeviceResources(ID2D1HwndRenderTarget* target)
{
	HRESULT hr;
	hr = target->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::White),
		&_wallBrush);

	if(SUCCEEDED(hr))
	{
	hr = target->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Yellow
		),
		&_snakeBrush);
	}

	if(SUCCEEDED(hr))
	{
	hr = target->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Red),
		&_itemBrush);
	}

	if(SUCCEEDED(hr))
	{
	hr = target->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::White),
		&_whiteBrush);
	}

	if(SUCCEEDED(hr))
	{
	hr = target->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black),
		&_blackBrush);
	}

	return hr;
}

void PlayingGameState::OnInput(int keysChanged, int keysDown, int keysUp)
{
	if(_dead)
	{
		if(keysDown & (static_cast<int>(KeyboardKeys::ENTER) | static_cast<int>(KeyboardKeys::SPACE)))
		{
			if(_lives > 0)
				RestartLevel();
			else
			{
				ChangeGameState(GAME_STATE::START_MENU);
				Sleep(500);
			}
		}
		return;
	}

	if(_nextLevelSplash)
	{
		if(keysDown & (static_cast<int>(KeyboardKeys::ENTER) | static_cast<int>(KeyboardKeys::SPACE)))
		{
			NextLevel();
			StartLevel();
			_nextLevelSplash = false;
		}
		return;
	}

	if(_won)
	{
		if(keysDown & (static_cast<int>(KeyboardKeys::ENTER) | static_cast<int>(KeyboardKeys::SPACE)))
		{
			ChangeGameState(GAME_STATE::START_MENU);
			Sleep(500);
		}
	}

	if(keysDown & static_cast<int>(KeyboardKeys::DOWN_ARROW))
		if(_snakeDirection != Direction::UP)
			_snakeDirection = Direction::DOWN;
	if(keysDown & static_cast<int>(KeyboardKeys::UP_ARROW))
		if(_snakeDirection != Direction::DOWN)
			_snakeDirection = Direction::UP;
	if(keysDown & static_cast<int>(KeyboardKeys::LEFT_ARROW))
		if(_snakeDirection != Direction::RIGHT)
			_snakeDirection = Direction::LEFT;
	if(keysDown & static_cast<int>(KeyboardKeys::RIGHT_ARROW))
		if(_snakeDirection != Direction::LEFT)
			_snakeDirection = Direction::RIGHT;
}

void PlayingGameState::RestartLevel()
{
	_totalScore -= _scoreThisLevel;
	_dead = false;
	StartLevel();
}

void PlayingGameState::StartLevel()
{
	_snakeDirection = Direction::RIGHT;
	_snake.clear();
	Position startSnake;
	startSnake.x = 2;
	startSnake.y = 2;

	bool placed_item = false;
	while(!placed_item)
		placed_item = PlaceNewItem();

	_snake.push_back(startSnake);

	_ticksSinceLastUpdate = 0;
	_ticksSinceLastSnakeMove = 0;
	_ticksLastUpdate = 0;

	_growNextMove = 0;	
	_scoreThisLevel = 0;
	_scoreToLevel = (_currentLevel + (_currentLevel * 8 )) * 10;
}

void PlayingGameState::ChangeLevel(int level)
{
	std::string lvl = "level" + std::to_string(level);
	std::string path = "levels/" + lvl;
	
	for( int x = 0; x < GAME_GRID_SIZE; x++)
		for(int y = 0; y < GAME_GRID_SIZE; y++ )
			_levelData[x][y] = 0;

	LoadLevel(path);
}

void PlayingGameState::NextLevel()
{
	_currentLevel++;

	if(_currentLevel == 11)
	{
		_won = true;
		return;
	}

	_lives++;
	if(_gameSpeed != 10)
		_gameSpeed = _gameSpeed - 10;
	
	ChangeLevel(_currentLevel);
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
			y++;
			x = 0;
		}
		else
		{
			if(cell == '1')
				_levelData[x][y] |= (int)TileTypes::WALL;
			else if(cell == '0')
				_levelData[x][y] |= (int)TileTypes::NOTHING;
			x++;
		}
	}
	fin.close();
}
