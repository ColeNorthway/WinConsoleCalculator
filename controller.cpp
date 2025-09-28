#include <regex>
#include <processthreadsapi.h>
#include <string>
#include <list>
#include "controller.h"
#include "execution.h"

using namespace std;
using namespace regex_constants;


list<wstring> splitter(wstring argsToSplit, wstring del)
{
	int start, end = -1 * del.size();
	list<wstring> listArgs;
	do 
	{
		start = end + del.size();
		end = argsToSplit.find(del, start);
		listArgs.push_back(argsToSplit.substr(start, (end - start)));
	} while (end != -1);

	return listArgs;
}


bool charCheck(wstring args)
{
	wstring acceptedChar = L" 1234567890pie*/+-!^sqrt)(logln.";
	for (int i = 0; i < args.size(); i++)
	{
		//if char isn't in accepted return false
		if (acceptedChar.find(args[i]) == -1)
		{
			return false;
		}
	}
	//returns false if the char isn't in string
	return true;
}


list<wstring> funcParenFormat(list<wstring>* sepArgs)
{
	//wstring to hold chars
	wstring placeHolder;
	//final args formatted to not have any issues with funcs
	list<wstring> finalArgs;
	//regex flags and regex
	syntax_option_type ecma_script = ECMAScript;
	//looping to catch (func() or func())
	for (auto currentArg = sepArgs->begin(); currentArg != sepArgs->end(); ++currentArg)
	{
		//convert arg to pass to regex
		if (currentArg->find(L"(") != -1 && currentArg->length() > 1)
		{
			//on first match push the first ( to back then rest after to sep ( from func
			placeHolder = (*currentArg)[0];
			finalArgs.push_back(placeHolder);
			finalArgs.push_back(currentArg->substr(1, currentArg->length()));
		} else if (currentArg->find(L")") != -1 && currentArg->length() > 1)
		{
			//push the string from zero to len -1 to skip the last ) then push ) after, to successfully sep args
			placeHolder = (*currentArg)[currentArg->length()-1];
			finalArgs.push_back(currentArg->substr(0, (currentArg->length()-1)));
			finalArgs.push_back(placeHolder);
		} else
		{
			finalArgs.push_back(*currentArg);
		}
	}

	return finalArgs;
}


void pemdasMap(list<wstring> sepArgs, list<int>* sepArgsOrderPtr)
{
	wstring helperFuncs[4] = {L"sqrt(", L"ln(", L"log(", L"log10"};
	//this will break the first for loop to prevent adding 0 and 1
	int parenTypeIndicator = 0;

	//main iterator to map order to args
	for (auto currentString = sepArgs.begin(); currentString != sepArgs.end(); ++currentString)
	{
		if ((currentString->find(L"(")) != -1 || (currentString->find(L")")) != -1)
		{
			//looping to find if func is attached to paren
			for (wstring func : helperFuncs)
			{
				if (currentString->find(func) != -1)
				{
					parenTypeIndicator = 1;
				}
			}
			//checking to push 2 or 1
			if (parenTypeIndicator == 1)
			{
				sepArgsOrderPtr->push_back(2);
				parenTypeIndicator = 0;
			} else
			{
				sepArgsOrderPtr->push_back(1);
			}
		} else if (currentString->find(L"!") != -1 || currentString->find(L"^") != -1)
		{
			sepArgsOrderPtr->push_back(2);
		} else if (*currentString == L"*")
		{
			sepArgsOrderPtr->push_back(3);
		} else if (*currentString == L"/")
		{
			sepArgsOrderPtr->push_back(4);
		} else if (*currentString == L"+")
		{
			sepArgsOrderPtr->push_back(5);
		} else if (*currentString == L"-")
		{
			sepArgsOrderPtr->push_back(6);
		} else 
		{
			sepArgsOrderPtr->push_back(0);
		}
	}
}


//this will send out () to be executed 
wstring parenthesesConvert(list<wstring>* sepArgsPtr, list<int>* sepArgsOrderPtr)
{
	//used used designate expressions in ()
	list<wstring> expressionArgs;
	list<int> expressionArgsOrder;
	list<wstring> evaluatedParenArgs;//evaluated () results go here or stuff outside ()
	int inParen = 0;//later will be used to increment and decrement to find the furthest nested parentheses
	for (auto currentString = sepArgsPtr->begin(); currentString  != sepArgsPtr->end(); ++currentString)
	{
		//Dump to new args for (..)
		if (sepArgsOrderPtr->front() == 1 && inParen == 0)//setting this to dump () in the string
		{
			inParen = 1;
			expressionArgs.push_back(currentString->substr(1, (currentString->length()-1)));//excludes the first ( to keep parsing clean
		} else if (sepArgsOrderPtr->front() != 1 && inParen == 1) //if in middle dump
		{
			expressionArgs.push_back(*currentString);
		} else if (sepArgsOrderPtr->front() == 1 && inParen == 1) //dump and flip counter
		{
			inParen = 0; //decrement later
			expressionArgs.push_back(currentString->substr(0, (currentString->length()-1)));//excludes the last ) to keep parsing clean

			//starting execution of a paren and adding to final expression
			pemdasMap(expressionArgs, &expressionArgsOrder);
			wstring parenResult = executeParen(&expressionArgs, expressionArgsOrder);//return result in new string
			evaluatedParenArgs.push_back(parenResult);
			expressionArgs.clear();//clears to be able to put next () in
			expressionArgsOrder.clear();
		} else 
		{
			evaluatedParenArgs.push_back(*currentString);
		}
		//popping the front off the list to go to the next one
		sepArgsOrderPtr->pop_front();
	}

	list<int> argsOrder;
	pemdasMap(evaluatedParenArgs, &argsOrder);
	//stripping de () args of funcs
	list<wstring> deFuncArgs = executeFuncs(evaluatedParenArgs, argsOrder);
	//remapping pemdas to de func args order
	argsOrder.clear();
	pemdasMap(deFuncArgs, &argsOrder);

	//getting arith done for final result
	wstring result = execute(deFuncArgs, argsOrder);
	return result;
}


wstring parse(wstring args)
{
	//our return value
	wstring result;
	//checks that no invalid characters are in user input
	bool isValid = charCheck(args);
	if (isValid == false)
	{
		result = L"Your input has unknown characters";
		return result;
	}
	//IMPLEMENT FURTHER CHECKS HERE

	//now we will set the order to our args
	list<wstring> sepArgs = splitter(args);//DELETE 1 + (1 + 1)
	list<int> sepArgsOrder;
	//getting that final form for the args w/ de ()
	list<wstring> finalSepArgs = funcParenFormat(&sepArgs);
	pemdasMap(finalSepArgs, &sepArgsOrder);//DELETE 05151

	//execute the args and return the result
	result = parenthesesConvert(&finalSepArgs, &sepArgsOrder);
	return result;
}
