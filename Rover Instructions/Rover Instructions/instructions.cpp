#include "instructions.h"

bool InstructionSet::ValidInstructionSet()
{
	for (auto instr : m_Sequence)
	{
		if (instr != 'L' && instr != 'R' && instr != 'M')
			return false;
	}
	return true;
}