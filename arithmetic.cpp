#include <math.h>
#include "arithmetic.h"
#include "controller.h"

using namespace std;

double add(double x, double y)
{
	double z = x + y;
	return z;
}

double subtract(double x, double y)
{
	double z = x - y;
	return z;
}

double multiply(double x, double y)
{
	double z = x * y;
	return z;
}

double divide(double x, double y)
{
	double z = x / y;
	return z;
}

double factorial(int x)
{
	int multiples[x];
	for (int i = 1; i <= x; i++)
	{
		multiples[i - 1] = i;
	}


	int helper = 1;
	for (int i = 0; i < x; i++)
	{
		helper *= multiples[i];
	}
	return (double)helper;
}

double exponential(double x, double y)
{
	return pow(x, y);
}

double squareRoot(double x)//works
{
	return sqrt(x);
}

double logarithm(double x, double y)
{
	double logg = log(y) / log(x);
	return logg;
}

double natLog(double x)//works
{
	return log(x);
}

double baseTenLog(double x)//works
{
	return log10(x);
}
