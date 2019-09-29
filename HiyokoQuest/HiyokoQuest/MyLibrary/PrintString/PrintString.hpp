#ifndef __PRINT_STRING_HPP__
#define __PRINT_STRING_HPP__

#include <string>

#include <GL/freeglut.h>

/* PrintStringクラス: 定数定義 */
namespace PS
{
	/* 色定義 */
	enum class COLOR : unsigned char
	{
		RED,
		GREEN,
		BLUE,
		BLACK,
		WHITE,
		SILVER,

		SIZE
	};
	/* RGB数 */
	constexpr int CL_SIZE = 3;
	/* カラーテーブル */
	constexpr float CL_TABLE[static_cast<int>(PS::COLOR::SIZE)][PS::CL_SIZE] =
	{
		1.00f, 0.00f, 0.00f, /* RED    */
		0.00f, 1.00f, 0.00f, /* GREEN  */
		0.00f, 0.00f, 1.00f, /* BLUE   */
		0.00f, 0.00f, 0.00f, /* BLACK  */
		1.00f, 1.00f, 1.00f, /* WHITE  */
		0.55f, 0.55f, 0.55f  /* SILVER */
	};
};

class PrintString
{
	public:
	PrintString();
	~PrintString();

	/* HELVETICAフォント 18pt 描画 */
	/* 引数: 描画文字列, 描画位置(x,y,z), 色 */
	void DrawStrings(const std::string str,const float x,const float y,const float z,const PS::COLOR color);
};

#endif
