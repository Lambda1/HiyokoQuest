#ifndef __RESOURCE_PATH_HPP__
#define __RESOURCE_PATH_HPP__

#include <string>

namespace FILE_PATH
{
	class ResoucePath
	{
	public:
		ResoucePath() = delete;
		~ResoucePath() = delete;

		inline static const std::string bmp_suffix = ".bmp";
		inline static const std::string obj_suffix = ".obj";

		inline static const std::string vert_shader_suffix = ".vert";
		inline static const std::string frag_shader_suffix = ".fs";

		inline static const std::string obj_shader_path = "../ModelResources/Shader/obj_shader";

		inline static const std::string player_model_path = "../ModelResources/Player/player";
	};
}

#endif