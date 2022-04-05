#pragma once
#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;

namespace MuSeoun_Engine
{
	class MConsoleRenderer
	{
	public :
		MConsoleRenderer() { SetCursorState(false); }
		~MConsoleRenderer() {}

		void MoveCursor(short x, short y)
		{
			COORD position = { x , y };
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
		}

		void SetCursorState(bool visible)
		{
			CONSOLE_CURSOR_INFO consoleCursorInfo;
			consoleCursorInfo.bVisible = visible;
			consoleCursorInfo.dwSize = 1;
			SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleCursorInfo);
		}

		void DrawString(string s)
		{
			cout << s;
		}

		void Clear()
		{
			system("cls");
		}
	};
	
}