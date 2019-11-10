#ifndef __RESULT_SCENE_HPP__
#define __RESULT_SCENE_HPP__

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "../BaseScene/BaseScene.hpp"

#include "..//..//PrintString/PrintString.hpp"

#include "..//Scene.hpp"

class ResultScene : public BaseScene
{
	enum class BUTTON : int
	{
		OK = static_cast<int>('s'),
		NONE = 0
	};

	const int width = 900, height = 600; /* ��ʃT�C�Y(main�Ɠ���) */

	BUTTON key_pos;

	PrintString print_manager;

	void View2D();       /* ���ˉe���[�h */
	void View3D();       /* �������e���[�h */

	public:
		ResultScene();
		~ResultScene();

		void Update() override;
		void Draw()   override;
		void KeyInput(const bool* key_now,const bool* key_prev) override;
};

#endif
