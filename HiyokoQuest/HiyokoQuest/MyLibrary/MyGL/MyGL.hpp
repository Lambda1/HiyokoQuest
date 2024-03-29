#ifndef __MY_GL_HPP__
#define __MY_GL_HPP__

#include <string>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

/* glut/freeglutの処理ラッパ */
/* Callback関数の設定は, 静的メンバ関数で対応. */
/* 苦肉の策なので, より良い方法が見つかれば変更したい. */

/* 基本プログラム: (https://)github.com/Lambda1/my_library/tree/master/OpenGL/MyGL */

class MyGL
{
	/* Timer関係 */
	constexpr static unsigned int msecs = 10;
	constexpr static int timer_value = 0;
	static unsigned int deltaTime;
	/* Keyboard関係 */
	/* glutのキーボード操作に同時判定がないため, バッファリングで対応 */
	constexpr static unsigned char key_bias_start = 'a', key_bias_end = 'z';
	const int alphabet = 26;
	static int sp_key,sp_key_up;
	static bool* key_buffer;

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
	inline int GetSpKey(){ return sp_key; };
	inline int GetSpKeyUp() { int dummy = sp_key_up; sp_key_up = 0; return dummy; };
	inline bool* GetKeyBuffer() { return key_buffer; }
};

#endif