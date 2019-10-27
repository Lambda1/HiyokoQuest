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

		inline static const std::string player_model_path = "../ModelResources/Player/player"; /* プレイヤーモデル */
		
		inline static const std::string wall_model_path = "../ModelResources/Wall/wall";    /* 壁モデル */
		inline static const std::string stair_model_path = "../ModelResources/Stair/stair"; /* 階段モデル */
		inline static const std::string tyle_model_path = "../ModelResources/Tyle/tyle";    /* 床モデル */

		inline static const std::string enemy_model_path = "../ModelResources/Enemy/enemy";    /* 敵モデル */
	};
}

#endif