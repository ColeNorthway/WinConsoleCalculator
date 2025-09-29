#include <iostream>
#include "errorHandling.h"
#include <regex>
#include <string>
#include <list>
#include "controller.h"

using namespace std;
using namespace regex_constants;


bool charChecker(wstring &args)
{
	wstring acceptedChar = L" 1234567890pie*/+-!^sqrt)(logln.";
	for (int i = 0; i < args.size(); i++)
	{
		//if char isn't in accepted return false
		if (acceptedChar.find(args[i]) == -1)
		{
			args = L"Your input has unknown characters";
			return false;
		}
	}
	//returns false if the char isn't in string
	return true;
}


bool formatCheck(wstring &args)
{
	list<wstring> argList = splitter(args);

	//checking low hanging fruit
	syntax_option_type ecma_script = ECMAScript;
	if (regex_search(args, wregex(L"(\\+|-|\\/|\\*){2,}", ecma_script)))
	{
		args = L"Error, invalid operator formatting";
		return false;
	} if (regex_search(args, wregex(L"([0-9]|\\.)+ ([0-9]|\\.)+", ecma_script)))
	{
		args = L"Error, missing operator";
		return false;
	} if (regex_search(args, wregex(L"[0-9]{2,}\\.", ecma_script)))
	{
		args = L"Error, invalid decimal placement on literal";
		return false;
	} if (regex_search(args, wregex(L"/([0-9.]+(\\+|-|\\/|\\*)+|(\\+|-|\\/|\\*)+[0-9.]+)/gm", ecma_script)))
	{
		args = L"Error, operators and literals must be separated by spaces";
		return false;
	} if (regex_search(args, wregex(L"sqrt\\(-[0-9.]+\\)", ecma_script)))
	{
		args = L"Error, passing negative value to a square root";
		return false;
	} if (args.size() > 150)
	{
		args = L"Error, char limit is 150";
		return false;
	} if (args.find(L"sqrt(") == -1 && args.find(L"ln") == -1 && args.find(L"!") == -1 && args.find(L"log") == -1)
	{
		if (args.find(L"+") == -1 && args.find(L"*") == -1 && args.find(L"/") == -1 && args.find(L"-") == -1)
		{
			args = L"Error no operators were found only literals";
			return false;
		}
	}

	//checking dec count
	int count = 0;
	wchar_t dec = L'.';
	for (wstring lilOOF : argList)
	{
		for (int i = 0; i < lilOOF.length(); i++)
		{
			if (lilOOF[i] == dec)
			{
				count++;
			} if (count > 1)
			{
				args = L"Error, invalid amount of decimals";
				return false;
			}
		}
		count = 0;
	}
	return true;
}


bool parenNumberCheck(list<wstring> &args)
{
	int parenCount = 0;
	for (wstring arg : args)
	{
		if (arg == L")" or arg == L"(")
		{
			parenCount++;
		}
	}
	if (parenCount % 2 != 0)
	{
		wcout << L"Error, invalid amount of parentheses\n";
		return false;
	}
	return true;
}

bool parenOrderCheck(list<wstring> &args)
{
	wstring parenO = L"(";
	wstring parenC = L")";

	for (wstring arg : args)
	{
		if (arg == parenO || arg == parenC)
		{
			if (arg == parenC)
			{
				wcout << L"Error, invalid orientation of parentheses\n";
				return false;
			}
			break;
		}
	}
	for (auto rIT = args.rbegin(); rIT != args.rend(); ++rIT)
	{
		if (*rIT == parenO || *rIT == parenC)
		{
			if (*rIT == parenO)
			{
				wcout << L"Error, invalid orientation of parentheses\n";
				return false;
			}
			break;
		}
	}
	return true;
}

bool parenSplitCheck(list<wstring> &args)
{
	int openParen = 0;
	int closedParen = 0;

	for (wstring arg : args)
	{
		if (arg == L"(")
		{
			openParen++;
		} else if (arg == L")")
		{
			closedParen++;
		}
	}

	if (openParen != closedParen)
	{
		return false;
	}
	return true;
}
//first lets build a 
