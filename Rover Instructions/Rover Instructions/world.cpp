#include "world.h"

bool World::UpdateWorld()
{
	IncrementTimer();
    return m_Controller.ExecuteInstruction(&m_Plateau);
}

void World::GetInput()
{
    int top, right;
    cin >> top >> right;
    m_Plateau = Plateau(top, right);
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        int x, y;
        char dir;
        string cmd;
        cin >> x >> y >> dir >> cmd;
        m_Controller.InstructRover(Rover(x, y, dir), InstructionSet(cmd));
        m_Plateau.Present(Position(x, y));
    }
}