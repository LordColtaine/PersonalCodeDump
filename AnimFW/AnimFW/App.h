#pragma once
#include "Window.h"

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
};