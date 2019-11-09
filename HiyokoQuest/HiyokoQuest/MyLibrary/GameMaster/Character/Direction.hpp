#ifndef __DIRECTION_HPP__
#define __DIRECTION_HPP__

#include <map>

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

/* テーブル用データ構造 */
namespace DIRECT_TABLE
{
	class DIRECTION_TABLE
	{
	private:
		DIRECTION_TABLE() :
			m_ang1(0), m_ang2(0), m_direct(DIRECTION::NONE)
		{};
	public:
		POS_TYPE m_ang1, m_ang2;
		DIRECTION m_direct;

		DIRECTION_TABLE(const POS_TYPE &ang1,const POS_TYPE &ang2,const DIRECTION &direct) :
			m_ang1(ang1), m_ang2(ang2), m_direct(direct)
		{};
		~DIRECTION_TABLE() {};
	};

	/* 角度分解能 */
	inline static constexpr POS_TYPE ANG_DEG   = 360.0f / 16.0f; /* 16分割 */
	inline static constexpr POS_TYPE ANG_DEG_8 = 360.0f /  8.0f; /* 8分割 */

	/* 角度テーブル */
	inline static const DIRECTION_TABLE manager_direct[] =
	{
		/* 実角度 => ゲーム角度 */
		/* [-22.5, 0.0) => [0.0, 22.5) */
		{ ANG_DEG * 0.0f,  ANG_DEG * 1.0f, DIRECTION::EAST},
		/* [0.0, 22.5)  => [-22.5, 0.0) */
		{-ANG_DEG * 1.0f,  ANG_DEG * 0.0f, DIRECTION::EAST},
		
		/* [22.5, 67.5) => [-67.5, -22.5) */
		{-ANG_DEG * 3.0f, -ANG_DEG * 1.0f, DIRECTION::NORTH_EAST},
		
		/* [67.5, 112.5) => [-112.5, -67.5) */
		{-ANG_DEG * 5.0f, -ANG_DEG * 3.0f, DIRECTION::NORTH},
		
		/* [112.5, 157.5) => [-157.5, -112.5) */
		{-ANG_DEG * 7.0f, -ANG_DEG * 5.0f, DIRECTION::NORTH_WEST},
		
		/* [157.5, 180) => [-180.0, -157.5) */
		{-ANG_DEG * 8.0f, -ANG_DEG * 7.0f, DIRECTION::WEST},
		/* [-180.0, -157.5) => [157.5, 180.0) */
		{ ANG_DEG * 7.0f,  ANG_DEG * 8.0f, DIRECTION::WEST},
		/* [-180.0, 180.0) => [180.0, 180.0) */
		{ ANG_DEG * 8.0f,  ANG_DEG * 8.001f, DIRECTION::WEST},

		/* [-157.5, -112.5) => [112.5, 157.5) */
		{ ANG_DEG * 5.0f,  ANG_DEG * 7.0f, DIRECTION::SOUTH_WEST},

		/* [-112.5, -67.5) => [67.5, 112.5) */
		{ ANG_DEG * 3.0f,  ANG_DEG * 5.0f, DIRECTION::SOUTH},

		/* [-67.5, -22.5) => [22.5, 67.5) */
		{ ANG_DEG * 1.0f,  ANG_DEG * 3.0f, DIRECTION::SOUTH_EAST}
	};

	/* テーブルサイズ */
	inline static constexpr int table_size = sizeof(manager_direct) / sizeof(manager_direct[0]);
};
#endif