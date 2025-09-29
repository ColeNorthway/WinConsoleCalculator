#include <list>
#include <string>
#include "controller.h"
#include "arithmetic.h"
#include "execution.h"
using namespace std;



wstring executeFuncs(wstring Func)
{
	list<double> dubArgs;
	wstring nums = L"0123456789.";

	if (Func.find(L"log(") != -1)//checking to see if func i log(x)(y) to handle double args
	{
		int wLen = Func.length();//conv wstr to wchar_t
		wchar_t wArr[wLen + 1];
		wcscpy(wArr, Func.c_str());
		//setting up wchar_t pointer and vars for token loop
		wchar_t* buffer;
		wchar_t* token;
		token = wcstok(wArr, L"()", &buffer);
		//starting from second iteration to avoid passing 'log' as an arg
		token = wcstok(NULL, L"()", &buffer);
		while (token != NULL)//looping thu tokens and adding to dubArgs
		{
			wstring wLogArg(token);
			dubArgs.push_back(stod(wstring(wLogArg.begin(),wLogArg.end())));
			token = wcstok(NULL, L"()", &buffer);
		}
		//adding result to here now
		return to_wstring(logarithm(dubArgs.front(), dubArgs.back()));
		dubArgs.clear();
	} else if (Func.find(L"^") != -1)
	{
		int wLen = Func.length();//conv wstr to wchar_t
		wchar_t wArr[wLen + 1];
		wcscpy(wArr, Func.c_str());
		//setting up token vars
		wchar_t* buffer;
		wchar_t* token;
		token = wcstok(wArr, L"^", &buffer);
		while (token != NULL)//looping thu tokens and adding to dubArgs
		{
			wstring exponentialArg(token);
			dubArgs.push_back(stod(wstring(exponentialArg.begin(),exponentialArg.end())));
			token = wcstok(NULL, L"^", &buffer);
		}
		//adding result to here now
		return to_wstring(exponential(dubArgs.front(), dubArgs.back()));
		dubArgs.clear();
	} else if (Func.find(L"log10(") != -1)
	{
		wstring wLog10Arg;//looping thru to grab arg
		wchar_t  dec{ L'.' };
		for (int i = 0; i < Func.length(); i++)
		{
			if (nums.find(Func[i]) != -1 && i > 4)
			{
				wLog10Arg += Func[i];
			}
		}
		if (wLog10Arg[0] == dec)//checking if arg start w/ . then 0.
		{
			wLog10Arg = L"0" + wLog10Arg;
		}
		//pushing result to list
		double log10Arg = stod(wstring(wLog10Arg.begin(), wLog10Arg.end()));
		return to_wstring(baseTenLog(log10Arg));
	} else if (Func.find(L"sqrt(") != -1)
	{
		wstring wSqrtArg;//looping thru to grab arg
		wchar_t  dec{ L'.' };
		for (int i = 0; i < Func.length(); i++)
		{
			if (nums.find(Func[i]) != -1)
			{
				wSqrtArg += Func[i];
			}
		}
		if (wSqrtArg[0] == dec)//checking if arg start w/ . then 0.
		{
			wSqrtArg = L"0" + wSqrtArg;
		}
		//pushing result to list
		double sqrtArg = stod(wstring(wSqrtArg.begin(), wSqrtArg.end()));
		return to_wstring(squareRoot(sqrtArg));
	} else if (Func.find(L"ln(") != -1)
	{
		wstring wLnArg;//looping thru to grab arg
		wchar_t  dec{ L'.' };
		for (int i = 0; i < Func.length(); i++)
		{
			if (nums.find(Func[i]) != -1)
			{
				wLnArg += Func[i];
			}
		}
		if (wLnArg[0] == dec)//checking if arg start w/ . then 0.
		{
			wLnArg = L"0" + wLnArg;
		}
		//pushing result to list
		double lnArg = stod(wstring(wLnArg.begin(), wLnArg.end()));
		return to_wstring(natLog(lnArg));
	} else if (Func.find(L"!") != -1)
	{
		wstring wFactArg;//looping thru to grab arg
		wchar_t  dec{ L'.' };
		for (int i = 0; i < Func.length(); i++)
		{
			if (nums.find(Func[i]) != -1)
			{
				wFactArg += Func[i];
			}
		}
		if (wFactArg[0] == dec)//checking if arg start w/ . then 0.
		{
			wFactArg = L"0" + wFactArg;
		}
		//pushing result to list
		double factArg = stod(wstring(wFactArg.begin(), wFactArg.end()));
		return to_wstring(factorial(factArg));
	}

	return Func;//returns Func if no condition met

}


list<wstring> executeFuncs(list<wstring> &args, list<int> &argsOrder)
{
	list<wstring> nonFuncList;
	
	for (wstring Arg : args)
	{
		nonFuncList.push_back(executeFuncs(Arg));
	}
	return nonFuncList;
}




///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////




//make args a back to a ref if we find another problem
wstring execute(list<wstring> &args, list<int> &argsOrder)
{
	list<wstring> copyToArgs = args;//helper list to copy to then clear args and dump this there

	while (true)
	{
		wstring argsArray[copyToArgs.size()];//dumping list to array to direct access to do funcs
		//testing DELETE
		int size = copyToArgs.size();
		for (int i = 0; i < size; i++)
		{
			argsArray[i] = copyToArgs.front();
			copyToArgs.pop_front();
		}

		//if statement to exit infinite loop
		if (sizeof(argsArray)/sizeof(argsArray[0]) == 1)
		{
			return argsArray[0];
		}

		pemdasMap(copyToArgs, &argsOrder);
		int highestOrder = 7;
		for (int value : argsOrder)
		{
			if (value < highestOrder && value != 0)
			{
				highestOrder = value;
			}
		}
		//main iterator
		//iterates, finds basic operator, evals the args, drops literal args, pushes the result in place of the expression
		for (int i = 0; i < size; i++)
		{
			if (argsOrder.front() == 3 && highestOrder == 3)
			{
				double arg1 = stod(wstring(argsArray[i-1].begin(), argsArray[i-1].end()));
				double arg2 = stod(wstring(argsArray[i+1].begin(), argsArray[i+1].end()));
				wstring result = to_wstring(multiply(arg1,arg2));
				//going to drop these and replace with 
				argsArray[i-1] = L"";
				argsArray[i] = L"&";
				argsArray[i+1] = L"";
				copyToArgs.clear();//clearing so we can fill up the right way
				for (wstring arg : argsArray)
				{
					if (arg == L"&")
					{
						copyToArgs.push_back(result);
					} else if (arg != L"")
					{
						copyToArgs.push_back(arg);
					}
				}
				argsOrder.clear();
				pemdasMap(copyToArgs, &argsOrder);
				break;
			} else if (argsOrder.front() == 4 && highestOrder == 4)
			{
				double arg1 = stod(wstring(argsArray[i-1].begin(), argsArray[i-1].end()));
				double arg2 = stod(wstring(argsArray[i+1].begin(), argsArray[i+1].end()));
				wstring result = to_wstring(divide(arg1,arg2));
				//going to drop these and replace with 
				argsArray[i-1] = L"";
				argsArray[i] = L"&";
				argsArray[i+1] = L"";
				copyToArgs.clear();//clearing so we can fill up the right way
				for (wstring arg : argsArray)
				{
					if (arg == L"&")
					{
						copyToArgs.push_back(result);
					} else if (arg != L"")
					{
						copyToArgs.push_back(arg);
					}
				}
				argsOrder.clear();
				pemdasMap(copyToArgs, &argsOrder);
				break;
			} else if (argsOrder.front() == 5 && highestOrder == 5)
			{
				double arg1 = stod(wstring(argsArray[i-1].begin(), argsArray[i-1].end()));
				double arg2 = stod(wstring(argsArray[i+1].begin(), argsArray[i+1].end()));
				wstring result = to_wstring(add(arg1,arg2));
				//going to drop these and replace with 
				argsArray[i-1] = L"";
				argsArray[i] = L"&";
				argsArray[i+1] = L"";
				copyToArgs.clear();//clearing so we can fill up the right way
				for (wstring arg : argsArray)
				{
					if (arg == L"&")
					{
						copyToArgs.push_back(result);
					} else if (arg != L"")
					{
						copyToArgs.push_back(arg);
					}
				}
				argsOrder.clear();
				pemdasMap(copyToArgs, &argsOrder);
				break;
			} else if (argsOrder.front() == 6 && highestOrder == 6)
			{
				double arg1 = stod(wstring(argsArray[i-1].begin(), argsArray[i-1].end()));
				double arg2 = stod(wstring(argsArray[i+1].begin(), argsArray[i+1].end()));
				wstring result = to_wstring(subtract(arg1,arg2));
				//going to drop these and replace with 
				argsArray[i-1] = L"";
				argsArray[i] = L"&";
				argsArray[i+1] = L"";
				copyToArgs.clear();//clearing so we can fill up the right way
				for (wstring arg : argsArray)
				{
					if (arg == L"&")
					{
						copyToArgs.push_back(result);
					} else if (arg != L"")
					{
						copyToArgs.push_back(arg);
					}
				}
				argsOrder.clear();
				pemdasMap(copyToArgs, &argsOrder);
				break;
			}///////////////////////////////////////////////////////////
			else
			{
				argsOrder.pop_front();
			}
		}
	}
}


wstring executeParen(list<wstring>* args, list<int> &argsOrder)
{
	//this is copied list to send to execute
	list<wstring> nonFuncArgs;
	
	//pops empty strings (where () are)
	if (args->front() == L"")
	{
		args->pop_front();
		argsOrder.pop_front();
	} if (args->back() == L"")
	{
		args->pop_back();
		argsOrder.pop_back();
	}


	//looping through and eval func then send to execute
	for (auto currentString = args->begin(); currentString != args->end(); ++currentString)
	{
		if (argsOrder.front() == 2)//checking for func in ()
		{
			if (currentString->find(L"log(") != -1)//checking to see if func i log(x)(y) to handle double args
			{
				wstring funcResult = executeFuncs(*currentString);
				nonFuncArgs.push_back(funcResult);
			} else if (currentString->find(L"^") != -1)
			{
				wstring funcResult = executeFuncs(*currentString);
				nonFuncArgs.push_back(funcResult);
			} else if (currentString->find(L"log10(") != -1)
			{
				wstring funcResult = executeFuncs(*currentString);
				nonFuncArgs.push_back(funcResult);
			} else if (currentString->find(L"sqrt(") != -1)
			{
				wstring funcResult = executeFuncs(*currentString);
				nonFuncArgs.push_back(funcResult);
			} else if (currentString->find(L"ln(") != -1)
			{
				wstring funcResult = executeFuncs(*currentString);
				nonFuncArgs.push_back(funcResult);
			} else if (currentString->find(L"!") != -1)
			{
				wstring funcResult = executeFuncs(*currentString);
				nonFuncArgs.push_back(funcResult);
			}
		} else
		{
			nonFuncArgs.push_back(*currentString);
		}
		argsOrder.pop_front();
	}
	list<int> nonFuncArgsOrder;
	//remapping the non func in ()
	pemdasMap(nonFuncArgs, &nonFuncArgsOrder);
	//exec whats in () to be added to the final expression in controller
	wstring Result = execute(nonFuncArgs, nonFuncArgsOrder);
	return Result;
}
