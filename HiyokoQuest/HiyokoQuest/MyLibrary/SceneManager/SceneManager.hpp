#ifndef __SCENE_MANAGER_HPP__
#define __SCENE_MANAGER_HPP__

#include "./BaseScene/BaseScene.hpp"
#include "./TitleScene/TitleScene.hpp"
#include "./GameScene/GameScene.hpp"
#include "./ResultScene/ResultScene.hpp"

#include "./Scene.hpp"

/* 基本プログラム: (https://)github.com/Lambda1/my_library/tree/master/Game/SceneManager */

class SceneManager
{
	BaseScene* m_scene; /* シーン管理 */
	SCENE now_scene;    /* 現在のシーン */

	/* キー入力管理 */
	/* 今と前を比較することで, 押された・離されたを判定 */
	/* now|prev : TRUE|FALSE => 押された, FALSE|TRUE => 離された */
	const int alphabet = 26;
	const unsigned char judge_base = static_cast<unsigned char>('a');
	bool* key_buffer_now, *key_buffer_prev;

	public:
		SceneManager();
		~SceneManager();

		void ChangeScene(SCENE scene);
		void Update();
		void Draw();

		/* キーボード仲介処理 */
		void KeyInput(const bool* key_buffer);

		/* シーン遷移処理 */
		void TransScene();

		/* ゲーム終了処理 */
		inline bool GameQuite() { return m_scene->GetQuit(); }
};

#endif
