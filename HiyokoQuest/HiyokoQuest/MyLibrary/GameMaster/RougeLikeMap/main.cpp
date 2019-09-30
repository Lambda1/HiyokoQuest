#include <iostream>
#include <string>
#include <ctime>

#include "./RougeLikeMap.hpp"
#include "./MapSet.hpp"

int main(int argc,char *argv[])
{
	constexpr int width = 200, height = 200;
	constexpr int room_num = 100;

	//RougeLikeMap<unsigned char> test(1569577908);
	RougeLikeMap<unsigned char> test(static_cast<int>(time(NULL)));
	test.Init(width, height, room_num);
	test.SetBaseInfo(static_cast<unsigned char>(MAPSET::DATA::NONE), static_cast<unsigned char>(MAPSET::DATA::ROOM) ,static_cast<unsigned char>(MAPSET::DATA::ROAD), static_cast<unsigned char>(MAPSET::DATA::WALL));
	test.Generate();
	
	test.Update();
	unsigned char *dungeon = test.GetALL();
	for(int i = 0;i < height;i++){
		for(int j = 0;j < width;j++){
			std::cout << static_cast<int>(dungeon[i*width+j]);
		}
		std::cout << std::endl;
	}

	return 0;
}
