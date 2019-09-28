#include <iostream>
#include <string>

#include "./MyGL.hpp"

int main(int argc,char *argv[])
{
	std::string window_name = "Sample";
	MyGL test;

	test.Init(window_name,500,700,argc,argv);
	test.EntryCallBack(false);

	while(true)
	{
		test.MainLoop();
		
		test.Clear();
		test.DrawHorizon(100,100,1.0);
		test.Display();
		
		std::cout << test.GetKey() << ":" << test.GetKeyUp() << std::endl;
	}

	return 0;
}
