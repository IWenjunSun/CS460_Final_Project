#include <Windows.h>
#include <iostream>
//https://guidedhacking.com
//Traxin's How To Call Game Function Tutorial


using std::cout;
using std::endl;

void FuncA()
{
	cout << "I'm function a!" << endl;
}

void FuncB(char * string)
{
	cout << string << endl;
}

int main()
{
	while (true)
	{
		if (GetAsyncKeyState(VK_F2) & 1)
		{
			FuncA();
			FuncB("I'm function b!");
		}
	}
	return 0;
}