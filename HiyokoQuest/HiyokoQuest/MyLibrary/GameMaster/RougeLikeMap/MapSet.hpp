#ifndef __MAP_SET_HPP__
#define __MAP_SET_HPP__

/* ローグライク: シンプルダンジョン要素 */
namespace MAPSET
{
	enum class DATA : unsigned char
	{
		NONE, /* 無 */
		ROOM, /* 部屋 */
		ROAD, /* 道 */
		WALL, /* 壁 */

		SIZE  /* 要素数 */
	};
};

#endif
