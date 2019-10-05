#ifndef __DRAW_GAME_HPP__
#define __DRAW_GAME_HPP__

#include "..//VBOLoader/VBOLoader.hpp"
#include "..//MyShader/MyShader.hpp"

#include "..//Graphics/ResourcePath.hpp"

#include "..//Graphics/ObjLoader/ObjLoader.hpp"

#include "..//GameMaster/RougeLikeMap/MapSet.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <string>

class DrawGame
{
	const int obj_info     = 3; /* 頂点・法線・UV座標 */
	const int model_number = 1; /* 使用する3Dモデル数 */

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
	void DrawObj(ObjLoader *obj_data, const float &x, const float &z); /* VBO+Shaderで.objを描画 */
public:
		DrawGame();
		~DrawGame();

		void Init(); /* 初期化処理 */

		void CameraPos(const int pos_x, const int pos_y, const int pos_z, const int gaze_x,const int gaze_y,const int gaze_z); /* カメラ位置 */

		void DrawMap(const unsigned char *dungeon,const int &width,const int &height); /* マップ層表示 */
		void DrawCharacter(const unsigned char* dungeon, const int& width, const int& height); /* キャラクター表示 */
};

#endif
