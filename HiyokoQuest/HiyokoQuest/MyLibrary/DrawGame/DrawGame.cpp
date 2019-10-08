#include "./DrawGame.hpp"

DrawGame::DrawGame() :
	player(nullptr)
{
}

DrawGame::~DrawGame()
{
	if (player) delete player;
}

void DrawGame::Init()
{
	/* Player3Dモデルのロード */
	player = new ObjLoader;
	player->Init(FILE_PATH::ResoucePath::player_model_path + FILE_PATH::ResoucePath::obj_suffix);
	player->LoadTextureImage(FILE_PATH::ResoucePath::player_model_path + FILE_PATH::ResoucePath::bmp_suffix);

	/* Shader初期化 */
	shader_manager.Set(FILE_PATH::ResoucePath::obj_shader_path+FILE_PATH::ResoucePath::vert_shader_suffix, FILE_PATH::ResoucePath::obj_shader_path + FILE_PATH::ResoucePath::frag_shader_suffix);

	/* VBO初期化 */
	vbo_manager.Init(obj_info*model_number); /* VBOを使用する数で初期化 */

	/* TextureをVBOに登録 */
	player->SetTexId(vbo_manager.InitTex()); /* Playerのテクスチャ登録 */

	/* モデル情報をVBOに登録 */
	SetVBOInfo(player, player_id_start); /* VBOにplayerを登録 */
}
/* マップ層描画 */
/* レイヤ版 */
void DrawGame::DrawMap(const unsigned char *dungeon,const int &width, const int &height)
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (dungeon[i * width + j] == static_cast<unsigned char>(MAPSET::DATA::WALL))
				DrawObj(player, static_cast<float>(j), static_cast<float>(i), 0.0f);
}
/* 座標版 */
void DrawGame::DrawMap(const unsigned char* dungeon, const int& width, const int& height, const int& px, const int& py)
{
	int sx = ((px - range_x) < 0) ? 0 : px - range_x;
	int sy = ((py - range_y) < 0) ? 0 : py - range_y;
	int range_w = ((sx + range_x * 2) < width  ? (sx + range_x * 2) : width);
	int range_h = ((sy + range_y * 2) < height ? (sy + range_y * 2) : height);

	for (int i = sy; i < range_h; i++)
		for (int j = sx; j < range_w; j++) {
			switch (dungeon[i * width + j])
			{
			case static_cast<unsigned char>(MAPSET::DATA::WALL):
				DrawObj(player, static_cast<float>(j), static_cast<float>(i), 0.0f);  break;
			case static_cast<unsigned char>(MAPSET::DATA::STAIR) :
					DrawObj(player, static_cast<float>(j), static_cast<float>(i), 0.0f);  break;
			default:
				break;
			}
		}
}
/* キャラクター表示 (y->z) */
/* レイヤ版 */
void DrawGame::DrawCharacter(const unsigned char* dungeon, const int& width, const int& height)
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (dungeon[i * width + j] == static_cast<unsigned char>(MAPSET::DATA::PLAYER))
				DrawObj(player, static_cast<float>(j), static_cast<float>(i), 0.0f);
}
/* 座標版 */
void DrawGame::DrawCharacter(Character* ch_data)
{
	switch (ch_data->GetCharaInfo())
	{
	case MAPSET::DATA::PLAYER:
		DrawObj(player, ch_data->GetPosPX(), ch_data->GetPosPY(), ch_data->GetAngle()); break;
	default:
		break;
	}
}
/* private */
/* 初期化関係 */
void DrawGame::SetVBOInfo(ObjLoader* obj_data,const int id_start)
{
	obj_data->SetVBOId(player_id_start, id_start + obj_info); /* vertex, texture, uv */
	vbo_manager.SetBuffer(obj_data->GetVertexSize(), obj_data->GetVertexPointer(), obj_data->GetVertexID()); /* 頂点 */
	vbo_manager.SetBuffer(obj_data->GetNormalSize(), obj_data->GetNormalPointer(), obj_data->GetNormalID()); /* 法線 */
	vbo_manager.SetBuffer(obj_data->GetUVSize(), obj_data->GetUVPointer(), obj_data->GetUVID()); /* UV */
	vbo_manager.SetTexBuffer(static_cast<GLint>(obj_data->GetTexID()), obj_data->GetTexWidth(), obj_data->GetTexHeight(), obj_data->GetTexBuf()); /* Texture */
}
/* 描画関係 */
void DrawGame::DrawObj(ObjLoader *obj_data, const float &x, const float &z,const float &ang)
{
	/* シェーダの設定 */
	int* value = new int[obj_info];
	/* .obj用シェーダの変数取得 */
	for (int i = 0; i < obj_info; i++)
		value[i] = shader_manager.GetAttLocationValue(obj_shader_var[i]);
	/* Attribute有効 */
	for (int i = 0; i < obj_info-1; i++)
		shader_manager.EnableVAArray(value[i]); /* Vertex, UVの2点 */
	/* Uniform設定 */
	shader_manager.Uniform1i(obj_data->GetTexID(), 0); /* Texture */

	/* VBO 設定 */
	vbo_manager.BindBufferUseShader(obj_data->GetVertexID(), value[0], 3);
	vbo_manager.BindBufferUseShader(obj_data->GetUVID(), *(value+1), 2);
	vbo_manager.BindTexture(static_cast<GLuint>(player->GetTexID()));

	/* Start */
	glPushMatrix();
	/* 平行移動 */
	glTranslatef(x, 0, z);
	/* 回転 */
	glRotatef(ang, 0, 1, 0);
	/* Shader+VBOを有効にして, 描画 */
	shader_manager.UseProgram();
	vbo_manager.EnableState(GL_VERTEX_ARRAY);
	vbo_manager.EnableState(GL_NORMAL_ARRAY);
	vbo_manager.EnableTex();

	/* 描画 */
	vbo_manager.DrawArrays(GL_TRIANGLES, obj_data->GetVertexSize() / 3); /* 使用する.objは3点で1ポリゴン形成 */
	
	/* Shader+VBOを無効にする */
	vbo_manager.UnBind(true);
	vbo_manager.DisableState(GL_VERTEX_ARRAY);
	vbo_manager.DisableState(GL_NORMAL_ARRAY);
	vbo_manager.DisableTex();
	shader_manager.UnUseProgram();
	glPopMatrix();
	/* End */

	delete[] value;
}