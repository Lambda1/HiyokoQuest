#include "./SceneManager.hpp"

SceneManager::SceneManager() :
	m_scene(nullptr), now_scene(SCENE::TITLE),
	key_buffer_now(nullptr), key_buffer_prev(nullptr)
{
	key_buffer_now = new bool[alphabet];
	key_buffer_prev = new bool[alphabet];
	for (int i = 0; i < alphabet; i++) key_buffer_now[i] = key_buffer_prev[i] = false;
}

SceneManager::~SceneManager()
{
	if (m_scene) delete m_scene;
	if (key_buffer_now) delete[] key_buffer_now;
	if (key_buffer_prev) delete[] key_buffer_prev;
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
	m_scene->KeyInput(key_buffer_now, key_buffer_prev);
	m_scene->Update();
}

void SceneManager::Draw()
{
	m_scene->Draw();
}

void SceneManager::KeyInput(const bool* key_buffer)
{
	for (int i = 0; i < alphabet; i++)
	{
		key_buffer_prev[i] = key_buffer_now[i];
		key_buffer_now[i] = key_buffer[i];
	}
}


void SceneManager::TransScene()
{
	if (m_scene->GetScene() != now_scene)
	{
		now_scene = m_scene->GetScene();
		ChangeScene(now_scene);
	}
}