#pragma once

#include "plateau.h"
using namespace std;
enum class Direction
{
	NORTH, EAST, SOUTH, WEST
};
struct Rover
{
	Position m_Position;
	Direction m_Direction;
	Rover()
	{
		m_Position = Position();
		m_Direction = Direction::NORTH;
	}
	Rover(int x, int y)
	{
		m_Position = Position(x, y);
		m_Direction = Direction::NORTH;
	}
	Rover(Direction dir)
	{
		m_Position = Position();
		m_Direction = dir;
	}
	Rover(int x, int y, Direction dir)
	{
		m_Position = Position(x, y);
		m_Direction = dir;
	}
	Rover(int x, int y, char dir)
	{
		switch (dir)
		{
		case 'N':
			m_Direction = Direction::NORTH;
			break;
		case 'E':
			m_Direction = Direction::EAST;
			break;
		case 'W':
			m_Direction = Direction::WEST;
			break;
		case 'S':
			m_Direction = Direction::SOUTH;
			break;
		}
		m_Position = Position(x, y);
	}
	int MoveRover(Plateau* plateau);
	Direction TurnLeft();
	Direction TurnRight();
	
	Position GetPosition()
	{
		return m_Position;
	}

	Direction GetDirection()
	{
		return m_Direction;
	}
	string GetDirectionAsString();
	template <typename T>
	void OutputRoverDetails(T& stream)
	{
		m_Position.OutputPosition(stream);
		stream << " " << GetDirectionAsString();
	}
	int ParseInstruction(char Instruction, Plateau *p)
	{
		switch (Instruction)
		{
		case 'M':return MoveRover(p);
		case 'L':TurnLeft();
			break;
		case 'R':TurnRight();
		}
		return 1;
	}
};