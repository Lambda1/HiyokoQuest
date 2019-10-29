#ifndef __DIRECTION_HPP__
#define __DIRECTION_HPP__

/* キャラクターの移動方向 */
/* GameKey.hppと対応させる */

/* キャラクタの座標型 */
using POS_TYPE = float;

enum class DIRECTION : unsigned char
{
	EAST  = 0x04, /* 右 */
	WEST  = 0x08, /* 左 */
	SOUTH = 0x02, /* 下 */
	NORTH = 0x01, /* 上 */
	
	SOUTH_EAST = EAST | SOUTH,  /* 右下 */
	SOUTH_WEST = WEST | SOUTH,  /* 左下 */
	NORTH_EAST = EAST | NORTH,  /* 右上 */
	NORTH_WEST = WEST | NORTH,  /* 右下 */

	NONE = 0x00 /* 移動なし */
};

#endif
