// Windows GDI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include<Windows.h>
using namespace std;

int xi, yi, xf, yf, xint, yint;
char buf[8];
bool RECTANGLE = false, ELLIPSE = false, LINE = false, PEN = false, BUTDOWN = false, BRUSH = false;
int color = 0x000000;
int r, g, b;
int PENSIZE = 2;

HWND hw;
DWORD rgbCurrent;

//BITMAP b;



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC h = GetDC(hWnd);
    HPEN pen = CreatePen(PS_SOLID, PENSIZE, (COLORREF)color);
    HBRUSH hbrush = CreateSolidBrush(rgbCurrent);
    if (BRUSH)SelectObject(h, hbrush);
    else SelectObject(h, pen);
    
    if (LINE || PEN)
        MoveToEx(h, xi, yi, (LPPOINT)nullptr);
    switch (message)
    {
    case WM_LBUTTONDOWN: 
    {
        if (RECTANGLE || ELLIPSE || PEN || LINE||BRUSH)
        {
            xi = LOWORD(lParam);
            yi = HIWORD(lParam);
            xint = xi;
            yint = yi;
        }
        if (PEN)BUTDOWN = true;
        break;
    }
    case WM_LBUTTONUP:
    {
        xf = LOWORD(lParam);
        yf = HIWORD(lParam);
        BUTDOWN = false;
        if (RECTANGLE)
        {
            Rectangle(h, xi, yi, xf, yf);
            RECTANGLE = false;
        }
        
        if (ELLIPSE)
        {
            Ellipse(h, xi, yi, xf, yf);
            ELLIPSE = false;
        }
        if (LINE)
        {
            LineTo(h, xf, yf);
            LINE = false;
        }
        if (PEN)
        {
            LineTo(h, xf, yf);
            PEN = false;
        }
        /*if (BRUSH)
        {
            LineTo(h, xf, yf);
            BRUSH = false;
        }*/
        break;
        
    }
    case WM_MOUSEMOVE:
    {
        xf = LOWORD(lParam);
        yf = HIWORD(lParam);
        if (PEN && BUTDOWN)// || BRUSH&&BUTDOWN)
        {
            xi = LOWORD(lParam);
            yi = HIWORD(lParam);
            LineTo(h, xi, yi);
        }
        break;
    }
    case WM_COMMAND:
    
        //cout << "WPARAM : " << wParam << "\nLPARAM :" << lParam << endl;
        switch (wParam)
        {
        case 1:
        {
            RECTANGLE = true;
            //MessageBoxA(0, "", "MB 1 pressed", 0);
            ////h= GetDC(hWnd);
            //
            //HPEN pen = CreatePen(PS_SOLID, 2, 0xFF0000);
            //SelectObject(h, pen);
            //Ellipse(h, 10, 10, 30, 30);
            //DeleteObject(pen);
            PENSIZE = 2;
            break;
        }
        case 2:
            PENSIZE = 2;
            ELLIPSE = true;
            break;
        case 3:
            PENSIZE = 2;
            LINE = true;
            break;
        case 4:
            PENSIZE = 2;
            PEN = true;
            //BRUSH = false;
            break;
        case 5:
            BRUSH = true;
            //PEN = false;
                             // common dialog box structure 
            static COLORREF acrCustClr[16]; // array of custom colors 
            //HBRUSH hbrush;                  // brush handle
                    // initial color selection
            CHOOSECOLOR cc;
            // Initialize CHOOSECOLOR 
            ZeroMemory(&cc, sizeof(cc));
            cc.lStructSize = sizeof(cc);
            cc.hwndOwner = hWnd;
            cc.lpCustColors = (LPDWORD)acrCustClr;
            cc.rgbResult = rgbCurrent;
            cc.Flags = CC_FULLOPEN | CC_RGBINIT;

            if (ChooseColor(&cc) == TRUE)
            {
                hbrush = CreateSolidBrush(cc.rgbResult);
                rgbCurrent = cc.rgbResult;
            }
            break;
        case 6:
        {

            color = 0;
            int n = 1;
            for (int i = 5; i > -1; i--)
            {
                if (buf[i] >= '0' && buf[i] <= '9')
                    color = color + n * (buf[i] - '0');
                if (buf[i] >= 'a' && buf[i] <= 'f')
                    color = color + n * (buf[i] - 'a' + 10);
                if (buf[i] >= 'A' && buf[i] <= 'F')
                    color = color + n * (buf[i] - 'A' + 10);
                n *= 16;
            }
            cout << color << endl;
            break;
        }
        case 7:
            PEN = true;
            color = 0xFFFFFF;
            PENSIZE = 10;
            break;
        case 8:
        {
            int buf[256];
            
            
            



            if (true)
            {
                OPENFILENAME ofn;       // common dialog box structure
                char szFile[260];       // buffer for file name
                //HWND hwnd;              // owner window
                FILE* hf;              // file handle

                // Initialize OPENFILENAME
                ZeroMemory(&ofn, sizeof(ofn));
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = hWnd;
                ofn.lpstrFile = szFile;
                // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
                // use the contents of szFile to initialize itself.
                ofn.lpstrFile[0] = '\0';
                ofn.nMaxFile = sizeof(szFile);
                ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
                ofn.nFilterIndex = 1;
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = NULL;
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                // Display the Open dialog box. 

                if (GetSaveFileName(&ofn) == TRUE)
                {
                    hf = fopen(ofn.lpstrFile, "w");
                    if (hf)
                    {
                        cout << "FILE OPENED SUCCESSFULLY\n";
                        for (int i = 150; i < 300; i++)
                        {
                            for (int j = 150; j < 300; j++)
                            {
                                COLORREF c = GetPixel(h, i, j);
                                fprintf(hf, "%x ", c);
                            }
                            fprintf(hf, "\n");
                        }

                        //fprintf(hf, "%p\n", hBitmap);
                        fclose(hf);
                    }
                    else cout << "Error opening\n";
                }
                else cout << "Didn't get name\n";
            }
            break;
        }
        case 9:
        {
            OPENFILENAME ofn;       // common dialog box structure
            char szFile[260];       // buffer for file name
            //HWND hwnd;              // owner window
            FILE* hf;              // file handle

            // Initialize OPENFILENAME
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hWnd;
            ofn.lpstrFile = szFile;
            // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
            // use the contents of szFile to initialize itself.
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(szFile);
            ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            // Display the Open dialog box. 

            if (GetSaveFileName(&ofn) == TRUE)
            {
                hf = fopen(ofn.lpstrFile, "r");
                if (hf)
                {
                    HDC dc = GetDC(hw);
                    cout << "FILE OPENED SUCCESSFULLY\n";
                    for (int i = 150; i < 300; i++)
                    {
                        for (int j = 150; j < 300; j++)
                        {
                            COLORREF c;
                            fscanf(hf, "%x", &c);
                            //cout << c << endl;
                            if (-1 == SetPixel(dc, i, j, c))
                                cout << "Error setting pixel\n";
                        }
                    }
                    ReleaseDC(hw, dc);
                    //fprintf(hf, "%p\n", hBitmap);
                    fclose(hf);
                }
                else cout << "Error opening\n";
            }
            else cout << "Didn't get name\n";
            }
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        DeleteObject(pen);
        DeleteObject(hbrush);
        ReleaseDC(hWnd, h);
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    DeleteObject(pen);
    DeleteObject(hbrush);
    ReleaseDC(hWnd, h);
    return 0;
}
int main()
{

    WNDCLASSEXW wcex = {};

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc = WndProc;
    wcex.lpszClassName = L"xyz";
    RegisterClassExW(&wcex);

    hw = CreateWindowA("xyz", "Paint Lite", WS_OVERLAPPED | WS_VISIBLE, 20, 20, 800, 600, 0, 0, 0, 0);

    HWND bh1 = CreateWindowA("button", "Rectangle", WS_CHILD | WS_VISIBLE, 20, 50, 70, 20, hw,(HMENU)1,0,0 );
    HWND bh2 = CreateWindowA("button", "Ellipse", WS_CHILD | WS_VISIBLE, 20, 100, 70, 20, hw, (HMENU)2, 0, 0);
    HWND bh3 = CreateWindowA("button", "Line", WS_CHILD | WS_VISIBLE, 20, 150, 70, 20, hw, (HMENU)3, 0, 0);
    HWND bh4 = CreateWindowA("button", "Pen", WS_CHILD | WS_VISIBLE, 20, 200, 70, 20, hw, (HMENU)4, 0, 0);
    HWND bh5 = CreateWindowA("button", "Brush", WS_CHILD | WS_VISIBLE, 20, 250, 70, 20, hw, (HMENU)5, 0, 0);
    
    //HWND canvas = CreateWindowA("xyz", "Click", WS_CHILD | WS_VISIBLE, 100, 100, 600, 450, h, 0, 0, 0);


    HWND color1 = CreateWindowA("Edit", "FF0000", WS_CHILD | WS_VISIBLE, 50, 300, 100, 20, hw, (HMENU)10, 0, 0);
    HWND bh6 = CreateWindowA("button", "Submit color", WS_CHILD | WS_VISIBLE, 20, 350, 90, 20, hw, (HMENU)6, 0, 0);

    HWND bh7 = CreateWindowA("button", "Eraser", WS_CHILD | WS_VISIBLE, 20, 450, 90, 20, hw, (HMENU)7, 0, 0);
    HWND bh8 = CreateWindowA("button", "Save File", WS_CHILD | WS_VISIBLE, 300, 32, 90, 20, hw, (HMENU)8, 0, 0);
    HWND bh9 = CreateWindowA("button", "Load File", WS_CHILD | WS_VISIBLE, 400, 32, 90, 20, hw, (HMENU)9, 0, 0);
    //SetWindowTextA(h, "abcd");
    
    MSG m;
    while (GetMessageA(&m, 0, 0, 0))
    {

        if (!GetWindowTextA(color1, buf, 8))
        {
            cout << "Error\n";
        }
        TranslateMessage(&m);
        DispatchMessageA(&m);

    }
    return 0;
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
