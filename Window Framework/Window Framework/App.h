#pragma once
#include "Window.h"
#include "Timer.h"
class App
{
public:
	App();
	//message loop / master frame
	int Go();

private:
	void DoFrame();

private:
	Window window;
	Timer timer;
};