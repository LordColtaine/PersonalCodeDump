#pragma once
#include<vector>
#include <string>
using namespace std;
struct InstructionSet
{
	vector<char> m_Sequence;
	int m_NextInstruction;

	InstructionSet()
	{
		m_NextInstruction = 0;
	}
	InstructionSet(string s)
	{
		m_NextInstruction = 0;
		for (char ch : s)
			m_Sequence.push_back(ch);
	}

	InstructionSet(char* s)
	{
		m_NextInstruction = 0;
		for (int i = 0; s[i] != '\0'; i++)
			m_Sequence.push_back(s[i]);
	}

	bool ValidInstructionSet();
	
	template<typename T>
	static InstructionSet GetInstructionSet(T& stream)
	{
		InstructionSet IS;
		char ch;
		stream >> ch;
		while (ch)
		{
			IS.m_Sequence.push_back(ch);
		}
		return IS;
	}
	void OnExecute()
	{
		m_NextInstruction++;
	}
	void GoBack()
	{
		m_NextInstruction--;
	}
	bool InBounds()
	{
		return m_NextInstruction < m_Sequence.size();
	}
	int GetIndex()
	{
		return m_NextInstruction;
	}
	char GetCurrentInstruction()
	{
		if (InBounds())
		{
			char ret = m_Sequence[m_NextInstruction];
			OnExecute();
			return ret;
		}
		return 0;
	}
	void Reset()
	{
		m_NextInstruction = 0;
	}
};