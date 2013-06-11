#include "SnakeD2D.h"

HRESULT SnakeD2D::Initialize()
{
	HRESULT hr;
	LPCWSTR windowsClassName = L"SnakeD2DWindowClass";

	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
	wcex.style         = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc   = SnakeD2D::WndProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = sizeof(LONG_PTR);
	wcex.hInstance     = HINST_THISCOMPONENT;
	wcex.hbrBackground = nullptr;
	wcex.lpszMenuName  = nullptr;
	wcex.hCursor       = LoadCursor(nullptr, IDI_APPLICATION);
	wcex.lpszClassName = windowsClassName;

	RegisterClassEx(&wcex);

	hWnd = CreateWindow(
		windowsClassName,
		L"SnakeD2D",
		WS_EX_TOPMOST | WS_POPUP,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0, 0,
		nullptr,
		nullptr,
		HINST_THISCOMPONENT,
		this
		);

	hr = hWnd ? S_OK : E_FAIL;
    
	if (SUCCEEDED(hr))
	{
		game = new Game(hWnd);
		hr = game->Initialize();
		ShowWindow(hWnd, SW_SHOWNORMAL);
		UpdateWindow(hWnd);
	}

	return hr;
}

LRESULT CALLBACK SnakeD2D::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
	bool wasHandled = false;

    if (message == WM_CREATE)
    {
        LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		SnakeD2D *app = (SnakeD2D*)pcs->lpCreateParams;

        ::SetWindowLongPtrW(
            hwnd,
            GWLP_USERDATA,
            PtrToUlong(app)
            );
    }
    else
    {
        SnakeD2D *app = reinterpret_cast<SnakeD2D *>(static_cast<LONG_PTR>(
            ::GetWindowLongPtrW(
                hwnd,
                GWLP_USERDATA
                )));
		switch(message) 
		{
		case WM_KEYDOWN:
			switch(wParam)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			default:
				break;
			}
			wasHandled = true;
			break;
		default:
			break;
		case WM_PAINT:
			ValidateRect(hwnd, nullptr); // avoid new wm_paint messages
			wasHandled = true;
			result = 0;
			break;
		case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				app->OnResize(width, height);
			}
			result = 0;
			wasHandled = true;
			break;
		}
	}

	if(!wasHandled)
		result = DefWindowProc(hwnd, message, wParam, lParam);

	return result;
}


void SnakeD2D::OnResize(UINT width, UINT height)
{
	if(game)
		game->OnResize(width, height);
}


void SnakeD2D::Run()
{
	MSG msg;

	bool running = true;
	
	while(running) 
	{
		if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				running = false;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if(game)
				game->Update();
		}
	}
}

SnakeD2D::SnakeD2D(void) :
	hWnd(nullptr),
	game(nullptr)
{
}


SnakeD2D::~SnakeD2D(void)
{
}
