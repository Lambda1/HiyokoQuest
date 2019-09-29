#include "./PrintString.hpp"

PrintString::PrintString()
{
}

PrintString::~PrintString()
{
}

void PrintString::DrawStrings(const std::string str,const float x,const float y,const float z, const PS::COLOR color)
{
	glPushMatrix();
	glColor3fv(PS::CL_TABLE[static_cast<int>(color)]);
	glRasterPos3f(x,y,z);
	for(char ch : str) { glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ch); }
	glPopMatrix();
}
