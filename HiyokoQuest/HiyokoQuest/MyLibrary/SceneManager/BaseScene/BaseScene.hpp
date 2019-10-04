#ifndef __BASE_SCENE_HPP__
#define __BASE_SCENE_HPP__

#include "..//Scene.hpp"

#include <iostream>

/* 基本プログラム: (https://)github.com/Lambda1/my_library/tree/master/Game/BaseScene */

class BaseScene
{
	protected:
		/* シーン処理関係 */
		bool scene_transition; /* シーン移行フラグ */
		bool game_quite;       /* ゲーム終了フラグ */
		SCENE  scene_number;   /* シーン状態 */
		/* キー入力関係 */
		const int  alphabet = 26;
		const char key_bias = static_cast<char>('a');
	public:
		BaseScene() :
			scene_transition(false), game_quite(false), scene_number(SCENE::TITLE)
		{ std::cout << "BASE CON" << std::endl; }
		virtual ~BaseScene(){std::cout << "BASE DES" << std::endl;}
		
		virtual void Update()=0;
		virtual void Draw()=0;
		
		virtual void KeyInput(const bool *key_buffer,const bool *key_buffer_prev)=0;

		inline bool  GetQuit()  { return game_quite; }
		inline SCENE GetScene() { return scene_number; }
};

#endif