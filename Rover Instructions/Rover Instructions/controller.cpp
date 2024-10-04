#include "controller.h"


void Controller::ReleaseRover(int index)
{
	m_Rovers.erase(m_Rovers.begin() + index);
	m_Commands.erase(m_Commands.begin() + index);
}
bool Controller::ExecuteInstruction(Plateau *p)
{
	int count = 1;
	int timer = 0;
	vector<bool> flags;
	for (int i = 0; i < m_Rovers.size(); i++)flags.push_back(false);
	while (count > 0)
	{
		timer++;
		if (timer > 30)break;
		count = 0;
		for (int i = 0; i < m_Rovers.size(); i++)
		{
			if (!flags[i])
			{
				char Ins = m_Commands[i].GetCurrentInstruction();
				if (Ins)
				{
					p->Absent(m_Rovers[i].GetPosition());
					int res = m_Rovers[i].ParseInstruction(Ins, p);
					if (0 == res)
					{
						m_Commands[i].GoBack();
						count++;
					}
					else
					{
						flags[i] = true;
						timer = 0;
					}
					p->Present(m_Rovers[i].GetPosition());
				}
				else
				{
					ReleaseRover(i);
					i--;
				}
			}
		}
	}
	return true;
}

void Controller::Display()
{
	for (auto rover : m_Rovers)
		rover.OutputRoverDetails(cout);
}