#ifndef __BASE_SCENE_HPP__
#define __BASE_SCENE_HPP__

#include <iostream>

/* Šî–{ƒvƒƒOƒ‰ƒ€: (https://)github.com/Lambda1/my_library/tree/master/Game/BaseScene */

class BaseScene
{
	protected:
		const int  alphabet = 26;
		const char key_bias = static_cast<char>('a');
	public:
		BaseScene(){ std::cout << "BASE CON" << std::endl; }
		virtual ~BaseScene(){std::cout << "BASE DES" << std::endl;}
		
		virtual void Update()=0;
		virtual void Draw()=0;
		
		virtual void KeyInput(bool *key, bool *key_prev)=0;
};

#endif
