#include "include/otherInclude.h"
#include "macros.h"

struct myStruct
{
	MyInt i;
	MyUInt ui;
	MyLong l;
	MyFloat f;
	MyDouble d;
	MyChar c;
};

struct myStruct2
{
	struct myStruct;
	char test[100];
	int a[SIZE];
};
