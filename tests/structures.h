struct myStruct
{
	int a;
	char b;
};

struct myStruct2
{
	struct myStruct myStructVar;
	struct myStruct2 *myStruct2Ptr;
	struct myStruct2 **myStruct2PtrPtr;
};