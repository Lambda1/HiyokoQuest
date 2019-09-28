#ifndef __SCENE_MANAGER_HPP__
#define __SCENE_MANAGER_HPP__

#include "./BaseScene/BaseScene.hpp"
#include "./TitleScene/TitleScene.hpp"
#include "./GameScene/GameScene.hpp"
#include "./ResultScene/ResultScene.hpp"

/* ��{�v���O����: (https://)github.com/Lambda1/my_library/tree/master/Game/SceneManager */

enum class SCENE : int { TITLE, GAME, RESULT };

class SceneManager
{
	BaseScene* m_scene; /* �V�[���Ǘ� */

	/* �L�[���͊Ǘ� */
	/* ���ƑO���r���邱�Ƃ�, �����ꂽ�E�����ꂽ�𔻒� */
	/* now|prev : TRUE|FALSE => �����ꂽ, FALSE|TRUE => �����ꂽ */
	const int alphabet = 26;
	const unsigned char judge_base = static_cast<unsigned char>('a');
	bool *key_buffer, *key_buffer_prev;

	public:
		SceneManager();
		~SceneManager();

		void ChangeScene(SCENE scene);
		void Update();
		void Draw();

		/* �L�[�{�[�h����� */
		void KeyInput(const unsigned char key_on, const unsigned char key_up);
		inline void KeyOn(const unsigned char key_on)
		{
			int diff = static_cast<int>(key_on - judge_base);
			if (diff < 0 || diff > alphabet) return;
			key_buffer[diff] = true;
		}
		inline void KeyOff(const unsigned char key_up)
		{
			int diff = static_cast<int>(key_up - judge_base);
			if (diff < 0 || diff > alphabet) return;
			key_buffer[diff] = false;
		}
};

#endif
