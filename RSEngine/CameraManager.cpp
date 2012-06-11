#include "CameraManager.h"
#include "CameraClass.h"

CameraManager::CameraManager(void)
{
}


CameraManager::~CameraManager(void)
{
}

CameraManager* CameraManager::m_instance = 0;

CameraManager* CameraManager::getInstance()
{

	if (m_instance == 0)
	{
		m_instance = new CameraManager();
	}
	return m_instance;
}

CameraClass* CameraManager::getCamera(std::string tag)
{
	for (std::vector<CameraClass*>::iterator iter = m_cameraList.begin();
		iter != m_cameraList.end(); ++iter)
	{
		if ((*iter)->NameEqual(tag))
			return (*iter);
	}
	return NULL;
}

void CameraManager::insertCamera(CameraClass* cc)
{
	m_cameraList.push_back(cc);
}

void CameraManager::Shutdown()
{
	for (std::vector<CameraClass*>::iterator iter = m_cameraList.begin();
		iter != m_cameraList.end(); ++iter)
	{
		delete (*iter);
		(*iter) = 0;
	}
}