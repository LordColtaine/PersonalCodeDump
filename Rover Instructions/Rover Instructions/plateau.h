#pragma once
#include "position.h"
#include<vector>
using namespace std;
struct Plateau
{
	Position Top_Right;
	vector<vector<bool>> m_Grid;
	Plateau()
	{
		Top_Right = Position();
	}
	Plateau(int top, int right)
	{
		Top_Right = Position(top, right);
		for (int i = 0; i < top; i++)
		{
			vector<bool> temp;
			for (int j = 0; j < right; j++)
				temp.push_back(false);
			m_Grid.push_back(temp);
		}
	}
	int GetTop()
	{
		return Top_Right.GetY();
	}
	int GetRight()
	{
		return Top_Right.GetX();
	}

	bool OutOfBounds(int x, int y)
	{
		return x >= 0 && y >= 0 && x <= Top_Right.GetX() && y <= Top_Right.GetY();
	}

	bool IsInPlateau(Position pos)
	{
		return (pos.GetX() >= 0 && pos.GetY() >= 0 && pos.GetX() <= Top_Right.GetX() && pos.GetY() <= Top_Right.GetY());
	}
	void Present(Position pos)
	{
		m_Grid[m_Grid.size() - 1 - pos.GetY()][pos.GetX()] = true;
	}

	void Absent(Position pos)
	{
		m_Grid[m_Grid.size()-1 - pos.GetY()][pos.GetX()] = false;
	}

	bool IsColliding(Position pos)
	{
		return m_Grid[m_Grid.size() - 1 - pos.GetY()][pos.GetX()];
	}
	void PrintGrid();
};