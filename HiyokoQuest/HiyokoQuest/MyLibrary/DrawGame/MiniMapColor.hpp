#ifndef __MINI_MAP_COLOR_HPP__
#define __MINI_MAP_COLOR_HPP__

#include <GL/glew.h>
#include <GL/freeglut.h>

class MiniMapColor
{
	MiniMapColor() = delete;
	public:
	/* �F��` */
	enum class COLOR : unsigned char
	{
		RED,
		GREEN,
		BLUE,
		YELLOW,
		PURPLE,
		ORANGE,
		SIZE
	};
	/* RGB�� */
	inline static const int RGBA_SIZE = 4;
	inline static const float ALPHA = 0.7f;
	/* �J���[�e�[�u�� */
	inline static const GLfloat CL_TABLE[static_cast<int>(COLOR::SIZE)][RGBA_SIZE] =
	{
		1.00f, 0.00f, 0.00f, ALPHA, /* RED    */
		0.00f, 1.00f, 0.00f, ALPHA, /* GREEN  */
		0.00f, 0.00f, 1.00f, ALPHA, /* BLUE   */
		1.00f, 1.00f, 0.00f, ALPHA, /* YELLOW */
		0.50f, 0.00f, 0.50f, ALPHA, /* PURPLE */
		1.00f, 0.55f, 0.00f, ALPHA  /* ORANGE */
	};
};

#endif
