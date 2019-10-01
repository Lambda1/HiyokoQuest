#ifndef __DIRECTION_HPP__
#define __DIRECTION_HPP__

/* キャラクターの移動方向 */

enum class DIRECTION : int
{
	EAST,        /* 右 */
	WEST,        /* 左 */
	SOUTH,       /* 下 */
	NORTH,       /* 上 */
	SOUTH_EAST,  /* 右下 */
	SOUTH_WEST,  /* 左下 */
	NORTH_EAST,  /* 右上 */
	NORTH_WESE   /* 右下 */
};

#endif
