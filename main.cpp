#include "errorHandling.h"
#include <iostream>
#include "arithmetic.h"
#include "controller.h"
#include "execution.h"
#include <string>
#include <windows.h>

using namespace std;

#define ESC L"\x1b"
#define CSI L"\x1b["
#define RESET L"\x1b[0m"


class VirtualTerminal
{
	public:
		//storing the orginal virtual terminal modes
		inline static bool storeOriginalModes()
		{
			if(!GetConsoleMode(hOut, &dwOrginalOutMode))
			{
				return false;
			}
			return true;
		}
		
		//setting the current terminal to virtual
		inline static int setVirtualMode()
		{
			dwOutMode = dwOrginalOutMode | dwRequestedOutModes;
			//sets the virtual output mode to current terminal
			if (!SetConsoleMode(hOut, dwOutMode))
	   		{
				dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
				dwOutMode = dwOrginalOutMode | dwRequestedOutModes;
				if (!SetConsoleMode(hOut, dwOutMode))
				{
					return -1;
				}
			}	
			//sets the virtual input mode to current terminal
			//success
			return 0;
		}
		
		//reset the terminal to original
		inline static int setOrignalMode()
		{
			if (!SetConsoleMode(hOut, dwOrginalOutMode))
			{
				return -1;
			}
			return 0;
		}
		
	private:
		//handles
		inline static HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		inline static DWORD dwOrginalOutMode = 0;
		//new modes
		inline static DWORD dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
		//new modes combined with the og
		inline static DWORD dwOutMode;
};


class TerminalFunctions
{
	public:
		inline static void lineWipe()
		{
			//up 6 lines
			wcout << CSI << L"A";
			wcout << CSI << L"A";
			wcout << CSI << L"A";
			wcout << CSI << L"A";
			wcout << CSI << L"A";
			wcout << CSI << L"A";
			//delete two lines
			wcout << CSI << L"1M";
			wcout << CSI << L"1M";
			//moves back down
			wcout << CSI << L"B";		
			wcout << CSI << L"B";		
			wcout << CSI << L"B";		
			wcout << CSI << L"B";		
		}
};


int main()
{
	VirtualTerminal::storeOriginalModes();
	VirtualTerminal::setVirtualMode();
	//variables
	
	int counter = 0;
	wstring args;
	
	//sets the help
	wcout << CSI << L"35m" << L"parentheses are allowed\nExample: 1 + (1 + 1)\nExample: 1 / sqrt(4) - (1 + (1 + 1))\nmultiplication = *\ndivision = \\\naddition = +\nsubtraction = -\nfactorials = x!\nexponentials = x^y\nsquare roots = sqrt(x)\nlog of x = log(x)(y)\nnatural log = ln(x)\nlog base 10 = log10(x)\n" << RESET;

	while (true)
	{
		if (counter > 2)
		{
			TerminalFunctions::lineWipe();
		} else
		{
			counter++;
		}
		wcout << L">";
		getline(wcin, args);
		//breaks
		if (args == L"exit")
		{
			return 0;
		}
		wcout << parse(args) << L"\n\r";
	}

	VirtualTerminal::setOrignalMode();
	return 0;
}
