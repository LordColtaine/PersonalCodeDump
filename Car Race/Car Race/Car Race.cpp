// Car Race.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include<Windows.h>
#include <WinBase.h>
#include<cstdlib>
#include "Scores.h"
using namespace std;
int main()
{
    int pos = 31;
    int count = 0, obstcount = 0, obspos = rand() % 63 + 1;
    int wallposleft, wallposright;
    bool obsflag = true, wallflagleft = true, wallflagright = true;
    int wallcount = 0, walltimer = 0, nowalltimer = 0, randomval;

    int score = 0;
    while (1)
    {
        char screen[64] = "0                                                             0";

        //GENERATE CARS/OBSTACLES
        count++;
        if (10 == count)
        {
            count--;
            obstcount++;
            if (obsflag)
            {
                obsflag = false;
                screen[obspos] = '!';
            }
            else obsflag = true;
            if (obstcount == 9)
            {
                screen[obspos] = '0';
                obstcount = count = 0;
                obspos = rand() % 63 + 1;
                obsflag = true;
            }
        }

        nowalltimer++;
        if (20 < nowalltimer)
        {
            
            if (0 == wallcount)
            {
                randomval = rand() % 30;
                if (randomval < 10)
                {
                    wallflagleft = true;
                    wallflagright = false;
                    wallposleft = rand() % 31 + 1;
                }
                else if (randomval < 20)
                {
                    wallflagleft = true;
                    wallflagright = true;
                    wallposleft = rand() % 31 + 1;
                    wallposright = rand() % 31 + 32;
                }
                else
                {
                    wallflagleft = false;
                    wallflagright = true;
                    wallposright = rand() % 31 + 32;
                }
            }
            wallcount++;
            if (10 < wallcount)
            {
                walltimer++;

                if (wallflagleft)
                {
                    for (int i = 0; i <= wallposleft; i++)
                        screen[i] = '0';
                    
                }
                if (wallflagright)
                {
                    for (int i = wallposright; i <= 62; i++)
                        screen[i] = '0';
                    
                }
                if (walltimer == 15)
                {
                    walltimer = 0;
                    nowalltimer = 0;
                    wallcount = 0;
                }

            }
            else
            {
                if (wallflagleft)
                {
                    for (int i = 0; i <= wallposleft; i++)
                        screen[i] = '!';
                    
                }
                if (wallflagright)
                {
                    for (int i = wallposright; i <= 62; i++)
                        screen[i] = '!';
                    
                }
            }
        }
        

        //CRASH CONDITION
        if (screen[pos] == '0')
        {
            cout << "YOU CRASHED! Your final score : "<<score<<"\nPress enter to end" << endl;
            break;
        }

        
        screen[pos] = 'V';
        if (GetAsyncKeyState(VK_ESCAPE))
        {
            cout << "You pressed Escape! Your final Score is " << score << endl << "Press enter to exit\n";
            break;
        }
        if (GetAsyncKeyState(VK_LEFT))pos--;
        if (GetAsyncKeyState(VK_RIGHT))pos++;
        cout << screen << "    Score : "<<score<<endl;
        Sleep(100);
        if (count == 0) score += 10;
        if (nowalltimer == 0)score += 5;
    }
    char* user = new char[256];
    if (GetEnvironmentVariable(TEXT("USERNAME"), user, 256))
        writeToFile(user, score);
    printTop5();
    getchar();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
