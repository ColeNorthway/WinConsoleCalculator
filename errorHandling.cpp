#include "errorHandling.h"
#include <regex>
#include <string>
#include <list>
#include "controller.h"

using namespace std;
using namespace regex_constants;




bool formatCheck(wstring &args)
{
	//helper list
	list<wstring> argList = splitter(args);


	wstring acceptedChar = L" 1234567890pie*/+-!^sqrt)(logln.";
	for (int i = 0; i < args.size(); i++)
	{//checking unknown char
		//if char isn't in accepted return false
		if (acceptedChar.find(args[i]) == -1)
		{
			args = L"Your input has unknown characters";
			return false;
		}
	}

	//checking low hanging fruit
	syntax_option_type ecma_script = ECMAScript;
	if (regex_search(args, wregex(L"(\\+|-|\\/|\\*){2,}", ecma_script)))
	{//checking ++ or ---
		args = L"Error, invalid operator formatting";
		return false;
	} if (regex_search(args, wregex(L"([0-9]|\\.)+ ([0-9]|\\.)+", ecma_script)))
	{//checking missing operators
		args = L"Error, missing operator";
		return false;
	} if (regex_search(args, wregex(L"sqrt\\(-[0-9.]+\\)", ecma_script)))
	{//checking sqrt(-val)
		args = L"Error, passing negative value to a square root";
		return false;
	} if (args.size() > 150)
	{//checking char limit
		args = L"Error, char limit is 150";
		return false;
	} if (args.find(L"sqrt(") == -1 && args.find(L"ln") == -1 && args.find(L"!") == -1 && args.find(L"log") == -1)
	{//checkin for no operators
		if (args.find(L"+") == -1 && args.find(L"*") == -1 && args.find(L"/") == -1 && args.find(L"-") == -1)
		{
			args = L"Error no operators were found only literals";
			return false;
		}
	} if (regex_search(args, wregex(L"[0-9.]+(\\*|\\+|-|\\/)+", ecma_script)) || regex_search(args, wregex(L"(\\*|\\+|-|\\/)+[0-9.]+", ecma_script)))
	{//checking for <oper><num> and vice versa (no space) for single pair like +1 or 1+
		if (regex_search(args, wregex(L"(\\*|\\+|-|\\/)+ -[0-9.]", ecma_script)) || regex_search(args, wregex(L"-[0-9.] (\\*|\\+|-|\\/)+", ecma_script)))
		{//checking for -1 for exceptn
			//do nothing bc excptn
		} else
		{
			args = L"Error, operators and literals must be separated by spaces";
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


bool spaceCheck(list<wstring> &args, wstring &result)
{
	syntax_option_type ecma_script = ECMAScript;
	for (wstring oneArg : args)
	{
		if (regex_search(oneArg, wregex(L"^[0-9.]+(\\*|\\+|-|\\/)+[0-9.]$", ecma_script)))
		{
			//fix this
			result = L"Error, operators and literals must be separated by spaces";
			return false;
		} 
	}
	return true;
}

//fix these
bool parenCheck(list<wstring> &args, wstring &result)
{
	//helper var
	wstring parenO = L"(";
	wstring parenC = L")";
	int openParen = 0;
	int closedParen = 0;
	int parenCount = 0;
	
	//checking () count
	for (wstring arg : args)
	{
		if (arg == L")" or arg == L"(")
		{
			parenCount++;
		}
	}
	if (parenCount % 2 != 0)
	{
		result = L"Error with parentheses";
		return false;
	}

	//checking () orientation
	for (wstring arg : args)
	{
		if (arg == parenO || arg == parenC)
		{
			if (arg == parenC)
			{
				result = L"Error with parentheses";
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
				result = L"Error with parentheses";
				return false;
			}
			break;
		}
	}

	//checking ( to )
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
		result = L"Error with parentheses";
		return false;
	}

	return true;
}
