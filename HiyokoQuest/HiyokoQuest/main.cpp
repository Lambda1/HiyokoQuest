#include <iostream>
#include <string>

#include ".//MyLibrary/MyGL/MyGL.hpp"

int main(int argc, char* argv[])
{
	std::string window_name = "Sample";
	MyGL opengl_manager;

	opengl_manager.Init(window_name, 700, 500, argc, argv);
	opengl_manager.EntryCallBack(false);

	glClearColor(1,1,1,0);
	glEnable(GL_DEPTH_TEST);

	while (true)
	{
		opengl_manager.MainLoop();

		opengl_manager.Display();
	}

	return 0;
}