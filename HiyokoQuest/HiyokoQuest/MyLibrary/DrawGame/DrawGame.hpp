#ifndef __DRAW_GAME_HPP__
#define __DRAW_GAME_HPP__

#include "..//VBOLoader/VBOLoader.hpp"
#include "..//MyShader/MyShader.hpp"

#include "..//Graphics/ResourcePath.hpp"

#include "..//Graphics/ObjLoader/ObjLoader.hpp"

#include "..//GameMaster/RougeLikeMap/MapSet.hpp"
#include "../GameMaster/Character/Character.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <string>

class DrawGame
{
	/* .obj情報 */
	const int obj_info     = 3; /* 頂点・法線・UV座標 */
	const int model_number = 1; /* 使用する3Dモデル数 */
	/* 描画クリッピング範囲 */
	const int range_x = 7, range_y = 9;


	/* 3Dモデル 関係 */
	ObjLoader* player;
	const int player_id_start = 0;

	/* Shader関係 */
	MyShader shader_manager;
	inline static const std::string obj_shader_var[] = {"pos", "uv", "texture"}; /* 今回は法線情報は使わない */

	/* VBO関係 */
	VBOLoader vbo_manager;

private:
	/* 初期化関係 */
	/* VBO初期化 */
	void SetVBOInfo(ObjLoader *obj_data, const int id_start); 	/* VBOに.objファイルを追加 */

	/* 描画関係 */
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
		/* 座標におけるキャラクタ表示 (クリッピングに使用) */
		void DrawCharacter(Character* ch_obj);
};

#endif
