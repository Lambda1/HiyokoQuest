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
	/* Player3D���f���̃��[�h */
	player = new ObjLoader;
	player->Init(FILE_PATH::ResoucePath::player_model_path + FILE_PATH::ResoucePath::obj_suffix);
	player->LoadTextureImage(FILE_PATH::ResoucePath::player_model_path + FILE_PATH::ResoucePath::bmp_suffix);

	/* Shader������ */
	shader_manager.Set(FILE_PATH::ResoucePath::obj_shader_path+FILE_PATH::ResoucePath::vert_shader_suffix, FILE_PATH::ResoucePath::obj_shader_path + FILE_PATH::ResoucePath::frag_shader_suffix);

	/* VBO������ */
	vbo_manager.Init(obj_info*model_number); /* VBO���g�p���鐔�ŏ����� */

	/* Texture��VBO�ɓo�^ */
	player->SetTexId(vbo_manager.InitTex()); /* Player�̃e�N�X�`���o�^ */

	/* ���f������VBO�ɓo�^ */
	SetVBOInfo(player, player_id_start); /* VBO��player��o�^ */
}
/* �J�������� */
void DrawGame::CameraPos(const int pos_x, const int pos_y, const int pos_z, const int gaze_x, const int gaze_y, const int gaze_z)
{
	glLoadIdentity();
	gluLookAt(pos_x, pos_y, pos_z, gaze_x, gaze_y, gaze_z, 0.0, 1.0, 0.0);
}
/* �}�b�v�w�`�� */
void DrawGame::DrawMap(const unsigned char *dungeon,const int &width, const int &height)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (dungeon[i * width + j] == static_cast<unsigned char>(MAPSET::DATA::WALL))
				DrawObj(player, static_cast<float>(j), static_cast<float>(i));
}
/* �L�����N�^�[�\�� (y->z) */
void DrawGame::DrawCharacter(const unsigned char* dungeon, const int& width, const int& height)
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (dungeon[i * width + j] == static_cast<unsigned char>(MAPSET::DATA::PLAYER))
				DrawObj(player, static_cast<float>(j), static_cast<float>(i));
}
/* private */
/* �������֌W */
void DrawGame::SetVBOInfo(ObjLoader* obj_data,const int id_start)
{
	obj_data->SetVBOId(player_id_start, id_start + obj_info); /* vertex, texture, uv */
	vbo_manager.SetBuffer(obj_data->GetVertexSize(), obj_data->GetVertexPointer(), obj_data->GetVertexID()); /* ���_ */
	vbo_manager.SetBuffer(obj_data->GetNormalSize(), obj_data->GetNormalPointer(), obj_data->GetNormalID()); /* �@�� */
	vbo_manager.SetBuffer(obj_data->GetUVSize(), obj_data->GetUVPointer(), obj_data->GetUVID()); /* UV */
	vbo_manager.SetTexBuffer(static_cast<GLint>(obj_data->GetTexID()), obj_data->GetTexWidth(), obj_data->GetTexHeight(), obj_data->GetTexBuf()); /* Texture */
}

/* �`��֌W */
void DrawGame::DrawObj(ObjLoader *obj_data, const float &x, const float &z)
{
	/* �V�F�[�_�̐ݒ� */
	int* value = new int[obj_info];
	/* .obj�p�V�F�[�_�̕ϐ��擾 */
	for (int i = 0; i < obj_info; i++)
		value[i] = shader_manager.GetAttLocationValue(obj_shader_var[i]);
	/* Attribute�L�� */
	for (int i = 0; i < obj_info-1; i++)
		shader_manager.EnableVAArray(value[i]); /* Vertex, UV��2�_ */
	/* Uniform�ݒ� */
	shader_manager.Uniform1i(obj_data->GetTexID(), 0); /* Texture */

	/* VBO �ݒ� */
	vbo_manager.BindBufferUseShader(obj_data->GetVertexID(), value[0], 3);
	vbo_manager.BindBufferUseShader(obj_data->GetUVID(), value[1], 2);
	vbo_manager.BindTexture(static_cast<GLuint>(player->GetTexID()));

	/* Start */
	glPushMatrix();
	/* ���s�ړ� */
	glTranslatef(x, 0, z);
	/* Shader+VBO��L���ɂ���, �`�� */
	shader_manager.UseProgram();
	vbo_manager.EnableState(GL_VERTEX_ARRAY);
	vbo_manager.EnableState(GL_NORMAL_ARRAY);
	vbo_manager.EnableTex();

	/* �`�� */
	vbo_manager.DrawArrays(GL_TRIANGLES, obj_data->GetVertexSize() / 3); /* �g�p����.obj��3�_��1�|���S���`�� */
	
	/* Shader+VBO�𖳌��ɂ��� */
	vbo_manager.UnBind(true);
	vbo_manager.DisableState(GL_VERTEX_ARRAY);
	vbo_manager.DisableState(GL_NORMAL_ARRAY);
	vbo_manager.DisableTex();
	shader_manager.UnUseProgram();
	glPopMatrix();
	/* End */

	delete[] value;
}