#include <Windows.h>
#include "SnakeD2D.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    if (SUCCEEDED(CoInitialize(NULL)))
    {
        {
            SnakeD2D *app = NULL;
			 app = new SnakeD2D();

            if (SUCCEEDED(app->Initialize()))
            {
               app->Run();
            }
			delete app;
        }

        CoUninitialize();
    }

    return 0;
}
