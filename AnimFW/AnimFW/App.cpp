#include "App.h"

App::App()
	:
	window(1280, 720, "Direct3D")
{
	;
}

int App::Go()
{
	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}
		DoFrame();
	}
}
void App::DoFrame()
{
	;
}