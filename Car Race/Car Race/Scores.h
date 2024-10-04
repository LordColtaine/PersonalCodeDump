#pragma once
using namespace std;
#include<iostream>
#include <string.h>
struct Record
{
	char name[256];
	int score;
	Record()
	{
		score = INT_MIN;
	}
	Record operator=(Record& r)
	{
		if (this != &r)
		{
			strcpy(name, r.name);
			score = r.score;
		}
		return *this;
	}
};
void writeToFile(char *user, int score)
{
	FILE* f = fopen("scores.txt", "a");
	if (f)
	{
		fprintf(f, "%s : %d\n", user, score);
		fclose(f);
	}
}

void printTop5()
{
	FILE* f = fopen("scores.txt", "r");
	if (f)
	{
		Record *arr = new Record[6];
		Record temp;
		char buf[256];
		while (nullptr != fgets(buf, sizeof(buf), f))
		{
			int k;
			for (k = 0; buf[k] != '\0'; k++)
			{
				if (buf[k] == ':')
				{
					temp.name[k] = '\0';
					break;
				}
				temp.name[k] = buf[k];
				
			}
			char numbuf[8];
			strcpy(numbuf, &buf[k + 2]);
			temp.score = atoi(numbuf);
			arr[0] = temp;
			for (int i = 0; i < 5; i++)
			{
				if (arr[i].score > arr[i + 1].score)
				{
					temp = arr[i];
					arr[i] = arr[i + 1];
					arr[i + 1] = temp;
				}
			}
		}
		for (int i = 5; i > 0; i--)
		{
			cout << arr[i].name<<arr[i].score << endl;
		}
	}
}
