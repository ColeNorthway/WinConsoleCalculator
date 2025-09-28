#include <list>
#include <string>

#ifndef CONTROLLER_H
#define CONTROLLER_H
std::wstring parse(std::wstring args);
std::list<std::wstring> splitter(std::wstring argsToSplit, std::wstring del = L" ");
bool charCheck(std::wstring args);
std::list<std::wstring> funcParenFormat(std::list<std::wstring>* sepArgs);
void pemdasMap(std::list<std::wstring> sepArgs, std::list<int>* sepArgsOrderPtr);
std::wstring parenthesesConvert(std::list<std::wstring>* sepArgsPtr, std::list<int>* sepArgsOrderPtr);
#endif
