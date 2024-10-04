#include "rover.h"

Direction Rover::TurnLeft()
{
	m_Direction = static_cast<Direction>((static_cast<int>(m_Direction) + 3)%4);
	return m_Direction;
}

Direction Rover::TurnRight()
{
	m_Direction = static_cast<Direction>((static_cast<int>(m_Direction) + 1) % 4);
	return m_Direction;
}

string Rover::GetDirectionAsString()
{
	switch (m_Direction)
	{
	case Direction::EAST:
		return "East";
		break;
	case Direction::NORTH:
		return "North";
		break;
	case Direction::WEST:
		return "West";
		break;
	default:
		return "South";
	}
}

int Rover::MoveRover(Plateau *plateau)
{
	Position NewPos = m_Position;
	switch (m_Direction)
	{
	case Direction::NORTH:
		NewPos.MoveInYDirection(1);
		break;
	case Direction::EAST:
		NewPos.MoveInXDirection(1);
		break;
	case Direction::SOUTH:
		NewPos.MoveInYDirection(-1);
		break; 
	case Direction::WEST:
		NewPos.MoveInXDirection(-1);
		break;
	}
	if (!plateau->IsInPlateau(NewPos))
	{
		return -1;
	}
	if (plateau->IsColliding(NewPos))
	{
		return 0;
	}
	m_Position = NewPos;
	return 1;
}