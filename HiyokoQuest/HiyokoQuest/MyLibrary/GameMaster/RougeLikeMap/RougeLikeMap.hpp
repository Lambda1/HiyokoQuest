#ifndef __ROUGE_LIKE_HPP__
#define __ROUGE_LIKE_HPP__

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <random>

#include "../../Mathematics/MyMath/MyMath.hpp"
#include "../../Mathematics/MyVector/MyVector.hpp"

template <class TYPE_MAP>
class RougeLikeMap
{
	struct room_coord{ my_math::Vec<int> left_up, right_down; }; /* 部屋座標 */

	enum class MAP_LAYER : TYPE_MAP { FIELD, ITEM, CHARA, ALL, SIZE };
	enum class BASE_INFO : TYPE_MAP { NONE, ROOM, ROAD, WALL, SIZE }; /* 基本マップ情報 */
	static constexpr double size_max_scale = 0.2, size_min_scale = 0.1; /*各部屋の大きさ倍率 */
	static constexpr int neighbor_offset = 4; /* 隣の部屋とのオフセット */
	static constexpr int width_offset = 1, height_offset = 1; /* 領域内オフセット */
	static constexpr int try_generate_room = 5000; /* 部屋生成試行回数 */

	TYPE_MAP *dungeon[static_cast<int>(MAP_LAYER::SIZE)]; /* ダンジョン */
	std::vector<TYPE_MAP> info; /* 部屋情報 */
	int width, height; /* サイズ */
	int room_num;      /* 部屋数 */
	int room_index;    /* 実際に生成できた部屋数 */
	std::vector<room_coord> room_pnt; /* 部屋座標 */
	static constexpr int checkpoint_offset = 1; /* 分岐点オフセット */

	enum class ANGLE : unsigned char { RIGHT, UP, LEFT, DOWN, SIZE }; /* 掘る方向 */
	static constexpr int del_ang = 30; /* 方向分解能(角度) */

	std::mt19937 mt_rnd; /* 乱数 */

	/* 部屋が密集していないか判定 */
	bool JudgeIsRoom(my_math::Vec<int> *lu_point,my_math::Vec<int> *rd_point)
	{
		auto judge = [this](my_math::Vec<int> *lu,my_math::Vec<int> *rd){
			for(int i = lu->y;i <= rd->y;i++)
				for(int j = lu->x;j <= rd->x;j++)
					if(dungeon[static_cast<int>(MAP_LAYER::FIELD)][i*width+j] != static_cast<int>(BASE_INFO::NONE)) return true;
			return false;
		};

		my_math::Vec<int> pnt1,pnt2;
		/* 走査: 上 */
		pnt1.Set(lu_point->x, ((lu_point->y-neighbor_offset > 0) ? (lu_point->y-neighbor_offset) : 0));
		pnt2.Set(rd_point->x, lu_point->y);
		if(judge(&pnt1, &pnt2)) return true;
		/* 走査: 下 */
		pnt1.Set(lu_point->x, rd_point->y);
		pnt2.Set(rd_point->x, ((rd_point->y+neighbor_offset < height) ? (rd_point->y+neighbor_offset) : height-1));
		if(judge(&pnt1, &pnt2)) return true;
		/* 走査: 左 */
		pnt1.Set(((lu_point->x-neighbor_offset > 0) ? (lu_point->x-neighbor_offset) : 0), lu_point->y);
		pnt2.Set(lu_point->x, rd_point->y);
		if(judge(&pnt1, &pnt2)) return true;
		/* 走査: 右 */
		pnt1.Set(rd_point->x, lu_point->y);
		pnt2.Set(((rd_point->x+neighbor_offset < width) ? (rd_point->x+neighbor_offset) : width-1), rd_point->y);
		if(judge(&pnt1, &pnt2)) return true;

		return false;
	}
	/* 生成する場所に部屋が存在するか */
	bool JudgeRoomExist(my_math::Vec<int> *lu_point,my_math::Vec<int> *rd_point)
	{
		for(int i = lu_point->y;i < rd_point->y;i++){
			for(int j = lu_point->x;j < rd_point->x;j++){
				if(dungeon[static_cast<int>(MAP_LAYER::FIELD)][i*width+j] != info[static_cast<int>(BASE_INFO::NONE)]) return true;
			}
		}
		return false;
	}
	/* 左上, 右下の部屋座標をランダムに生成 */
	bool RandomRoomVec(my_math::Vec<int> *lu_point,my_math::Vec<int> *rd_point)
	{
		my_math::Vec<int> l_pnt,r_pnt;
		std::uniform_int_distribution<int> x_rand(width_offset, width-width_offset-1), y_rand(height_offset, height-height_offset-1);

		auto swap = [](int &a,int &b){ a^=b; b^=a; a^=b; };
		l_pnt.Set(x_rand(mt_rnd),y_rand(mt_rnd)), r_pnt.Set(x_rand(mt_rnd),y_rand(mt_rnd));
		if(l_pnt.x > r_pnt.x) swap(l_pnt.x, r_pnt.x);
		if(l_pnt.y > r_pnt.y) swap(l_pnt.y, r_pnt.y);
		*lu_point = l_pnt, *rd_point = r_pnt;

		if((r_pnt.x-l_pnt.x) > static_cast<int>(width*size_max_scale))  return false;
		if((r_pnt.y-l_pnt.y) > static_cast<int>(height*size_max_scale)) return false;
		if((r_pnt.x-l_pnt.x) < static_cast<int>(width*size_min_scale))  return false;
		if((r_pnt.y-l_pnt.y) < static_cast<int>(height*size_min_scale)) return false;

		return true;
	}
	/* left-up, right-downの2点間から部屋を作成 */
	void CreateRoom(const my_math::Vec<int> *lu_point, const my_math::Vec<int> *rd_point)
	{
		for(int i = lu_point->x;i <= rd_point->x;i++) dungeon[static_cast<int>(MAP_LAYER::FIELD)][(lu_point->y*width)+i] = dungeon[static_cast<int>(MAP_LAYER::FIELD)][(rd_point->y*width)+i] = info[static_cast<int>(BASE_INFO::WALL)];
		for(int i = lu_point->y;i <= rd_point->y;i++) dungeon[static_cast<int>(MAP_LAYER::FIELD)][(i*width)+lu_point->x] = dungeon[static_cast<int>(MAP_LAYER::FIELD)][(i*width)+rd_point->x] = info[static_cast<int>(BASE_INFO::WALL)];

		for(int i = lu_point->y+1;i < rd_point->y;i++)
			for(int j = lu_point->x+1;j < rd_point->x;j++) dungeon[static_cast<int>(MAP_LAYER::FIELD)][(i*width)+j] = info[static_cast<int>(BASE_INFO::ROOM)];
	}
	/* 部屋座標登録 */
	inline void SetRoom(const my_math::Vec<int> *pnt1, const my_math::Vec<int> *pnt2)
	{
		room_coord data = {*pnt1, *pnt2};
		room_pnt.push_back(data);
	}
	/* 初期部屋生成 */
	bool GenerateRoom()
	{
		my_math::Vec<int> pnt1,pnt2;
		for(int i = 0;i < try_generate_room;i++)
			if(RandomRoomVec(&pnt1,&pnt2)){
				if(!JudgeIsRoom(&pnt1,&pnt2) && !JudgeRoomExist(&pnt1,&pnt2)){
					SetRoom(&pnt1,&pnt2);
					CreateRoom(&pnt1,&pnt2);
					room_index++;
					return true;
				}
			}
		return false;
	}
	/* 右へ掘る */
	void DigRight(room_coord *pnt1, room_coord *pnt2)
	{
		int cp_off = ((pnt1->right_down.x+checkpoint_offset - pnt2->left_up.x-checkpoint_offset)<=0) ? 0 : checkpoint_offset;
		std::uniform_int_distribution<int> start(pnt1->left_up.y+1,pnt1->right_down.y-1), goal(pnt2->left_up.y+1,pnt2->right_down.y-1);
		std::uniform_int_distribution<int> check_pnt(pnt1->right_down.x+cp_off, pnt2->left_up.x-cp_off);
		int sx = pnt1->right_down.x, ex = pnt2->left_up.x;
		int sy = start(mt_rnd), ey = goal(mt_rnd), branch = check_pnt(mt_rnd);

		for(int i = sx; i <= branch; i++) GenericSetDungeon(sy,i,info[static_cast<int>(BASE_INFO::ROAD)]);

		if(sy < ey)
			for(int i = sy; i <= ey; i++) GenericSetDungeon(i,branch,info[static_cast<int>(BASE_INFO::ROAD)]);
		else
			for(int i = sy; i >= ey; i--) GenericSetDungeon(i,branch,info[static_cast<int>(BASE_INFO::ROAD)]);

		for(int i = branch; i <= ex; i++) GenericSetDungeon(ey,i,info[static_cast<int>(BASE_INFO::ROAD)]);
	}
	/* 下へ掘る */
	void DigDown(room_coord *pnt1, room_coord *pnt2)
	{
		/* 横に掘った方が効率が良い時 */
		if(pnt1->right_down.y > pnt2->left_up.y){ ((pnt1->left_up.x > pnt2->left_up.x) ? DigRight(pnt2,pnt1) : DigRight(pnt1,pnt2)); return; }

		int cp_off = ((pnt1->right_down.y+checkpoint_offset - pnt2->left_up.y-checkpoint_offset)<=0) ? 0 : checkpoint_offset;
		std::uniform_int_distribution<int> start(pnt1->left_up.x+1,pnt1->right_down.x-1), goal(pnt2->left_up.x+1,pnt2->right_down.x-1);
		std::uniform_int_distribution<int> check_pnt(pnt1->right_down.y+cp_off, pnt2->left_up.y-cp_off);

		int sy = pnt1->right_down.y, ey = pnt2->left_up.y;
		int sx = start(mt_rnd), ex = goal(mt_rnd), branch = check_pnt(mt_rnd);

		for(int i = sy; i <= branch; i++) GenericSetDungeon(i,sx,info[static_cast<int>(BASE_INFO::ROAD)]);

		if(sx < ex)
			for(int i = sx; i <= ex; i++) GenericSetDungeon(branch,i,info[static_cast<int>(BASE_INFO::ROAD)]);
		else
			for(int i = sx; i >= ex; i--) GenericSetDungeon(branch,i,info[static_cast<int>(BASE_INFO::ROAD)]);

		for(int i = branch; i <= ey; i++) GenericSetDungeon(i,ex,info[static_cast<int>(BASE_INFO::ROAD)]);
	}
	/* 部屋連結 */
	void ConnectRoom(room_coord *pnt1,room_coord *pnt2)
	{
		auto map_ang = [](int ang)
		{
			ang = (ang+360)%360;
			if((360-del_ang) < ang || ang < del_ang)   return ANGLE::RIGHT;
			if(ang < (180-del_ang)) return ANGLE::UP;
			if(ang < (180+del_ang))  return ANGLE::LEFT;
			return ANGLE::DOWN;
		};
		double angle = (180*my_math::Math::Angle(static_cast<double>(pnt1->left_up.x), static_cast<double>(height-pnt1->left_up.y), static_cast<double>(pnt2->left_up.x), static_cast<double>(height-pnt2->left_up.y)))/my_math::Math::PI;

		switch(auto vec = map_ang(static_cast<int>(angle)); vec){
			case ANGLE::RIGHT:
				DigRight(&(*pnt1), &(*pnt2)); break;
			case ANGLE::UP:
				/* 注目する部屋を上下入れ替えて, 下へ掘る. */
				DigDown(&(*pnt2), &(*pnt1)); break;
			case ANGLE::LEFT:
				/* アルゴリズム上, 左に生やすことはない */ break;
			case ANGLE::DOWN:
				DigDown(&(*pnt1), &(*pnt2)); break;
			default: break;
		}
	}
	/* 連結しやすいように, 部屋の並び替え */
	void SortRoom()
	{
		auto cmp = [](const room_coord &pnt1, const room_coord &pnt2){ return pnt1.left_up.x < pnt2.left_up.x; };
		std::sort(room_pnt.begin(), room_pnt.end(), cmp);
	}
	/* 通路生成 */
	void MakeAisle()
	{
		SortRoom();
		for(int i = 0;i < room_index-1;i++){
			double min = my_math::Math::Distance<double>(room_pnt[i].left_up.x,room_pnt[i].left_up.y,room_pnt[i+1].left_up.x,room_pnt[i+1].left_up.y);
			int min_index = i+1;
			for(int j = i+2;j < room_index;j++){
				double tmp = my_math::Math::Distance<double>(room_pnt[i].left_up.x,room_pnt[i].left_up.y,room_pnt[j].left_up.x,room_pnt[j].left_up.y);
				if(min > tmp){
					min = tmp;
					min_index = j;
				}
			}
			ConnectRoom(&room_pnt[i],&room_pnt[min_index]);
		}
	}
	/* 壁を生成 */
	void MakeWall()
	{
		auto make_wall = [this](int i,int j)
		{
			for(int y = i-1;y <= i+1;y+=2)
				for(int x = j-1;x <= j+1;x+=2)
					if(x > -1 && x < width && y > -1 && y < height)
						if(dungeon[static_cast<int>(MAP_LAYER::FIELD)][y*width+x] == info[static_cast<int>(BASE_INFO::NONE)])
							dungeon[static_cast<int>(MAP_LAYER::FIELD)][y*width+x] = info[static_cast<int>(BASE_INFO::WALL)];
		};
		for(int i = 0;i < height;i++)
			for(int j = 0;j < width;j++) if(dungeon[static_cast<int>(MAP_LAYER::FIELD)][i*width+j] == info[static_cast<int>(BASE_INFO::ROAD)]) make_wall(i,j);
	}
	/* 何も設定されていない場合, マップに登録 */
	inline void GenericSetDungeon(const int i,const int j,const TYPE_MAP data)
	{
		if(dungeon[static_cast<int>(MAP_LAYER::FIELD)][i*width+j] != info[static_cast<int>(BASE_INFO::ROOM)]) dungeon[static_cast<int>(MAP_LAYER::FIELD)][i*width+j] = data;
	}

	public:
	RougeLikeMap() :
		dungeon{nullptr}, width(0), height(0), room_num(0),
		room_index(0),
		mt_rnd(0)
	{}
	RougeLikeMap(const int rnd_seed) :
		dungeon{nullptr}, width(0), height(0), room_num(0),
		room_index(0),
		mt_rnd(rnd_seed)
	{}
	~RougeLikeMap()
	{
		if(*dungeon){
			for(int i = 0;i < static_cast<int>(MAP_LAYER::SIZE);i++) delete[] dungeon[i];
		}
		info.clear();
		room_pnt.clear();
	}
	/* コピー禁止 */
	RougeLikeMap(const RougeLikeMap &obj) = delete;
	RougeLikeMap operator=(const RougeLikeMap&) = delete;

	/* 初期化 */
	void Init(const int width,const int height,const int room_num)
	{
		this->width  = width;
		this->height = height;
		this->room_num = room_num;
		for(int i = 0;i < static_cast<int>(MAP_LAYER::SIZE);i++) dungeon[i] = new TYPE_MAP[width*height];
	}

	/* ダンジョン生成: 穴掘り法 */
	/* シンプルな3要素でダンジョンを生成する. */
	void Generate()
	{
		for(int i = 0;i < width*height;i++) dungeon[static_cast<int>(MAP_LAYER::FIELD)][i] = info[static_cast<int>(BASE_INFO::NONE)];
		for(int i = 0;i < room_num;i++){ GenerateRoom(); }
		MakeAisle();
		MakeWall();
		for(int i = static_cast<int>(MAP_LAYER::ITEM);i < static_cast<int>(MAP_LAYER::SIZE);i++)
			for(int j = 0;j < width*height;j++) dungeon[i][j] = info[static_cast<int>(BASE_INFO::NONE)];
	}

	/* 総合レイヤを更新する */
	void Update()
	{
		/* 上層レイヤーから重ね合わせ */
		int layer_all = static_cast<int>(MAP_LAYER::ALL);
			for(int i = 0;i < layer_all;i++){
				for(int j = 0;j < width*height;j++){
					if(dungeon[i][j] != info[static_cast<int>(BASE_INFO::NONE)]) dungeon[layer_all][j] = dungeon[i][j];
				}
			}
	}

	/* ゲッタ */
	inline TYPE_MAP* GetDungeon() const { return dungeon[static_cast<int>(MAP_LAYER::FIELD)]; }
	inline TYPE_MAP* GetItem()    const { return dungeon[static_cast<int>(MAP_LAYER::ITEM)]; }
	inline TYPE_MAP* GetChara()   const { return dungeon[static_cast<int>(MAP_LAYER::CHARA)]; }
	inline TYPE_MAP* GetALL()     const { return dungeon[static_cast<int>(MAP_LAYER::ALL)]; }
	/* セッタ */
	inline void SetBaseInfo(const TYPE_MAP none,const TYPE_MAP room,const TYPE_MAP road,const TYPE_MAP wall){ info.push_back(none); info.push_back(room); info.push_back(road); info.push_back(wall); }
	inline void SetItem(const int i, const int j, const TYPE_MAP data){ dungeon[static_cast<int>(MAP_LAYER::ITEM)][i*width+j] = data; }
	inline void SetChara(const int i, const int j, const TYPE_MAP data){ dungeon[static_cast<int>(MAP_LAYER::CHARA)][i*width+j] = data; }
};

#endif
