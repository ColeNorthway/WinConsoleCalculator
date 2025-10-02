#include <list>
#include <string>

#ifndef ERRORHANDLING_H
bool formatCheck (std::wstring &args);
bool spaceCheck (std::list<std::wstring> &args, std::wstring &result);
bool parenCheck (std::list<std::wstring> &args, std::wstring &result);
#endif
