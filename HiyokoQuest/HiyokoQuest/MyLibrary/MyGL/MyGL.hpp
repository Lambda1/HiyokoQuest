#ifndef __MY_GL_HPP__
#define __MY_GL_HPP__

#include <string>

#include <GL/glew.h>
#include <GL/freeglut.h>

/* glut/freeglutの処理ラッパ */
/* Callback関数の設定は, 静的メンバ関数で対応. */
/* 苦肉の策なので, より良い方法が見つかれば変更したい. */

class MyGL
{
	/* Timer関係 */
	constexpr static unsigned int msecs = 10;
	constexpr static int timer_value = 0;
	static unsigned int deltaTime;
	/* Keyboard関係 */
	static unsigned char n_key,n_key_up;
	static int sp_key,sp_key_up;
	/* CallBack関数 */
	static void Resize(int width,int height);
	static void Timer(int value);
	static void Keyboard(unsigned char key,int x,int y);
	static void KeyboardUp(unsigned char key,int x,int y);
	static void SpKeyboardUp(int key,int x,int y);
	static void DummyDisplay();

	public:
	MyGL();
	~MyGL();
	
	/* 初期化 */
	void Init(std::string w_name,int width,int height,int,char**);
	/* CallBack関数の設定 */
	void EntryCallBack(const bool use_timer);
	/* デバッグ用: 地平線描画 */
	void DrawHorizon(int width,int height,float sq_len);

	/* 画面クリア */
	inline void Clear(){ glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); }
	/* ダブルバッファリング */
	inline void Display(){ glutSwapBuffers(); }
	/* Event処理 */
	inline void MainLoop(){ glutMainLoopEvent(); }

	/* Getter */
	/* Timer */
	inline unsigned int GetDeltaTime(){ return deltaTime; }
	inline void ResetDeltaTime(){ deltaTime = 0; }
	/* KeyBoard */
	inline unsigned char GetKey(){ return n_key; };
	inline unsigned char GetKeyUp(){ return n_key_up; };
	inline int GetSpKey(){ return sp_key; };
	inline int GetSpKeyUp(){ return sp_key_up; };
};

#endif
