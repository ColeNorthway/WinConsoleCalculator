#include <map>
#include <regex>
#include <processthreadsapi.h>
#include <string>
#include <list>
#include "controller.h"
#include "execution.h"
#include "errorHandling.h"

using namespace std;
using namespace regex_constants;


static map<int,int> parenMap;


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
		//checking if string stards with (
		if (regex_match(*currentArg, wregex(L"\\(.+", ecma_script)))
		{ //parent ( parse
			wchar_t  P{ L'(' };
			wstring Wp (L"(");
			for (int i = 0; i < currentArg->length(); i++)
			{
				//pushing back the (
				if ((*currentArg)[i] == P)
				{
					finalArgs.push_back(Wp);
					continue;
				//pushing back the rest of non (
				}
				finalArgs.push_back(currentArg->substr(i, (currentArg->length() - 1)));
				break;
			}
		} else if (regex_match(*currentArg, wregex(L".+\\)$", ecma_script)))
		{ //parent ) parse
			list<wstring> parenToPush;
			wchar_t  P{ L')' };
			wstring Wp (L")");
			if (!regex_search(*currentArg, wregex(L"(sqrt|ln|log)", ecma_script)))
			{ //no func just popping ))
				for (int i = currentArg->length()-1; i >= 0; i--)
				{
					if ((*currentArg)[i] == P)
					{
						parenToPush.push_back(Wp);
						continue;
					}
					//pushing func
					finalArgs.push_back(currentArg->substr(0, i + 1));
					//pushing paren
					for (wstring Paren : parenToPush)
					{
						finalArgs.push_back(Paren);
					}
					break;
				}
			} else if (regex_match(*currentArg, wregex(L"^(sqrt|log|ln).+\\)\\)+$", ecma_script)))
			{ //func w/ ))
				for (int i = currentArg->length()-1; i >= 0; i--)
				{
					if ((*currentArg)[i] == P)
					{
						parenToPush.push_back(Wp);
						continue;
					}
					//pushing func arg with paren
					finalArgs.push_back(currentArg->substr(0, i + 2));
					//making true paren even
					parenToPush.pop_back();
					//pushing paren
					for (wstring Paren : parenToPush)
					{
						finalArgs.push_back(Paren);
					}
					break;
				}
			} else
			{
				finalArgs.push_back(*currentArg);
			}
		} else
		{
			finalArgs.push_back(*currentArg);
		}
	}
	return finalArgs;
}


void pemdasMap(list<wstring> sepArgs, list<int>* sepArgsOrderPtr)
{
	//clearing the paren global vars
	parenMap.clear();

	wstring helperFuncs[4] = {L"sqrt(", L"ln(", L"log(", L"log10"};
	//this will break the first for loop to prevent adding 0 and 1
	int parenTypeIndicator = 0;
	//len tracker
	int len = -1;
	//tracking the most nested ()
	int priority = 0;
	int highestPriority = 0;

	//main iterator to map order to args
	for (auto currentString = sepArgs.begin(); currentString != sepArgs.end(); ++currentString)
	{
		//inc len
		len++;

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
				if (*currentString == L"(")
				{//pushing start ( to paren map
					priority++;
					parenMap[len] = priority;
					//setting highest priority
					if (priority > highestPriority)
					{
						highestPriority = priority;
					}
				} else if (*currentString == L")")
				{//pushing 1 to parenMap
					priority--;
				}
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
	//removing all but highest priority for map
	for (auto indexPriority : parenMap)
	{
		if (indexPriority.second < highestPriority)
		{
			parenMap.erase(indexPriority.first);
		}
	}
}


//this will send out () to be executed AND EVENTUALLY FINISH EXEC!
wstring parenthesesConvert(list<wstring> &sepArgs, list<int> &sepArgsOrder)
{
	//used used designate expressions in ()
	list<wstring> parenArgs;
	list<int> parenArgsOrder;
	//evaluated () results go here or stuff outside ()
	list<wstring> evaluatedParenArgs;
	//helper list for copy and clear
	//counter
	int inParen = 0;
	int len = -1;

	//enter main exec loop
	goto mainLoop;

/////////////////////////////////////////////////////
mainLoop:
	//first highest priority index
	auto parenFirst = parenMap.begin();
	for (auto currentString = sepArgs.begin(); currentString  != sepArgs.end(); ++currentString)
	{
		//tracking len
		len++;
		//checking to see if we are on a ( and if that is an index of highest order (
		if (sepArgsOrder.front() == 1 && len == parenFirst->first || inParen == 1)
		{
			if (inParen == 0)
			{//setting this to dump () in the string
				inParen = 1;
				//excludes the first ( to keep parsing clean
				parenArgs.push_back(currentString->substr(1, (currentString->length()-1)));
			} else if (sepArgsOrder.front() != 1 && inParen == 1) //if in middle dump
			{
				parenArgs.push_back(*currentString);
			} else if (sepArgsOrder.front() == 1 && inParen == 1) //dump and flip counter
			{
				inParen = 0; //decrement later
				parenArgs.push_back(currentString->substr(0, (currentString->length()-1)));//excludes the last ) to keep parsing clean

				//starting execution of a paren and adding to final expression
				pemdasMap(parenArgs, &parenArgsOrder);
				//return result in new string
				wstring parenResult = executeParen(&parenArgs, parenArgsOrder);
				//pushing eval () to final de() list
				evaluatedParenArgs.push_back(parenResult);
				parenArgs.clear();//clears to be able to put next () in
				parenArgsOrder.clear();
			}
		} else 
		{
			evaluatedParenArgs.push_back(*currentString);
		}
		//popping the front off the list to go to the next one
		sepArgsOrder.pop_front();
	}

	//resetting len for next loop
	len = -1;
	//pushing the new args to sepArgs for next iteration
	sepArgs.clear();
	sepArgsOrder.clear();
	for (wstring arg : evaluatedParenArgs)
	{
		sepArgs.push_back(arg);
	}
	evaluatedParenArgs.clear();
	//pemdas mapping sepArgs and sepArgs order
	//if parenMap is empty exit.
	pemdasMap(sepArgs, &sepArgsOrder);
	if (parenMap.size() == 0)
	{
		goto exit;
	} else
	{
		goto mainLoop;
	}
	
/////////////////////////////////////////////////////
exit:

	//stripping de () args of funcs
	list<wstring> deFuncArgs = executeFuncs(sepArgs, sepArgsOrder);
	//remapping pemdas to de func args order
	sepArgsOrder.clear();
	pemdasMap(deFuncArgs, &sepArgsOrder);

	//getting arith done for final result
	wstring result = execute(deFuncArgs, sepArgsOrder);
	return result;
}


wstring parse(wstring args)
{
	//our return value
	wstring result;
	//doing format checks
	bool isValid = formatCheck(args);
	isValid = charChecker(args);
	if (isValid == false)
	{
		result = args;
		return result;
	}

	//now we will set the order to our args
	list<wstring> sepArgs = splitter(args);
	list<int> sepArgsOrder;
	//getting that final form for the args w/ de ()
	list<wstring> finalSepArgs = funcParenFormat(&sepArgs);
	//doing parentheses checks on finalSepArgs
	isValid = parenNumberCheck(finalSepArgs);
	isValid = parenOrderCheck(finalSepArgs);
	isValid = parenSplitCheck(finalSepArgs);
	if (isValid == false)
	{
		result = args;
		return result;
	}

	pemdasMap(finalSepArgs, &sepArgsOrder);

	//execute the args and return the result
	result = parenthesesConvert(finalSepArgs, sepArgsOrder);
	return result;
}
