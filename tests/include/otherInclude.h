#define SIZE_MAJOR		2
#define SIZE_MINOR		1
#define SIZE 			(SIZE_MAJOR + SIZE_MINOR)

typedef struct otherStruct
{
	int a;
	char b;
} otherStruct;

typedef enum 
{
	enum99,
	enum100,
} otherEnum;

int f(char **argv);
