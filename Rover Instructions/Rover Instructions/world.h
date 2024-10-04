#pragma once

#include "controller.h"

struct World
{
	int m_Timer;
	Controller m_Controller;
	Plateau m_Plateau;
	World()
	{
		m_Timer = 0;
	}
	void IncrementTimer()
	{
		m_Timer++;
	}
	bool UpdateWorld();
	void GetInput();
	void Display()
	{
		//m_Controller.Display();
		m_Plateau.PrintGrid();
	}

	int GetTimer()
	{
		return m_Timer;
	}
};