#include "./DrawGame.hpp"

DrawGame::DrawGame() :
	width(0), height(0),
	player(nullptr),
	wall(nullptr), stair(nullptr), tyle(nullptr),
	frame_counter(0), is_frame_counter(false)
{
}

DrawGame::~DrawGame()
{
	/* オブジェクトの解放 */
	if (player) delete player;
	if (wall)   delete wall;
	if (stair)  delete stair;
	if (tyle)   delete tyle;
	for (std::vector<ObjLoader*>::iterator itr = enemy.begin(); itr != enemy.end();)
	{
		if (*itr) {
			delete* itr;
			itr = enemy.erase(itr);
			continue;
		}
		++itr;
	}
	std::vector<ObjLoader*>().swap(enemy);
	std::vector<int>().swap(enemy_id_start);
}

void DrawGame::Init()
{
	/* 3Dモデルのロード */
	player = LoadObjFile(FILE_PATH::ResoucePath::player_model_path); /* Player */
	wall   = LoadObjFile(FILE_PATH::ResoucePath::wall_model_path);   /* Wall */
	stair  = LoadObjFile(FILE_PATH::ResoucePath::stair_model_path);  /* Stair */
	tyle   = LoadObjFile(FILE_PATH::ResoucePath::tyle_model_path);   /* Tyle */
	/* 敵3Dモデルのロード */
	for (int i = 0; i < FILE_PATH::ResoucePath::ENEMY_MODEL_NUM; i++)
	{
		enemy.push_back(LoadObjFile(FILE_PATH::ResoucePath::enemy_model_path_array[i]));
		enemy_id_start.push_back(tyle_id_start + obj_info * (i + 1));
	}

	/* Shader初期化 */
	shader_manager.Set(FILE_PATH::ResoucePath::obj_shader_path+FILE_PATH::ResoucePath::vert_shader_suffix, FILE_PATH::ResoucePath::obj_shader_path + FILE_PATH::ResoucePath::frag_shader_suffix);

	/* VBO初期化 */
	vbo_manager.Init(obj_info*model_number); /* VBOを使用する数で初期化 */

	/* TextureをVBOに登録 */
	player->SetTexId(vbo_manager.InitTex()); /* Playerのテクスチャ登録 */
	wall->SetTexId(vbo_manager.InitTex());   /* Wallのテクスチャ登録 */
	stair->SetTexId(vbo_manager.InitTex());  /* Stairのテクスチャ登録 */
	tyle->SetTexId(vbo_manager.InitTex());   /* Tyleのテクスチャ登録 */
	for (std::vector<ObjLoader*>::iterator itr = enemy.begin(); itr != enemy.end(); ++itr) { (*itr)->SetTexId(vbo_manager.InitTex()); }

	/* モデル情報をVBOに登録 */
	SetVBOInfo(player, player_id_start); /* VBOにplayerを登録 */
	SetVBOInfo(wall,   wall_id_start);   /* VBOにwallを登録 */
	SetVBOInfo(stair,  stair_id_start);  /* VBOにstairを登録 */
	SetVBOInfo(tyle,   tyle_id_start);   /* VBOにtyleを登録 */
	int index_id = 0;
	for (std::vector<ObjLoader*>::iterator itr = enemy.begin(); itr != enemy.end(); ++itr) { SetVBOInfo((*itr),enemy_id_start[index_id++]); }

	/* シェーダ処理の初期化 */
	shader_manager.UseProgram();
	shader_manager.UnUseProgram();

	/* テーブル登録 */
	InitMiniMapColor();
	InitCharacterObj();
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
		for (int j = sx; j < range_w; j++)
		{
			if (manage_draw_obj.find(static_cast<MAPSET::DATA>(dungeon[i * width + j])) != manage_draw_obj.end())
				DrawObj(manage_draw_obj[static_cast<MAPSET::DATA>(dungeon[i * width + j])], static_cast<float>(j), static_cast<float>(i), 0.0f);
		}
}
/* 座標版 (指定版) */
void DrawGame::DrawMap(const unsigned char* dungeon, const int& width, const int& height, const int& px, const int& py,const int &range)
{
	int sx = ((px - range) < 0) ? 0 : px - range;
	int sy = ((py - range) < 0) ? 0 : py - range;
	int range_w = ((sx + range * 2) < width ? (sx + range * 2) : width);
	int range_h = ((sy + range * 2) < height ? (sy + range * 2) : height);

	for (int i = sy; i < range_h; i++)
		for (int j = sx; j < range_w; j++)
		{
			if (manage_draw_obj.find(static_cast<MAPSET::DATA>(dungeon[i * width + j])) != manage_draw_obj.end())
				DrawObj(manage_draw_obj[static_cast<MAPSET::DATA>(dungeon[i * width + j])], static_cast<float>(j), static_cast<float>(i), 0.0f);
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
	if (manage_draw_obj.find(ch_data->GetCharaInfo()) != manage_draw_obj.end())
	{
		/* 敵以外の3Dモデル */
		DrawObj(manage_draw_obj[ch_data->GetCharaInfo()], ch_data->GetPosPX(), ch_data->GetPosPY(), ch_data->GetAngle());
	}
	else if(manage_draw_obj.find(ch_data->GetEnemyID()) != manage_draw_obj.end())
	{
		/* 敵の3Dモデル */
		/* HACK: 識別IDを使用しているため,別処理 */
		DrawObj(manage_draw_obj[ch_data->GetEnemyID()], ch_data->GetPosPX(), ch_data->GetPosPY(), ch_data->GetAngle());
	}
}
/* クリッピング */
void DrawGame::DrawCharacter(Character* ch_data, const int& width, const int& height, const int& px, const int& py)
{
	int sx = ((px - range_x) < 0) ? 0 : px - range_x;
	int sy = ((py - range_y) < 0) ? 0 : py - range_y;
	int range_w = ((sx + range_x * 2) < width ? (sx + range_x * 2) : width);
	int range_h = ((sy + range_y * 2) < height ? (sy + range_y * 2) : height);
	
	if(ch_data->GetPosX() >= sx && ch_data->GetPosX() <= range_w)
		if(ch_data->GetPosY() >= sy && ch_data->GetPosY() <= range_h) DrawCharacter(ch_data);
}
/* ステータス描画 */
void DrawGame::DrawStatusBar(Character* ch_data, const int& floor)
{
	glPushMatrix();
	DrawMode2D();
	DrawRect(up_x, up_y);
	print_manager.DrawStrings(GetStringFL(floor), st_up_x + wide_length * 0, st_up_y, 0, PS::COLOR::WHITE);
	print_manager.DrawStrings(GetStringLV(ch_data->GetLevel()), st_up_x + wide_length * 1, st_up_y, 0, PS::COLOR::WHITE);
	print_manager.DrawStrings(GetStringHP(ch_data->GetHP(), ch_data->GetMaxHP()), st_up_x + wide_length * 2, st_up_y, 0, PS::COLOR::WHITE);
	DrawMode3D();
	glPopMatrix();
}
/* ミニマップ表示 */
void DrawGame::DrawMiniMap(const MAPSET::DATA *dungeon)
{
	DrawMode2D();
	glPointSize(7.0f);
	glBegin(GL_POINTS);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++) { DrawMiniMap2D(dungeon[i*width+j],i,j); }
	}
	glEnd();
	DrawMode3D();
}
/* 暗転描画 */
bool DrawGame::DrawBlackScreen(const int &floor_num, const int& frame_time)
{
	/* フレーム処理されていない場合, 初期化 */
	if (!is_frame_counter)
	{
		frame_counter = 0;
		is_frame_counter = true;
	}

	/* 遷移画面表示 */
	if (is_frame_counter)
	{
		glPushMatrix();
		{
			DrawMode2D();

			print_manager.DrawStrings(std::string("FLOOR:"), -0.4f, 0.0f, 0.0f, PS::COLOR::RED);
			print_manager.DrawStrings(std::to_string(floor_num), 0.0f, 0.0f, 0.0f, PS::COLOR::WHITE);

			DrawMode3D();
		}
		glPopMatrix();

		frame_counter++;
	}

	/* 規定フレーム数に達した場合, 処理を終了 */
	if (frame_counter >= frame_time)
	{
		frame_counter = 0;
		is_frame_counter = false;
		return true;
	}
	return false;
}
/* private */
/* 初期化関係 */
ObjLoader* DrawGame::LoadObjFile(const std::string &obj_path)
{
	ObjLoader* obj_file = new ObjLoader;
	obj_file->Init(obj_path + FILE_PATH::ResoucePath::obj_suffix);
	obj_file->LoadTextureImage(obj_path + FILE_PATH::ResoucePath::bmp_suffix);
	return obj_file;
}
void DrawGame::SetVBOInfo(ObjLoader* obj_data,const int id_start)
{
	obj_data->SetVBOId(id_start, id_start + obj_info); /* vertex, texture, uv */
	vbo_manager.SetBuffer(obj_data->GetVertexSize(), obj_data->GetVertexPointer(), obj_data->GetVertexID()); /* 頂点 */
	vbo_manager.SetBuffer(obj_data->GetNormalSize(), obj_data->GetNormalPointer(), obj_data->GetNormalID()); /* 法線 */
	vbo_manager.SetBuffer(obj_data->GetUVSize(), obj_data->GetUVPointer(), obj_data->GetUVID()); /* UV */
	vbo_manager.SetTexBuffer(static_cast<GLint>(obj_data->GetTexID()), obj_data->GetTexWidth(), obj_data->GetTexHeight(), obj_data->GetTexBuf()); /* Texture */
}
/* 描画関係 */
void DrawGame::DrawMode2D()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, -1.0); /* -1.0～1.0の空間に設定 */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}
void DrawGame::DrawMode3D()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, static_cast<double>(width / height), 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}
void DrawGame::DrawMiniMap2D(const MAPSET::DATA& data,const int &i, const int &j)
{
	if (manage_mini_map_color.find(data) != manage_mini_map_color.end())
	{
		glColor4fv(manage_mini_map_color[data]);
		glVertex3f(j * (1.0f / width), (height - i) * (1.0f / height), 0.0f);
	}
}
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
	vbo_manager.BindTexture(static_cast<GLuint>(obj_data->GetTexID()));

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

	vbo_manager.DisableBindBuffer(obj_data->GetUVID());
	vbo_manager.DisableBindBuffer(obj_data->GetVertexID());

	delete[] value;
}
/* ミニマップのカラーテーブル初期化 */
void DrawGame::InitMiniMapColor()
{
	manage_mini_map_color.emplace(MAPSET::DATA::WALL,   MiniMapColor::CL_TABLE[static_cast<int>(MiniMapColor::COLOR::RED)]);
	manage_mini_map_color.emplace(MAPSET::DATA::ROAD,   MiniMapColor::CL_TABLE[static_cast<int>(MiniMapColor::COLOR::GREEN)]);
	manage_mini_map_color.emplace(MAPSET::DATA::ROOM,   MiniMapColor::CL_TABLE[static_cast<int>(MiniMapColor::COLOR::BLUE)]);
	manage_mini_map_color.emplace(MAPSET::DATA::PLAYER, MiniMapColor::CL_TABLE[static_cast<int>(MiniMapColor::COLOR::ORANGE)]);
	manage_mini_map_color.emplace(MAPSET::DATA::ENEMY,  MiniMapColor::CL_TABLE[static_cast<int>(MiniMapColor::COLOR::PURPLE)]);
	manage_mini_map_color.emplace(MAPSET::DATA::STAIR,  MiniMapColor::CL_TABLE[static_cast<int>(MiniMapColor::COLOR::YELLOW)]);
}
/* 3Dモデルのテーブル初期化 */
void DrawGame::InitCharacterObj()
{
	manage_draw_obj.emplace(MAPSET::DATA::WALL,   wall);
	manage_draw_obj.emplace(MAPSET::DATA::ROOM,   tyle);
	manage_draw_obj.emplace(MAPSET::DATA::ROAD,   tyle);
	manage_draw_obj.emplace(MAPSET::DATA::STAIR,  stair);
	manage_draw_obj.emplace(MAPSET::DATA::PLAYER, player);
	MAPSET::DATA index = MAPSET::DATA::ENEMY1;
	for (auto itr = enemy.begin(); itr != enemy.end(); ++itr)
	{
		manage_draw_obj.emplace(index, (*itr));
		index = static_cast<MAPSET::DATA>(static_cast<int>(index) + 1);
	}
}