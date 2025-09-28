#include <list>
#include <string>

#ifndef ERRORHANDLING_H
bool formatCheck (std::list<std::wstring> &args);
bool nanCheck (std::wstring result);
bool negativeSqrtCheck (std::list<std::wstring> &args);
#endif


/*
BEFORE WE EVEN GET HERE WE GOTTA WORK ON THE NESTED PAREN!!


ok we need to check the following errors
1+1
++
so we need to implement a regex checker
- basically it will have valid forms of expression
- and it always must have a space
- .numbers are allowed

Also remember divide by zero and sqrt of -
- we can just implement a final check and if we get nan
- we can basically just say "invalid, divide by zero"
- We can also check if a negative number is passed to sqrt as well in a sep check

so by checking errors and having a standardized regex format we shouldn't run into anyother errors.
*/















/*

*/






















