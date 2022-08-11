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
	//-l "C:\Users\Bobby\Desktop\test" -r "C:\Users\Bobby\Desktop\example" -f file.txt -s -S
	
}

