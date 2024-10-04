#pragma once
#include "rover.h"
#include"instructions.h"

struct Controller
{
	vector<Rover> m_Rovers;
	vector<InstructionSet> m_Commands;

	void InstructRover(Rover rover, InstructionSet IS)
	{
		m_Rovers.push_back(rover);
		m_Commands.push_back(IS);
	}
	bool ExecuteInstruction(Plateau *p);
	void ReleaseRover(int index);

	void Display();
};