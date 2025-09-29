#include <list>
#include <string>

#ifndef ERRORHANDLING_H
bool charChecker(std::wstring &args);
bool formatCheck (std::wstring &args);
bool parenNumberCheck (std::list<std::wstring> &args);
bool parenOrderCheck (std::list<std::wstring> &args);
bool parenSplitCheck (std::list<std::wstring> &args);
#endif
