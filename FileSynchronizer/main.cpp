#include "InputReader.hpp"

int main(int argc, char** argv)
{
	try
	{
		InputReader r;
		r.startConversation();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
	
	//example command
	//-l "path-to-folder1" -r "path-to-folder2" -f file.txt -d -Q
	
}

