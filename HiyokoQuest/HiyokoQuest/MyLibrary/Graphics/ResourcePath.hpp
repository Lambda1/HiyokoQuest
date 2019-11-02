#ifndef __RESOURCE_PATH_HPP__
#define __RESOURCE_PATH_HPP__

#include <string>

namespace FILE_PATH
{
	class ResoucePath
	{
	public:
		ResoucePath() = delete;
		~ResoucePath(){}

		inline static const std::string bmp_suffix = ".bmp";
		inline static const std::string obj_suffix = ".obj_";

		inline static const std::string vert_shader_suffix = ".vert";
		inline static const std::string frag_shader_suffix = ".fs";

		inline static const std::string obj_shader_path = "../ModelResources/Shader/obj_shader";

		inline static const std::string player_model_path = "../ModelResources/Player/player"; /* �v���C���[���f�� */
		
		inline static const std::string wall_model_path = "../ModelResources/Wall/wall";    /* �ǃ��f�� */
		inline static const std::string stair_model_path = "../ModelResources/Stair/stair"; /* �K�i���f�� */
		inline static const std::string tyle_model_path = "../ModelResources/Tyle/tyle";    /* �����f�� */

		inline static const std::string enemy_model_path = "../ModelResources/Enemy/enemy";    /* �G(beta)���f�� */

		inline static const std::string enemy_model_path_array[] =
		{
			"../ModelResources/Enemy/Enemy1/enemy1" , /* �G���f��(�A�q��) */
			"../ModelResources/Enemy/Enemy2/enemy2" , /* �G���f��(�a) */
			"../ModelResources/Enemy/Enemy3/enemy3" , /* �G���f��(�r�V���b�v) */
			"../ModelResources/Enemy/Enemy4/enemy4" , /* �G���f��(��) */
			"../ModelResources/Enemy/Enemy5/enemy5"   /* �G���f��(���ڂ���) */
		};

		inline static const int ENEMY_MODEL_NUM = (sizeof(enemy_model_path_array) / sizeof(enemy_model_path_array[0]));

	};
}

#endif