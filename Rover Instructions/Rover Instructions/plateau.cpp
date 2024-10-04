#include "plateau.h"

void Plateau::PrintGrid()
{
	for (int i = 0; i < m_Grid.size(); i++)
	{
		for (int j = 0; j < m_Grid[i].size(); j++)
		{
			if (m_Grid[i][j])cout << "X";
			else cout << "0";
		}
		cout << endl;
	}
}