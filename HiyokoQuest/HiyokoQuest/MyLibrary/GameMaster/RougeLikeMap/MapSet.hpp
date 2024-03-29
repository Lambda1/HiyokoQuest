#ifndef __MAP_SET_HPP__
#define __MAP_SET_HPP__

/* 共通型定義 */
using MAP_TYPE = unsigned char;

/* ローグライク: シンプルダンジョン要素 */
namespace MAPSET
{
	enum class DATA : unsigned char
	{
		NONE,   /* 無 */
		ROOM,   /* 部屋 */
		ROAD,   /* 道 */
		WALL,   /* 壁 */

		STAIR,  /* 階段 */

		PLAYER, /* プレイヤー */

		ENEMY,  /* 敵 (素体: テスト) */

		ENEMY1, /* 敵 (アヒル) */
		ENEMY2, /* 敵 (蟻) */
		ENEMY3, /* 敵 (ビショップ) */
		ENEMY4, /* 敵 (豚) */
		ENEMY5, /* 敵 (かぼちゃ) */

		SIZE    /* 要素数 */
	};
};

#endif
