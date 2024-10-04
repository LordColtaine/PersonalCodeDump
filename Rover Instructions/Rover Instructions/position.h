#pragma once
#include <iostream>
using namespace std;
struct Position
{
	int m_x;
	int m_y;

	Position()
	{
		m_x = m_y = 0;
	}

	Position(int x, int y)
	{
		m_x = x;
		m_y = y;
	}

	int GetX()
	{
		return m_x;
	}

	int GetY()
	{
		return m_y;
	}

	bool MoveInXDirection(int Steps);
	bool MoveInYDirection(int Steps);
	Position operator=(const Position& position)
	{
		m_x = position.m_x;
		m_y = position.m_y;
		return *this;
	}
	template<typename T>
	void OutputPosition(T& stream)
	{
		stream << endl << GetX() << " " << GetY();
	}
};
