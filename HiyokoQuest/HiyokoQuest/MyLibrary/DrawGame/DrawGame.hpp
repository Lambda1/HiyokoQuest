#ifndef __DRAW_GAME_HPP__
#define __DRAW_GAME_HPP__

#include "..//VBOLoader/VBOLoader.hpp"
#include "..//MyShader/MyShader.hpp"

#include "..//Graphics/ResourcePath.hpp"

#include "..//Graphics/ObjLoader/ObjLoader.hpp"

#include "..//GameMaster/RougeLikeMap/MapSet.hpp"
#include "../GameMaster/Character/Character.hpp"

#include "../PrintString/PrintString.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <string>

class DrawGame
{
	enum class ENEMY_INFO : int
	{
		ENEMY,
	};

	/* .obj情報 */
	const int obj_info     = 3; /* 頂点・法線・UV座標 */
	const int model_number = 5; /* 使用する3Dモデル数 */
	/* 描画クリッピング範囲 */
	const int range_x = 7, range_y = 9;

	/* 画面サイズ */
	int width, height;
	
	/* ステータスバー位置 */
	const float up_x = -0.9f, up_y = 0.80f;
	const float frame_size_w = 0.6f, frame_size_h = 0.12f;
	const float wide_length = 0.15f;
	/* ステータス文字相対位置 */
	const float st_up_x = up_x + 0.01f, st_up_y = up_y + 0.03f;

	/* 3Dモデル 関係 */
	/* 主人公 */
	ObjLoader* player;
	const int player_id_start = 0;
	/* 壁 */
	ObjLoader* wall;
	const int wall_id_start  = player_id_start + obj_info;
	/* 階段 */
	ObjLoader* stair;
	const int stair_id_start = wall_id_start + obj_info;
	/* 床 */
	ObjLoader* tyle;
	const int tyle_id_start  = stair_id_start + obj_info;
	/* 敵 */
	std::vector<ObjLoader*> enemy;
	std::vector<int> enemy_id_start;

	/* Shader関係 */
	MyShader shader_manager;
	inline static const std::string obj_shader_var[] = {"pos", "uv", "texture"}; /* 今回は法線情報は使わない */

	/* VBO関係 */
	VBOLoader vbo_manager;

	/* 文字表示関連 */
	PrintString print_manager;

	/* フレーム処理関連 */
	int  frame_counter;    /* フレームカウンタ */
	bool is_frame_counter; /* フレーム処理判定 */

private:
	/* 初期化関係 */
	/* .objロード */
	ObjLoader* LoadObjFile(const std::string &obj_path);
	/* VBO初期化 */
	void SetVBOInfo(ObjLoader *obj_data, const int id_start); 	/* VBOに.objファイルを追加 */

	/* 描画関係 */
	void DrawMode2D(); /* 並行投影 */
	void DrawMode3D(); /* 透視投影 */
	/* ステータス表示 */
	inline void DrawRect(const float& x, const float& y)
	{
		glBegin(GL_QUADS);
		{
			glColor3f(0.4f, 0.4f, 0.4f);
			glVertex3f(x, y, 0.1f);
			glVertex3f(x + frame_size_w, y, 0.1f);
			glVertex3f(x + frame_size_w, y + frame_size_h, 0.1f);
			glVertex3f(x, y + frame_size_h, 0.1f);
		}
		glEnd();
	}
	inline std::string GetStringLV(const int& level)
	{
		return (std::string("Lv: ") + std::to_string(level));
	}
	inline std::string GetStringHP(const int& hp, const int& max_hp)
	{
		return (std::string("HP: ") + std::to_string(hp) + std::string(" / ") + std::to_string(max_hp));
	}
	inline std::string GetStringFL(const int& floor)
	{
		return (std::to_string(floor) + std::string(" F"));
	}
	/* .obj表示 */
	void DrawObj(ObjLoader *obj_data, const float &x, const float &z, const float &ang); /* VBO+Shaderで.objを描画 */

public:
		DrawGame();
		~DrawGame();

		void Init(); /* 初期化処理 */

		/* カメラ処理 */
		template<class T>
		void CameraPos(const T& pos_x, const T& pos_y, const T& pos_z, const T& gaze_x, const T& gaze_y, const T& gaze_z)
		{
			glLoadIdentity();
			gluLookAt(pos_x, pos_y, pos_z, gaze_x, gaze_y, gaze_z, 0.0, 1.0, 0.0);
		}
		/* 描画処理 */
		/* 画面クリア */
		inline void DrawInit()
		{
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		}
		/* レイヤ層によるマップ表示 */
		void DrawMap(const unsigned char *dungeon,const int &width,const int &height); /* マップ層表示 */
		/* 座標におけるマップ表示 (クリッピングに使用) */
		void DrawMap(const unsigned char* dungeon, const int& width, const int& height, const int& px, const int& py);
		/* レイヤ層によるキャラクタ表示 */
		void DrawCharacter(const unsigned char* dungeon, const int& width, const int& height); /* キャラクター表示 */
		/* 座標におけるキャラクタ表示 */
		void DrawCharacter(Character* ch_data);
		/* 座標におけるキャラクタ表示 (クリップングに使用) */
		void DrawCharacter(Character* ch_data, const int& width, const int& height,const int& px,const int& py);

		/* ステータス表示 */
		void DrawStatusBar(Character* ch_data, const int& floor);

		/* 階層表示 */
		bool DrawBlackScreen(const int &floor_num, const int &frame_time);

		/* セッタ */
		inline void SetSize(const int& width, const int& height) { this->width = width, this->height = height; }
};

#endif