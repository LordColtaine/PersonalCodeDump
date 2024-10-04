#include "position.h"

bool Position::MoveInXDirection(int Steps)
{
	m_x += Steps;
	return true;
}

bool Position::MoveInYDirection(int Steps)
{
	m_y += Steps;
	return true;
}
