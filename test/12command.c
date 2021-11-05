#include "00testhelper.h"

char **parseArgument(int argc, char *argv)
{
	char** arr = (char**)lscmalloc(4* argc);
	char *p = argv;
	for (int i = 0; i < argc; i++)
	{
		arr[i] = p;
		while (*p != '\0')
		{
			p++;
		}
		p++;
	}
	return arr;
}

int main(int argc, char *argv)
{
	char **args = parseArgument(argc, argv);
	checkEquals(argc, 1, __LINE__);
	checkStringEquals(args[0], "./z.out", __LINE__);
	return 0;
}
