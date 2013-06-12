#include "StartMenuState.h"
#include "safe_release.h"
#include "game_state.h"

HRESULT StartMenuState::OnRender(ID2D1HwndRenderTarget* renderTarget)
{
	static const WCHAR startGametext[] = L"Start";
	static const WCHAR quitGameText[] = L"Quit";

	D2D1_SIZE_F renderTargetSize = renderTarget->GetSize();

	float sliceSize = (renderTargetSize.height / 10);
	float startGameTextStartY = sliceSize * 4;
	float startGameTextStopY = sliceSize * 5;
	float quitGameTextStartY = sliceSize * 5;
	float quitGameTextStopY = sliceSize * 6;

	ID2D1SolidColorBrush* startBrush = nullptr;
	ID2D1SolidColorBrush* quitBrush = nullptr;
	D2D1_POINT_2F ellipsePosition;

	if(currentMenuOption == START_MENU_OPTION::START)
	{
		startBrush = activeTextBrush;
		quitBrush = idleTextBrush;
		ellipsePosition = D2D1::Point2F((renderTargetSize.width / 2) - 100.0f, startGameTextStartY + (sliceSize / 2) );
	} 
	else
	{
		startBrush = idleTextBrush;
		quitBrush = activeTextBrush;
		ellipsePosition = D2D1::Point2F((renderTargetSize.width / 2) - 100.0f, quitGameTextStartY + (sliceSize / 2));
	}

	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Wheat));

	renderTarget->DrawTextA(startGametext, ARRAYSIZE(startGametext) - 1, textFormat,
		D2D1::RectF(0, startGameTextStartY, renderTargetSize.width, startGameTextStopY), startBrush);
	
	renderTarget->DrawTextA(quitGameText, ARRAYSIZE(quitGameText) - 1, textFormat,
		D2D1::RectF(0, quitGameTextStartY, renderTargetSize.width, quitGameTextStopY), quitBrush);

	
	D2D1_ELLIPSE marker = D2D1::Ellipse(ellipsePosition, 10.0f, 10.0f );

	renderTarget->DrawEllipse(marker, activeTextBrush, 2.0f);
	
	return (HRESULT)S_OK;
}

void StartMenuState::OnUpdate()
{
}

void StartMenuState::OnInput(int keysChanged, int keysDown, int keysUp)
{
	if( keysDown & ((int)KeyboardKeys::UP_ARROW | (int)KeyboardKeys::DOWN_ARROW))
	{
		if(currentMenuOption == START)
			currentMenuOption = QUIT;
		else
			currentMenuOption = START;
	}

	if( keysDown & ( (int)KeyboardKeys::ENTER |(int)KeyboardKeys:: SPACE ) )
	{
		if(currentMenuOption == START)
		{
			ChangeGameState(GAME_STATE::PLAYING);
		}
		else
		{
			PostQuitMessage(0);
		}
	}
}

HRESULT StartMenuState::OnCreateDeviceResources(ID2D1HwndRenderTarget* renderTarget)
{
	HRESULT hr;

	hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black, 1.0f), &idleTextBrush);

	if(SUCCEEDED(hr))
		hr = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White, 1.0f), &activeTextBrush);

	return hr;
}

void StartMenuState::OnDiscardDeviceResources()
{
	SafeRelease(&idleTextBrush);
	SafeRelease(&activeTextBrush);
}


StartMenuState::StartMenuState(HWND windowHandle, ID2D1Factory* factory, Game* gamePtr) :
	GameState(windowHandle, factory, gamePtr), 
	writeFactory(nullptr),
	textFormat(nullptr),
	idleTextBrush(nullptr),
	activeTextBrush(nullptr),
	currentMenuOption(START_MENU_OPTION::START)
{
	HRESULT hr;
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,__uuidof(writeFactory), reinterpret_cast<IUnknown **>(&writeFactory));
	
	if(SUCCEEDED(hr))
	{
		hr = writeFactory->CreateTextFormat(L"Impact", nullptr, DWRITE_FONT_WEIGHT_NORMAL, 
			DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, TEXT_SIZE, L"", &textFormat);
	}
	if(SUCCEEDED(hr))
	{
		textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}
}


StartMenuState::~StartMenuState(void)
{
	SafeRelease(&writeFactory);
	SafeRelease(&textFormat);
	OnDiscardDeviceResources();
}
