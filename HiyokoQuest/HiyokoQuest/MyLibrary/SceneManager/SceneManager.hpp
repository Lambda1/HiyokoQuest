#ifndef __SCENE_MANAGER_HPP__
#define __SCENE_MANAGER_HPP__

#include "./BaseScene/BaseScene.hpp"
#include "./TitleScene/TitleScene.hpp"
#include "./GameScene/GameScene.hpp"
#include "./ResultScene/ResultScene.hpp"

#include "./Scene.hpp"

/* ��{�v���O����: (https://)github.com/Lambda1/my_library/tree/master/Game/SceneManager */

class SceneManager
{
	BaseScene* m_scene; /* �V�[���Ǘ� */
	SCENE now_scene;    /* ���݂̃V�[�� */

	/* �L�[���͊Ǘ� */
	/* ���ƑO���r���邱�Ƃ�, �����ꂽ�E�����ꂽ�𔻒� */
	/* now|prev : TRUE|FALSE => �����ꂽ, FALSE|TRUE => �����ꂽ */
	const int alphabet = 26;
	const unsigned char judge_base = static_cast<unsigned char>('a');
	bool* key_buffer_now, *key_buffer_prev;

	public:
		SceneManager();
		~SceneManager();

		void ChangeScene(SCENE scene);
		void Update();
		void Draw();

		/* �L�[�{�[�h����� */
		void KeyInput(const bool* key_buffer);

		/* �V�[���J�ڏ��� */
		void TransScene();

		/* �Q�[���I������ */
		inline bool GameQuite() { return m_scene->GetQuit(); }
};

#endif
