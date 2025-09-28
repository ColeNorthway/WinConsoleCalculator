#include <string>
#include <list>

#ifndef EXECUTION_H
std::wstring execute(std::list<std::wstring> &args, std::list<int> &argsOrder);
std::wstring executeParen(std::list<std::wstring>* args, std::list<int> &argOrder);
std::wstring executeFuncs(std::wstring);
std::list<std::wstring> executeFuncs(std::list<std::wstring> &args, std::list<int> &argsOrder);
#endif
