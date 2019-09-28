#include "./MyGL.hpp"

constexpr unsigned int MyGL::msecs;
constexpr int MyGL::timer_value;
unsigned int MyGL::deltaTime = 0;

unsigned char MyGL::n_key = 0, MyGL::n_key_up = 0;
int MyGL::sp_key = 0, MyGL::sp_key_up = 0;

void MyGL::Resize(int width,int height)
{
	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30,(double)width/height,1.0,100.0);

	glMatrixMode(GL_MODELVIEW);
}
void MyGL::Timer(int value)
{
	deltaTime++;
	glutTimerFunc(msecs,Timer,timer_value);
}
void MyGL::Keyboard(unsigned char key,int x,int y)
{
	n_key = key;
}
void MyGL::KeyboardUp(unsigned char key,int x,int y)
{
	n_key_up = key;
}
void MyGL::SpKeyboardUp(int key,int x,int y)
{
	sp_key_up = key;
}
void MyGL::DummyDisplay(){}

MyGL::MyGL()
{
}

MyGL::~MyGL()
{
}

void MyGL::Init(std::string name,int width,int height,int argc,char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(width,height);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
	glutCreateWindow(name.c_str());
	glewInit();

	/* ‚±‚±‚©‚ç: “Á‰»ˆ— */
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	/* ‚±‚±‚Ü‚Å: “Á‰»ˆ— */
}

void MyGL::EntryCallBack(const bool timer_flag)
{
	glutDisplayFunc(this->DummyDisplay);
	glutReshapeFunc(this->Resize);
	glutKeyboardFunc(this->Keyboard);
	glutKeyboardUpFunc(this->KeyboardUp);
	glutSpecialUpFunc(this->SpKeyboardUp);
	if(timer_flag) glutTimerFunc(msecs,this->Timer,timer_value);
}

void MyGL::DrawHorizon(int width,int height,float sq_len)
{
	glColor3f(0,0,0);
	glBegin(GL_LINES);
	for(float i = static_cast<float>(-height/2);i < static_cast<float>(height/2);i+=sq_len){
		glVertex3f(static_cast<GLfloat>(-width/2.0),0,i);
		glVertex3f(static_cast<GLfloat>(width/2.0),0,i);
	}
	for(float j = static_cast<float>(-width/2);j < static_cast<float>(width/2);j+=sq_len){
		glVertex3f(j,0, static_cast<GLfloat>(-height/2.0));
		glVertex3f(j,0, static_cast<GLfloat>(height/2.0));
	}
	glEnd();
}
