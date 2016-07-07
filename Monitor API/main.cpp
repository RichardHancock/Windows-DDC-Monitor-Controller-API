#include <windows.h>
#include <iostream>

int main(int, char**);
void processCommandLine(int argc, char** argv);


int main(int argc, char **argv)
{
	processCommandLine(argc, argv);


	return 0;
}

void processCommandLine(int argc, char **argv)
{
	std::cout << "No command line arguments are handled currently";
	std::cout << "These were the passed in args:";

	for (int i = 1; i < argc; i++)
	{
		std::cout << argv[i];
	}
}