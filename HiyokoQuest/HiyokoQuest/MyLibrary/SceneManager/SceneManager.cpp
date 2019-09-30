#include "./SceneManager.hpp"

SceneManager::SceneManager() :
	m_scene(nullptr), now_scene(SCENE::TITLE),
	key_buffer(nullptr), key_buffer_prev(nullptr)
{
	key_buffer = new bool[alphabet];
	key_buffer_prev = new bool[alphabet];
	for (int i = 0; i < alphabet; i++) key_buffer[i] = key_buffer_prev[i] = false;
}

SceneManager::~SceneManager()
{
	if (m_scene) delete m_scene;
	if (key_buffer) delete key_buffer;
	if (key_buffer_prev) delete key_buffer_prev;
}

void SceneManager::ChangeScene(SCENE scene)
{
	if(m_scene) delete m_scene;

	switch(scene)
	{
		case SCENE::TITLE:
			m_scene = new TitleScene(); break;
		case SCENE::GAME:
			m_scene = new GameScene(); break;
		case SCENE::RESULT:
			m_scene = new ResultScene(); break;
		default:
			break;
	}
}

void SceneManager::Update()
{
	m_scene->KeyInput(key_buffer, key_buffer_prev);
	m_scene->Update();
}

void SceneManager::Draw()
{
	m_scene->Draw();
}

void SceneManager::KeyInput(const unsigned char key_on, const unsigned char key_up)
{
	for (int i = 0; i < alphabet; i++) key_buffer_prev[i] = key_buffer[i];
	KeyOn(key_on);
	KeyOff(key_up);
}

void SceneManager::TransScene()
{
	if (m_scene->GetScene() != now_scene)
	{
		now_scene = m_scene->GetScene();
		ChangeScene(now_scene);
	}
}